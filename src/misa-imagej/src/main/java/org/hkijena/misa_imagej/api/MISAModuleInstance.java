/*
 * Copyright by Ruman Gerst
 * Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge
 * https://www.leibniz-hki.de/en/applied-systems-biology.html
 * HKI-Center for Systems Biology of Infection
 * Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)
 * Adolf-Reichwein-Straße 23, 07745 Jena, Germany
 *
 * This code is licensed under BSD 2-Clause
 * See the LICENSE file provided with this code for the full license.
 */

package org.hkijena.misa_imagej.api;

import com.google.common.collect.BiMap;
import com.google.common.collect.HashBiMap;
import com.google.common.collect.Maps;
import com.google.common.eventbus.EventBus;
import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import org.hkijena.misa_imagej.api.json.JSONSchemaObject;
import org.hkijena.misa_imagej.api.json.JSONSchemaObjectType;
import org.hkijena.misa_imagej.api.repository.MISAModule;
import org.hkijena.misa_imagej.api.repository.MISAModuleInfo;
import org.hkijena.misa_imagej.utils.FilesystemUtils;
import org.hkijena.misa_imagej.utils.GsonUtils;
import org.hkijena.misa_imagej.utils.OSUtils;
import org.hkijena.misa_imagej.utils.OperatingSystem;

import java.io.IOException;
import java.nio.file.Path;
import java.util.Map;

public class MISAModuleInstance implements MISAValidatable {

    /**
     * JSON schema object for runtime parameters
     */
    private JSONSchemaObject algorithmParameters;

    /**
     * JSON schema object for runtime parameters
     */
    private JSONSchemaObject runtimeParameters;

    /**
     * Template used for sample parameters
     */
    private JSONSchemaObject sampleParametersTemplate;

    private MISAFilesystemEntry sampleImportedFilesystemTemplate;

    private MISAFilesystemEntry sampleExportedFilesystemTemplate;

    /**
     * The samples
     */
    private BiMap<String, MISASample> samples = HashBiMap.create();

    private MISAModuleInfo moduleInfo;

    private MISAModule module;

    private String name = "module";

    private EventBus eventBus = new EventBus();

    public MISAModuleInstance(JSONSchemaObject object) {
        algorithmParameters = object.getProperties().get("algorithm");
        runtimeParameters = object.getProperties().get("runtime");
        sampleParametersTemplate = object.getProperties().get("samples").getAdditionalPropertiesTemplate();
        sampleImportedFilesystemTemplate = new MISAFilesystemEntry(null,
                object.getPropertyFromPath("filesystem", "json-data", "imported", "children").getAdditionalPropertiesTemplate(),
                "",
                MISACacheIOType.Imported);
        sampleExportedFilesystemTemplate = new MISAFilesystemEntry(null,
                object.getPropertyFromPath("filesystem", "json-data", "exported", "children").getAdditionalPropertiesTemplate(),
                "",
                MISACacheIOType.Exported);
    }

    public BiMap<String, MISASample> getSamples() {
        return Maps.unmodifiableBiMap(samples);
    }

    public MISASample getOrCreateAnySample() {
        if(samples.isEmpty())
            addSample("__OBJECT__");
        return samples.values().stream().findFirst().get();
    }

    public void addSample(String name) {
        if (samples.containsKey(name)) {
            int counter = 1;
            while (samples.containsKey(name + " (" + counter + ")")) {
                ++counter;
            }
            name = name + " (" + counter + ")";
        }
        MISASample sample = new MISASample(this,
                (JSONSchemaObject) sampleParametersTemplate.clone(),
                (MISAFilesystemEntry) sampleImportedFilesystemTemplate.clone(),
                (MISAFilesystemEntry) sampleExportedFilesystemTemplate.clone());
        samples.put(name, sample);
        getEventBus().post(new AddedSampleEvent(sample));
    }

    public void removeSample(String name) {
        MISASample removed = samples.get(name);
        samples.remove(name);
        getEventBus().post(new RemovedSampleEvent(removed, name));
    }

    public void removeSample(MISASample sample) {
        String sampleName = sample.getName();
        samples.remove(sample.getName());
        getEventBus().post(new RemovedSampleEvent(sample, sampleName));
    }

    public boolean renameSample(MISASample sample, String newName) {
        if(newName == null || newName.isEmpty())
            return false;
        if(samples.containsKey(newName))
            return false;
        String oldName = sample.getName();
        samples.remove(oldName);
        samples.put(newName, sample);
        getEventBus().post(new RenamedSampleEvent(oldName, sample));
        return true;
    }

    public MISASample getSample(String name) {
        return samples.get(name);
    }

    public JSONSchemaObject getAlgorithmParameters() {
        return algorithmParameters;
    }

    public JSONSchemaObject getRuntimeParameters() {
        return runtimeParameters;
    }

    /**
     * Returns the parameters as JSON object
     *
     * @return
     */
    public JsonElement getParametersAsJson(Path importedDirectory, Path exportedDirectory) {
        JSONSchemaObject parameters = new JSONSchemaObject(JSONSchemaObjectType.jsonObject);

        // Save properties
        parameters.addProperty("algorithm", algorithmParameters);
        parameters.addProperty("runtime", runtimeParameters);
        parameters.addProperty("samples", new JSONSchemaObject(JSONSchemaObjectType.jsonObject));

        boolean filesystemNeeedsSymlinks = false;
        for (MISASample sample : samples.values()) {
            parameters.getPropertyFromPath("samples").addProperty(sample.getName(), sample.getParameters());

            // Detect if we need symlinking
            filesystemNeeedsSymlinks |= sample.getImportedFilesystem().hasExternalPathDefinitions();
            filesystemNeeedsSymlinks |= sample.getExportedFilesystem().hasExternalPathDefinitions();

        }

        // Windows does not support creation of symlinks (by default)
        // If we are on Windows, we need to use a "json-data" filesystem to redirect the symlinks
        if(filesystemNeeedsSymlinks && !FilesystemUtils.symlinkCreationAvailable()) {
            MISAFilesystemEntry rootImportFilesystem = new MISAFilesystemEntry(null, "", MISACacheIOType.Imported);
            rootImportFilesystem.setExternalPath(importedDirectory);

            MISAFilesystemEntry rootExportFilesystem = new MISAFilesystemEntry(null, "", MISACacheIOType.Exported);
            rootExportFilesystem.setExternalPath(exportedDirectory);

            for (MISASample sample : samples.values()) {
                sample.getImportedFilesystem().setParent(rootImportFilesystem);
                sample.getExportedFilesystem().setParent(rootExportFilesystem);
                rootImportFilesystem.insert(sample.getName(), sample.getImportedFilesystem());
                rootExportFilesystem.insert(sample.getName(), sample.getExportedFilesystem());
            }

            parameters.ensurePropertyFromPath("filesystem").addProperty("source", JSONSchemaObject.createString("json"));
            parameters.ensurePropertyFromPath("filesystem", "json-data", "imported").setValue(rootImportFilesystem.toJson());
            parameters.ensurePropertyFromPath("filesystem", "json-data", "exported").setValue(rootExportFilesystem.toJson());

            // Undo changes to filesystem
            for (MISASample sample : samples.values()) {
                sample.getImportedFilesystem().setParent(null);
                sample.getExportedFilesystem().setParent(null);
            }
        }
        else {
            parameters.ensurePropertyFromPath("filesystem").addProperty("source", JSONSchemaObject.createString("directories"));
            parameters.ensurePropertyFromPath("filesystem").addProperty("input-directory",
                    JSONSchemaObject.createString(importedDirectory.toString()));
            parameters.ensurePropertyFromPath("filesystem").addProperty("output-directory",
                    JSONSchemaObject.createString(exportedDirectory.toString()));
        }

        return parameters.toJson();
    }

    /**
     * Writes the final JSON parameter
     *
     * @param parameterJsonPath where the parameter will be written
     * @param importedDirectory The physical path of the import directory where ImageJ data is exported if needed.
     * @param exportedDirectory The physical path of the export directory where everything will be cached
     * @param forceCopy         If true, the importer will copy the files into the imported directory even if not necessary
     */
    public void install(Path parameterJsonPath, Path importedDirectory, Path exportedDirectory, boolean forceCopy, boolean relativeDirectories) {

        // Install imported data into their proper filesystem locations
        // Important: This MUST be done before creating the parameter JSON!
        for (MISASample sample : samples.values()) {
            for (MISACache cache : sample.getImportedCaches()) {
                // IMPORTANT: Reset external path
                cache.getFilesystemEntry().setExternalPath(null);

                Path cachePath = importedDirectory.resolve(sample.getName()).resolve(cache.getFilesystemEntry().getInternalPath());
                cache.install(cachePath, forceCopy);
            }
        }

        // Write Parameters
        JsonElement parameterJson;
        if (relativeDirectories) {
            parameterJson = getParametersAsJson(parameterJsonPath.getParent().relativize(importedDirectory),
                    parameterJsonPath.getParent().relativize(exportedDirectory));
        } else {
            parameterJson = getParametersAsJson(importedDirectory, exportedDirectory);
        }

        try {
            GsonUtils.toJsonFile(GsonUtils.getGson(), parameterJson, parameterJsonPath);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }


    }

    /**
     * Generates the parameter schema report
     *
     * @return
     */
    @Override
    public MISAValidityReport getValidityReport() {
        MISAValidityReport report = new MISAValidityReport();

        report.merge(algorithmParameters.getValidityReport(), "Algorithm parameters");
        report.merge(runtimeParameters.getValidityReport(), "Runtime parameters");

        for (MISASample sample : samples.values()) {
            report.merge(sample.getValidityReport(), "Samples", sample.getName());
        }

        if(OSUtils.detectOperatingSystem() == OperatingSystem.Windows) {
            double num_threads = (double) runtimeParameters.getPropertyFromPath("num-threads").getValue();
            if(num_threads > 1) {
                report.report(runtimeParameters.getPropertyFromPath("num-threads"),
                        "Known issues",
                        MISAValidityReport.Entry.Type.Warning,
                        "Running multi-threaded Windows-applications within ImageJ can cause freezes. Consider using the 'Export' functionality.");
            }
        }

        return report;
    }

    /**
     * Loads parameters from a JSON file
     * @param parameterFile
     * @param samplePolicy
     * @throws IOException
     */
    public void loadParameters(Path parameterFile, MISASamplePolicy samplePolicy) throws IOException {
        Gson gson = GsonUtils.getGson();
        this.loadParameters(GsonUtils.fromJsonFile(gson, parameterFile, JsonObject.class),
                MISASamplePolicy.createMissingSamples);
    }

    /**
     * Load parameters from the provided JSON
     *
     * @param root
     */
    public void loadParameters(JsonObject root, MISASamplePolicy samplePolicy) {
        // Add missing samples & merge their parameters
        if (root.has("samples")) {
            for (Map.Entry<String, JsonElement> kv : root.getAsJsonObject("samples").entrySet()) {
                if (!samples.containsKey(kv.getKey())) {
                    if(samplePolicy != MISASamplePolicy.createMissingSamples)
                        continue;
                    addSample(kv.getKey());
                }
                samples.get(kv.getKey()).getParameters().setValueFromJson(kv.getValue(), false);
            }
        }

        // Merge parameters
        if (root.has("algorithm")) {
            algorithmParameters.setValueFromJson(root.get("algorithm"), false);
        }
        if (root.has("runtime")) {
            runtimeParameters.setValueFromJson(root.get("runtime"), false);
        }

    }

    public MISAModuleInfo getModuleInfo() {
        return moduleInfo;
    }

    public void setModuleInfo(MISAModuleInfo moduleInfo) {
        this.moduleInfo = moduleInfo;
    }

    public MISAModule getModule() {
        return module;
    }

    public void setModule(MISAModule module) {
        this.module = module;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public EventBus getEventBus() {
        return eventBus;
    }

    public static class AddedSampleEvent {
        private MISASample sample;

        public AddedSampleEvent(MISASample sample) {
            this.sample = sample;
        }

        public MISASample getSample() {
            return sample;
        }
    }

    public static class RemovedSampleEvent {
        private MISASample sample;
        private String removedSampleName;

        public RemovedSampleEvent(MISASample sample, String removedSampleName) {
            this.sample = sample;
            this.removedSampleName = removedSampleName;
        }

        public MISASample getSample() {
            return sample;
        }

        public String getRemovedSampleName() {
            return removedSampleName;
        }
    }

    public static class RenamedSampleEvent {
        private String oldName;
        private MISASample sample;

        public RenamedSampleEvent(String oldName, MISASample sample) {
            this.oldName = oldName;
            this.sample = sample;
        }

        public MISASample getSample() {
            return sample;
        }

        public String getOldName() {
            return oldName;
        }
    }
}

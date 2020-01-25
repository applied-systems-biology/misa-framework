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

import org.hkijena.misa_imagej.api.json.JSONSchemaObject;

import java.awt.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Models a data sample
 */
public class MISASample implements MISAValidatable {

    private MISAModuleInstance moduleInstance;

    /**
     * Parametes of this sample
     */
    private JSONSchemaObject parameters;

    private MISAFilesystemEntry importedFilesystem;

    private MISAFilesystemEntry exportedFilesystem;

    private List<MISACache> importedCaches = new ArrayList<>();

    private List<MISACache> exportedCaches = new ArrayList<>();

    public MISASample(MISAModuleInstance moduleInstance, JSONSchemaObject parameters, MISAFilesystemEntry importedFilesystem, MISAFilesystemEntry exportedFilesystem) {
        this.moduleInstance = moduleInstance;
        this.parameters = parameters;
        this.importedFilesystem = importedFilesystem;
        this.exportedFilesystem = exportedFilesystem;

        // Look for caches
        importedFilesystem.findCaches(this, getImportedCaches());
        exportedFilesystem.findCaches(this, getExportedCaches());

        // Update parameters
        this.parameters.setId("Sample");
        this.parameters.update();
    }

    public JSONSchemaObject getParameters() {
        return parameters;
    }

    /**
     * The imported filesystem of this sample
     */
    public MISAFilesystemEntry getImportedFilesystem() {
        return importedFilesystem;
    }

    /**
     * The exported filesystem of this sample
     */
    public MISAFilesystemEntry getExportedFilesystem() {
        return exportedFilesystem;
    }

    public List<MISACache> getImportedCaches() {
        return importedCaches;
    }

    public List<MISACache> getExportedCaches() {
        return exportedCaches;
    }

    @Override
    public String toString() {
        return getName();
    }

    public Color toColor() {
        return nameToColor(getName());
    }

    public static Color nameToColor(String name) {
        if(name == null)
            return Color.WHITE;
        float h = Math.abs(name.hashCode() % 256) / 255.0f;
        return Color.getHSBColor(h, 0.8f, 0.8f);
    }

    @Override
    public MISAValidityReport getValidityReport() {
        MISAValidityReport report = new MISAValidityReport();

        report.merge(parameters.getValidityReport(), "Parameters");
        for(MISACache cache : importedCaches) {
            report.merge(cache.getValidityReport(), "Data", "Input");
        }
        for(MISACache cache : exportedCaches) {
            report.merge(cache.getValidityReport(), "Data", "Output");
        }

        return report;
    }

    public MISAModuleInstance getModuleInstance() {
        return moduleInstance;
    }

    public MISACache getExportedCacheByRelativePath(String path) {
        return getExportedCaches().stream().filter(misaCache -> misaCache.getRelativePath().equals(path)).findFirst().orElse(null);
    }

    public MISACache getImportedCacheByRelativePath(String path) {
        return getImportedCaches().stream().filter(misaCache -> misaCache.getRelativePath().equals(path)).findFirst().orElse(null);
    }

    /**
     * Name of the sample
     */
    public String getName() {
        return getModuleInstance().getSamples().inverse().get(this);
    }

    /**
     * Finds the best matching cache to a path (imported/exported)/...
     * @param fullCachePath
     * @return
     */
    public MISACache findMatchingCache(String fullCachePath) {
        MISACache matchingCache = null;
        List<MISACache> cacheList;

        if(fullCachePath.startsWith("imported/")) {
            cacheList = getImportedCaches();
        }
        else {
            cacheList = getExportedCaches();
        }

        for(MISACache cache : cacheList) {
            if(fullCachePath.startsWith(cache.getFullRelativePath())) {
                if(matchingCache == null || cache.getRelativePath().length() > matchingCache.getRelativePath().length()) {
                    matchingCache = cache;
                }
            }
        }

        return matchingCache;
    }
}

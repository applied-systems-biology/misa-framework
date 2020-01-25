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

package org.hkijena.misa_imagej.api.repository;

import com.google.gson.Gson;
import com.google.gson.annotations.SerializedName;
import org.apache.commons.exec.CommandLine;
import org.hkijena.misa_imagej.api.MISAModuleInstance;
import org.hkijena.misa_imagej.api.json.JSONSchemaObject;
import org.hkijena.misa_imagej.utils.*;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * MISA++ module located within a repository
 */
public class MISAModule {

    @SerializedName("executable-path")
    private String executablePath;

    @SerializedName("operating-system")
    private OperatingSystem operatingSystem;

    @SerializedName("architecture")
    private OperatingSystemArchitecture operatingSystemArchitecture;

    /**
     * Contains the info about this module
     */
    private transient MISAModuleInfo moduleInfo;

    /**
     * Contains the path where this module was defined
     */
    transient String linkPath;

    /**
     * Parameter schema queried from the executable
     */
    private transient String parameterSchema;

    /**
     * The human-readable README in markdown format
     * Queried from executable
     */
    private transient String readme;

    public MISAModule() {

    }

    /**
     * Returns or queries the module info
     * @return
     */
    public MISAModuleInfo getModuleInfo() {
        if(moduleInfo == null && isCompatible()) {
            String infoString = queryModuleInfo();
            if(infoString != null) {
                Gson gson = GsonUtils.getGson();
                moduleInfo = gson.fromJson(infoString, MISAModuleInfo.class);
            }
        }
        return moduleInfo;
    }

    /**
     * Finds the executable that matches best to the current operating system
     * If no matching executable is found, NULL is returned
     * @return
     */
    public boolean isCompatible() {
        OperatingSystem os = OSUtils.detectOperatingSystem();
        OperatingSystemArchitecture arch = OSUtils.detectArchitecture();
        return OSUtils.isCompatible(os, arch, getOperatingSystem(), getOperatingSystemArchitecture());
    }

    /**
     * Returns the parameter schema JSON if applicable
     * @return The parameter schema. If no matching executable is found or the executable crashes, returns null
     */
    public String getParameterSchemaJSON() {
        if(parameterSchema == null && isCompatible()) {
            parameterSchema = queryParameterSchema();
        }
        return parameterSchema;
    }

    /**
     * Returns the README if applicable
     * @return null if the module is incompatible or crashes
     */
    public String getREADME() {
        if(readme == null && isCompatible()) {
            readme = queryReadme();
        }
        return readme;
    }

    public String getLinkPath() {
        return linkPath;
    }

    /**
     * Returns the parameter schema if possible
     * @return The parameter Schema JSON if successful. Otherwise null.
     */
    private String queryParameterSchema() {
        try {
            Path tmppath = Files.createTempFile("MISAParameterSchema", ".json");
            int result = ProcessUtils.executeFast(getExecutablePath(), "--write-parameter-schema", tmppath.toString());
            if(result == 0) {
                return new String(Files.readAllBytes(tmppath));
            }
            else {
                System.err.println("Unable to load parameter schema from " + getExecutablePath());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * Returns the README if possible
     * @return The README JSON if successful. Otherwise null.
     */
    private String queryReadme() {
        try {
            Path tmppath = Files.createTempFile("MISA_README", ".md");
            int result = ProcessUtils.executeFast(getExecutablePath(), "--write-readme", tmppath.toString());
            if(result == 0) {
                return new String(Files.readAllBytes(tmppath));
            }
            else {
                System.err.println("Unable to query README from " + getExecutablePath());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public CommandLine run(Path parameters) {
        CommandLine commandLine = new CommandLine(getExecutablePath().toFile());
        commandLine.addArgument("--parameters");
        commandLine.addArgument(parameters.toString());
        return commandLine;
    }

    private String queryModuleInfo() {
       String result = ProcessUtils.queryFast(getExecutablePath(), "--module-info");
       if(result == null)
           System.err.println("Unable to query module info from " + getExecutablePath());
       return result;
    }

    @Override
    public String toString() {
        if(getModuleInfo() == null)
            return "<Unable to load>";
        else
            return getModuleInfo().toString();
    }

    /**
     * Generates a filename for this module.
     * Does not include an extension
     * @return
     */
    public String getGeneratedFileName() {
        return getModuleInfo().getId() + "-" + getModuleInfo().getVersion() + "-" + getOperatingSystem().toString() + "-" + getOperatingSystemArchitecture().toString();
    }

    /**
     * Creates a new module instance
     * @return
     */
    public MISAModuleInstance instantiate() {
        Gson gson = GsonUtils.getGson();
        JSONSchemaObject schema = gson.fromJson(getParameterSchemaJSON(), JSONSchemaObject.class);
        schema.setId("parameters");
        schema.update();
        MISAModuleInstance instance = new MISAModuleInstance(schema);
        instance.setModuleInfo(getModuleInfo());
        instance.setModule(this);
        return instance;
    }

    public Path getExecutablePath() {
        Path path = Paths.get(executablePath);
        if(!path.isAbsolute()) {
            return Paths.get(linkPath).getParent().resolve(executablePath);
        }
        return Paths.get(executablePath);
    }

    public void setExecutablePath(Path executablePath) {
        this.executablePath = executablePath.toString();
    }

    public OperatingSystem getOperatingSystem() {
        return operatingSystem;
    }

    public void setOperatingSystem(OperatingSystem operatingSystem) {
        this.operatingSystem = operatingSystem;
    }

    public OperatingSystemArchitecture getOperatingSystemArchitecture() {
        return operatingSystemArchitecture;
    }

    public void setOperatingSystemArchitecture(OperatingSystemArchitecture operatingSystemArchitecture) {
        this.operatingSystemArchitecture = operatingSystemArchitecture;
    }
}

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
import ij.IJ;
import org.hkijena.misa_imagej.utils.*;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Manages MISA++ repositories
 */
public class MISAModuleRepository {

    private static MISAModuleRepository instance;

    public static final Path USER_MODULE_PATH = FilesystemUtils.getSystemConfigPath().resolve("MISA-ImageJ").resolve("misa-modules");

    /**
     * Paths where to look for modules
     */
    private List<Path> paths = new ArrayList<>();
    /**
     * Known modules
     */
    private List<MISAModule> modules = new ArrayList<>();

    private MISAModuleRepository() {
        paths.add(USER_MODULE_PATH);
        if(System.getenv().containsKey("MISA_MODULE_LINK_PATHS")) {
            for(String path : System.getenv("MISA_MODULE_LINK_PATHS").split(";")) {
                paths.add(Paths.get(path));
            }
        }
        if(IJ.getDirectory("plugins") != null) {
            paths.add(Paths.get(IJ.getDirectory("plugins")).resolve("misa-modules"));
        }
        if(OSUtils.detectOperatingSystem() == OperatingSystem.Linux) {
            // Detect the search path via ld
            String result = ProcessUtils.queryFast(Paths.get("/usr/bin/ld"), "--verbose");
            Pattern pattern = Pattern.compile("SEARCH_DIR\\(\"=([^\"]+)\"\\);");
            Matcher matcher = pattern.matcher(result);
            while(matcher.find()) {
                paths.add(Paths.get(matcher.group(1) + "/misaxx/modules"));
            }
        }
    }

    public List<MISAModule> getModules() {
        return Collections.unmodifiableList(modules);
    }

    public MISAModule getModule(String name) {
        Optional<MISAModule> result = modules.stream().filter(misaModule -> misaModule.getModuleInfo().getId().equals(name)).findFirst();
        return result.orElse(null);
    }

    public void refresh() {
        modules.clear();
        for(Path path : paths) {
            if(Files.isDirectory(path)) {
                File f = path.toFile();
                File[] files = f.listFiles();
                if(files != null) {
                    for(File entry : files) {
                        if(entry.isFile())
                            tryLoadModule(entry.getAbsolutePath());
                    }
                }
            }
        }
    }

    private void tryLoadModule(String path) {
        Gson gson = GsonUtils.getGson();
        try (InputStreamReader r = new InputStreamReader(new FileInputStream(path))) {
            MISAModule module = gson.fromJson(r, MISAModule.class);
            module.linkPath = path;
            if(module.getModuleInfo() == null) {
                return;
            }

            // Try to load parameter schema
            if(module.getParameterSchemaJSON() == null) {
                return;
            }

            // Check if the executable was already loaded
            if(modules.stream().anyMatch(m -> m.getExecutablePath().equals(module.getExecutablePath()))) {
                return;
            }

            modules.add(module);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static MISAModuleRepository getInstance() {
        if(instance == null)
            instance = new MISAModuleRepository();
        return instance;
    }

    public static void main(String[] args) {
        MISAModuleRepository.getInstance();
    }
}

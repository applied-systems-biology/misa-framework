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

package org.hkijena.misa_imagej.utils;

import com.google.common.io.MoreFiles;
import com.google.common.io.RecursiveDeleteOption;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.attribute.PosixFilePermission;
import java.util.Set;

import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;

public class FilesystemUtils {
    /**
     * Applies equivalent of chmod +x to the path
     * @param path
     * @throws IOException
     */
    public static void addPosixExecutionPermission(Path path) throws IOException {
        if(OSUtils.detectOperatingSystem() == OperatingSystem.Linux) {
            Set<PosixFilePermission> perms = Files.getPosixFilePermissions(path);
            perms.add(PosixFilePermission.OWNER_EXECUTE);
            Files.setPosixFilePermissions(path, perms);
        }
    }

    /**
     * Creates a symbolic link if possible. Otherwise copies the data.
     * This is required for Windows because Windows disables creation of symbolic links by default.
     * @param link
     * @param target
     */
    public static void createSymbolicLinkOrCopy(Path link, Path target) throws IOException {
        try {
            Files.createSymbolicLink(link, target);
        }
        catch (FileSystemException e) {
            if(Files.exists(link)) {
                MoreFiles.deleteRecursively(link, RecursiveDeleteOption.ALLOW_INSECURE);
            }
            copyFileOrFolder(target, link);
        }
    }

    /**
     * Returns whether we can create symlinks or not
     * @return
     */
    public static boolean symlinkCreationAvailable() {
        return OSUtils.detectOperatingSystem() != OperatingSystem.Windows;
    }

    private static void copy(Path source, Path dest) {
        System.out.println("Copying " + source.toString() + " to " + dest.toString());
        try {
            if(!Files.exists(dest.getParent())) {
                Files.createDirectories(dest.getParent());
            }
            Files.copy(source, dest, REPLACE_EXISTING);
        } catch (Exception e) {
            throw new RuntimeException(e.getMessage(), e);
        }

    }

    public static void copyFileOrFolder(Path src, Path dest) throws IOException {
        if(src.toFile().isDirectory()) {
            Files.walk(src)
                    .forEach(source -> {
                        if(Files.isRegularFile(source))
                            copy(source, dest.resolve(src.relativize(source)));
                    });
        }
        else {
            copy(src, dest);
        }
    }

    public static boolean directoryIsEmpty(Path dir) throws IOException {
        File[] entries = dir.toFile().listFiles();
        return entries == null || entries.length == 0;
    }

    /**
     * Returns the configuration path for the current operating system
     * @return
     */
    public static Path getSystemConfigPath() {
        switch (OSUtils.detectOperatingSystem()) {
            case Windows:
                return Paths.get(System.getenv("APPDATA"));
            case Linux:
                if(System.getenv().containsKey("XDG_CONFIG_HOME")) {
                    return Paths.get(System.getenv("XDG_CONFIG_HOME"));
                }
                else {
                    return Paths.get(System.getProperty("user.home")).resolve(".config");
                }
            default:
                throw new UnsupportedOperationException("Unsupported ");
        }
    }
}

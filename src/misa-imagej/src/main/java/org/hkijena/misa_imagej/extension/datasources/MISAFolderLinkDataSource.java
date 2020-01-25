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

package org.hkijena.misa_imagej.extension.datasources;

import com.google.common.io.MoreFiles;
import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISADataSource;
import org.hkijena.misa_imagej.api.MISAValidityReport;
import org.hkijena.misa_imagej.utils.FilesystemUtils;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class MISAFolderLinkDataSource implements MISADataSource {

    private Path sourceFolder;
    private MISACache cache;

    public MISAFolderLinkDataSource(MISACache cache) {
        this.cache = cache;
    }

    @Override
    public void install(Path installFolder, boolean forceCopy) {
        if(forceCopy) {
            try {
                Files.createDirectories(installFolder);
                FilesystemUtils.copyFileOrFolder(sourceFolder, installFolder);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
        else if(FilesystemUtils.symlinkCreationAvailable()) {
            try {
                MoreFiles.createParentDirectories(installFolder);
                Files.deleteIfExists(installFolder);
                Files.createSymbolicLink(installFolder, getSourceFolder());
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
        else {
            // Redirect filesystem path
            getCache().getFilesystemEntry().setExternalPath(sourceFolder);
        }
    }

    @Override
    public String getName() {
        return "Folder link";
    }

    @Override
    public boolean isEditable() {
        return true;
    }

    @Override
    public MISACache getCache() {
        return cache;
    }

    @Override
    public MISAValidityReport getValidityReport() {
        return new MISAValidityReport();
    }

    public Path getSourceFolder() {
        return sourceFolder;
    }

    public void setSourceFolder(Path sourceFolder) {
        this.sourceFolder = sourceFolder;
    }
}

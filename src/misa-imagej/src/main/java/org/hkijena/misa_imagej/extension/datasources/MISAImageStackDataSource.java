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
import org.hkijena.misa_imagej.utils.swappers.ImageStackSwapper;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class MISAImageStackDataSource implements MISADataSource {

    private MISACache cache;
    private ImageStackSwapper stackSwapper;

    public MISAImageStackDataSource(MISACache cache) {
        this.cache = cache;
    }

    public ImageStackSwapper getStackSwapper() {
        return stackSwapper;
    }

    public void setStackSwapper(ImageStackSwapper stackSwapper) {
        this.stackSwapper = stackSwapper;
    }

    @Override
    public void install(Path installFolder, boolean forceCopy) {
        if(stackSwapper.isInFilesystem()) {
            if(forceCopy) {
                try {
                    Files.createDirectories(installFolder);
                    FilesystemUtils.copyFileOrFolder(stackSwapper.getFolderPath(), installFolder);
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
            else if(FilesystemUtils.symlinkCreationAvailable()) {
                try {
                    MoreFiles.createParentDirectories(installFolder);
                    Files.deleteIfExists(installFolder);
                    Files.createSymbolicLink(installFolder, stackSwapper.getFolderPath());
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
            else {
                // Redirect filesystem path
                getCache().getFilesystemEntry().setExternalPath(stackSwapper.getFolderPath());
            }
        }
        else {
            stackSwapper.installToFilesystem(installFolder.toString(), forceCopy);
        }
    }

    @Override
    public String getName() {
        return "Image stack";
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
        if(stackSwapper == null)
            return new MISAValidityReport(this,
                    "Data " + cache.getCacheTypeName() + " " + cache.getRelativePathName(), false, "No data set. Please add data.");
        else if(!stackSwapper.isValid())
            return new MISAValidityReport(this,
                    "Data " + cache.getCacheTypeName() + " " + cache.getRelativePathName(), false, "Data is not present anymore. Did you close the image or remove the file?");
        else
            return new MISAValidityReport(this,
                    "Data " + cache.getCacheTypeName() + " " + cache.getRelativePathName(), true, "");
    }
}

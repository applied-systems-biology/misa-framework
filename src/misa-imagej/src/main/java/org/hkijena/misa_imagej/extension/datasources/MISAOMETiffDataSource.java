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

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISADataSource;
import org.hkijena.misa_imagej.api.MISAValidityReport;
import org.hkijena.misa_imagej.utils.swappers.OMETiffSwapper;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class MISAOMETiffDataSource implements MISADataSource {

    private MISACache cache;
    private OMETiffSwapper tiffSwapper;

    public MISAOMETiffDataSource(MISACache cache) {
        this.cache = cache;
    }

    public OMETiffSwapper getTiffSwapper() {
        return tiffSwapper;
    }

    public void setTiffSwapper(OMETiffSwapper tiffSwapper) {
        this.tiffSwapper = tiffSwapper;
    }

    @Override
    public void install(Path installFolder, boolean forceCopy) {
        try {
            Files.createDirectories(installFolder);
        } catch (IOException e) {
            e.printStackTrace();
        }

        String filename;
        if(tiffSwapper.isInFilesystem())
            filename = Paths.get(tiffSwapper.getPath()).getFileName().toString();
        else if(cache.getFilesystemEntry().getName() == null || cache.getFilesystemEntry().getName().isEmpty())
            filename = "image.ome.tif";
        else
            filename = cache.getFilesystemEntry().getName() + ".ome.tif";

        tiffSwapper.installToFilesystem(installFolder.resolve(filename).toString(), forceCopy);
    }

    @Override
    public String getName() {
        return "OME TIFF";
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
        if(tiffSwapper == null)
            return new MISAValidityReport(this,
                    "Data " + cache.getCacheTypeName() + " " + cache.getRelativePathName(), false, "No data set. Please add data.");
        else if(!tiffSwapper.isValid())
            return new MISAValidityReport(this,
                    "Data " + cache.getCacheTypeName() + " " + cache.getRelativePathName(), false, "Data is not present anymore. Did you close the image or remove the file?");
        else
            return new MISAValidityReport(this,
                    "Data " + cache.getCacheTypeName() + " " + cache.getRelativePathName(), true, "");
    }
}

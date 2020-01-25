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

package org.hkijena.misa_imagej.extension.outputcaches;

import ij.IJ;
import loci.plugins.LociImporter;
import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.workbench.MISAOutput;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Optional;

public class ImageOutputCacheUI extends GenericImageOutputCacheUI {
    public ImageOutputCacheUI(MISAOutput misaOutput, MISACache cache) {
        super(misaOutput, cache);
    }

    @Override
    protected void initialize() {

        if(getFilesystemPath() == null)
            return;

        AbstractButton importBioformatsButton = createButton("Import", UIUtils.getIconFromResources("imagej.png"));
        importBioformatsButton.addActionListener(e -> importImage());

        super.initialize();
    }

    private static boolean isSupportedFileType(Path path) {
        for(String extension : Arrays.asList(".bmp", ".pbm", ".pgm", ".ppm", ".sr", ".ras", ".jpeg", ".jpg", ".jpe", ".tiff", ".tif", ".png")) {
            if(path.toString().endsWith(extension))
                return true;
        }
        return false;
    }

    private void importImage() {
        try {
            Optional<Path> file = Files.list(getFilesystemPath()).filter(ImageOutputCacheUI::isSupportedFileType).findFirst();
            if(!file.isPresent())
                throw new IOException("Could not find a compatible file!");

            IJ.open(file.get().toString());
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
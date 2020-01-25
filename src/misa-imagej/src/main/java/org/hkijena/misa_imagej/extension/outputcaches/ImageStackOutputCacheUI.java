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
import ij.plugin.FolderOpener;
import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.workbench.MISAOutput;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

public class ImageStackOutputCacheUI extends GenericImageOutputCacheUI {
    public ImageStackOutputCacheUI(MISAOutput misaOutput, MISACache cache) {
        super(misaOutput, cache);
    }

    @Override
    protected void initialize() {

        if (getFilesystemPath() == null)
            return;

        AbstractButton importButton = createButton("Import", UIUtils.getIconFromResources("imagej.png"));
        importButton.addActionListener(e -> importImage());

        AbstractButton importIndividuallyButton = createButton("Import individually", UIUtils.getIconFromResources("imagej.png"));
        importIndividuallyButton.addActionListener(e -> importImagesIndividually());

        super.initialize();
    }

    private static boolean isSupportedFileType(Path path) {
        for (String extension : Arrays.asList(".bmp", ".pbm", ".pgm", ".ppm", ".sr", ".ras", ".jpeg", ".jpg", ".jpe", ".tiff", ".tif", ".png")) {
            if (path.toString().endsWith(extension))
                return true;
        }
        return false;
    }

    private void importImagesIndividually() {
        try {
            List<Path> files = Files.list(getFilesystemPath()).filter(ImageStackOutputCacheUI::isSupportedFileType).collect(Collectors.toList());
            if (files.size() > 10) {
                if (JOptionPane.showConfirmDialog(null, "This will import " + files.size() + " images. Proceed?", "Import individually", JOptionPane.YES_NO_OPTION) != JOptionPane.YES_OPTION) {
                    return;
                }
            }

            files.forEach(p -> IJ.open(p.toString()));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private void importImage() {
        FolderOpener opener = new FolderOpener();
        opener.openFolder(getFilesystemPath().toString());
    }
}
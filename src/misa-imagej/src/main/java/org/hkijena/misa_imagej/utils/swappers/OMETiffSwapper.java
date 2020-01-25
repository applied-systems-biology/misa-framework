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

package org.hkijena.misa_imagej.utils.swappers;

import ij.IJ;
import ij.ImagePlus;
import ij.WindowManager;
import org.hkijena.misa_imagej.utils.FilesystemUtils;

import java.awt.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;

/**
 * Handles import and export of OME TIFFs via Bioformats
 * Supports im/export from/to ImageJ and filesystem
 */
public class OMETiffSwapper implements FileSwapper {

    private ImagePlus imageJImage;
    private String path;

    /**
     * Creates a OMETiffSwapper (between memory and filesystem).
     * An OMETiffSwapper can have a window name (for ImageJ access) and/or a file path
     * @param imageJImage
     * @param path
     */
    public OMETiffSwapper(ImagePlus imageJImage, String path) {
        this.imageJImage = imageJImage;
        this.path = path;
    }

    @Override
    public Object getImageJObject() {
        if(imageJImage != null && (imageJImage.getWindow() == null || imageJImage.getWindow().isClosed())) {
            imageJImage = null;
        }
        return imageJImage;
    }

    @Override
    public String getPath() {
        return path;
    }

    @Override
    public boolean isInImageJ() {
        return getImageJObject() != null;
    }

    @Override
    public boolean isInFilesystem() {
        return path != null && Files.exists(Paths.get(path));
    }

    @Override
    public boolean isValid() {
        return  isInImageJ() || isInFilesystem();
    }

    @Override
    public void importIntoImageJ(String id) {
        if(!isInImageJ() && isInFilesystem()) {
            IJ.run("Bio-Formats Importer", "open='" + getPath() + "' color_mode=Default rois_import=[ROI manager] view=Hyperstack stack_order=XYCZT");
            this.imageJImage = WindowManager.getCurrentImage();
        }
        else if(isInImageJ()) {
            // Already in ImageJ. Do nothing here
        }
        else {
            throw new UnsupportedOperationException("The data is neither present in ImageJ, nor located within the filesystem!");
        }
    }

    @Override
    public void exportToFilesystem(String path) {
        if(isInImageJ()) {
            WindowManager.setCurrentWindow(imageJImage.getWindow());
            IJ.run("Bio-Formats Exporter", "save='" + path + "' export compression=Uncompressed");
            this.path = path;
        }
        else if(isInFilesystem()) {
            try {
                Files.copy(Paths.get(this.path), Paths.get(path), StandardCopyOption.REPLACE_EXISTING);
            } catch (IOException e) {
                e.printStackTrace();
            }
            this.path = path;
        }
        else {
            throw new UnsupportedOperationException("The data is neither present in ImageJ, nor located within the filesystem!");
        }
    }

    @Override
    public void installToFilesystem(String path, boolean forceCopy) {
        if(isInImageJ()) {
            // Always export if this is available
            WindowManager.setCurrentWindow(imageJImage.getWindow());
            IJ.run("Bio-Formats Exporter", "save='" + path + "' export compression=Uncompressed");
        }
        else if(isInFilesystem()) {
            if(forceCopy) {
                try {
                    Files.copy(Paths.get(this.path), Paths.get(path), StandardCopyOption.REPLACE_EXISTING);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            else {
                try {
                    FilesystemUtils.createSymbolicLinkOrCopy(Paths.get(path), Paths.get(this.path));
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        else {
            throw new UnsupportedOperationException("The data is neither present in ImageJ, nor located within the filesystem!");
        }
    }

    @Override
    public String toString() {
        if(isInImageJ()) {
            return imageJImage.toString();
        }
        else if(isInFilesystem()) {
            return path;
        }
        else {
            return "Error: Data was removed!";
        }
    }

    /**
     * If the image is already present in ImageJ, select its window
     * otherwise load it into imageJ beforehand
     */
    public void editInImageJ() {
        if(!isInImageJ()) {
            importIntoImageJ(null);
        }
        WindowManager.setCurrentWindow(imageJImage.getWindow());
        EventQueue.invokeLater(() -> imageJImage.getWindow().toFront());
    }
}

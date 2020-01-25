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

import ij.*;
import ij.gui.Overlay;
import ij.gui.Roi;
import ij.io.FileSaver;
import ij.measure.Calibration;
import ij.plugin.FolderOpener;
import ij.plugin.frame.Recorder;
import ij.process.ImageProcessor;
import ij.process.LUT;
import org.hkijena.misa_imagej.utils.FilesystemUtils;

import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.Locale;
import java.util.Properties;

/**
 * Handles import and export of Image stacks
 * Supports im/export from/to ImageJ and filesystem
 */
public class ImageStackSwapper implements FileSwapper {

    private ImagePlus imageJImage;
    private Path folderPath;

    /**
     * Creates a OMETiffSwapper (between memory and filesystem).
     * An OMETiffSwapper can have a window name (for ImageJ access) and/or a file path
     * @param imageJImage
     * @param folderPath
     */
    public ImageStackSwapper(ImagePlus imageJImage, Path folderPath) {
        this.imageJImage = imageJImage;
        this.folderPath = folderPath;
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
        return folderPath.toString();
    }

    public Path getFolderPath() {
        return folderPath;
    }

    @Override
    public boolean isInImageJ() {
        return getImageJObject() != null;
    }

    @Override
    public boolean isInFilesystem() {
        return folderPath != null && Files.exists(folderPath);
    }

    @Override
    public boolean isValid() {
        return  isInImageJ() || isInFilesystem();
    }

    @Override
    public void importIntoImageJ(String id) {
        if(!isInImageJ() && isInFilesystem()) {
            FolderOpener opener = new FolderOpener();
            ImagePlus img = opener.openFolder(folderPath.toString());
            if(img != null) {
                this.imageJImage = img;
            }
        }
        else if(isInImageJ()) {
            // Already in ImageJ. Do nothing here
        }
        else {
            throw new UnsupportedOperationException("The data is neither present in ImageJ, nor located within the filesystem!");
        }
    }

    /**
     * Adapted from ij.plugin.StackWriter code
     */
    private void exportImageJ(String folderPath) {
        ImagePlus imp = imageJImage;
        if (imp==null || (imp!=null && imp.getStackSize()<2&&!IJ.isMacro())) {
            IJ.error("Stack Writer", "This command requires a stack.");
            return;
        }
        int stackSize = imp.getStackSize();
        String name = imp.getTitle();
        int dotIndex = name.lastIndexOf(".");
        if (dotIndex>=0)
            name = name.substring(0, dotIndex);
        boolean hyperstack = imp.isHyperStack();
        LUT[] luts = null;
        int lutIndex = 0;
        int nChannels = imp.getNChannels();
        int[] dim = null;
        boolean firstTime = true;
        int ndigits = 4;
        if (hyperstack) {
            dim = imp.getDimensions();
            if (imp.isComposite())
                luts = ((CompositeImage)imp).getLuts();
            if (firstTime && ndigits==4) {
                ndigits = 3;
                firstTime = false;
            }
        }

        String fileType = "TIFF";
        boolean useLabels = true;

        int number = 0;
        if (ndigits<1) ndigits = 1;
        if (ndigits>8) ndigits = 8;
        int maxImages = (int)Math.pow(10,ndigits);
        if (stackSize>maxImages && !useLabels && !hyperstack) {
            IJ.error("Stack Writer", "More than " + ndigits
                    +" digits are required to generate \nunique file names for "+stackSize+" images.");
            return;
        }
        String format = fileType.toLowerCase(Locale.US);
        if (format.equals("gif") && !FileSaver.okForGif(imp))
            return;
        else if (format.equals("fits") && !FileSaver.okForFits(imp))
            return;

        if (format.equals("text"))
            format = "text image";
        String extension = "." + format;
        if (format.equals("tiff"))
            extension = ".tif";
        else if (format.equals("text image"))
            extension = ".txt";

        String title = "Save Image Sequence";
        String macroOptions = Macro.getOptions();
        String directory = folderPath;
        Overlay overlay = imp.getOverlay();
        boolean isOverlay = overlay!=null && !imp.getHideOverlay();
        if (!(format.equals("jpeg")||format.equals("png")))
            isOverlay = false;
        ImageStack stack = imp.getStack();
        ImagePlus imp2 = new ImagePlus();
        imp2.setTitle(imp.getTitle());
        Calibration cal = imp.getCalibration();
        int nSlices = stack.getSize();
        String path,label=null;
        imp.lock();
        for (int i=1; i<=nSlices; i++) {
            IJ.showStatus("writing: "+i+"/"+nSlices);
            IJ.showProgress(i, nSlices);
            ImageProcessor ip = stack.getProcessor(i);
            if (isOverlay) {
                imp.setSliceWithoutUpdate(i);
                ip = imp.flatten().getProcessor();
            } else if (luts!=null && nChannels>1 && hyperstack) {
                ip.setColorModel(luts[lutIndex++]);
                if (lutIndex>=luts.length) lutIndex = 0;
            }
            imp2.setProcessor(null, ip);
            String label2 = stack.getSliceLabel(i);
            if (label2!=null && label2.indexOf("\n")!=-1)
                imp2.setProperty("Info", label2);
            else {
                Properties props = imp2.getProperties();
                if (props!=null) props.remove("Info");
            }
            imp2.setCalibration(cal);
            String digits = getDigits(number++, hyperstack, dim, ndigits, 0);
            if (useLabels) {
                label = stack.getShortSliceLabel(i);
                if (label!=null && label.equals("")) label = null;
                if (label!=null) label = label.replaceAll("/","-");
            }
            if (label==null)
                path = directory+name+digits+extension;
            else
                path = directory+label+extension;
            if (i==1) {
                File f = new File(path);
                if (f.exists()) {
                    if (!IJ.isMacro() && !IJ.showMessageWithCancel("Overwrite files?",
                            "One or more files will be overwritten if you click \"OK\".\n \n"+path)) {
                        imp.unlock();
                        IJ.showStatus("");
                        IJ.showProgress(1.0);
                        return;
                    }
                }
            }
            if (Recorder.record)
                Recorder.disablePathRecording();
            imp2.setOverlay(null);
            if (overlay!=null && format.equals("tiff")) {
                Overlay overlay2 = overlay.duplicate();
                overlay2.crop(i, i);
                if (overlay2.size()>0) {
                    for (int j=0; j<overlay2.size(); j++) {
                        Roi roi = overlay2.get(j);
                        int pos = roi.getPosition();
                        if (pos==1)
                            roi.setPosition(i);
                    }
                    imp2.setOverlay(overlay2);
                }
            }
            IJ.saveAs(imp2, format, path);
        }
        imp.unlock();
        if (isOverlay) imp.setSlice(1);
        IJ.showStatus("");
    }

    private String getDigits(int n, boolean hyperstack, int[] dim, int ndigits, int startAt) {
        if (hyperstack) {
            int c = (n%dim[2])+1;
            int z = ((n/dim[2])%dim[3])+1;
            int t = ((n/(dim[2]*dim[3]))%dim[4])+1;
            String cs="", zs="", ts="";
            if (dim[2]>1) {
                cs = "00000000"+c;
                cs = "_c"+cs.substring(cs.length()-ndigits);
            }
            if (dim[3]>1) {
                zs = "00000000"+z;
                zs = "_z"+zs.substring(zs.length()-ndigits);
            }
            if (dim[4]>1) {
                ts = "00000000"+t;
                ts = "_t"+ts.substring(ts.length()-ndigits);
            }
            return ts+zs+cs;
        } else {
            String digits = "00000000"+(startAt+n);
            return digits.substring(digits.length()-ndigits);
        }
    }

    @Override
    public void exportToFilesystem(String path) {
        if(isInImageJ()) {
            exportImageJ(path);
            this.folderPath = Paths.get(path);
        }
        else if(isInFilesystem()) {
            throw new UnsupportedOperationException("Unsupported operation (this should be done by the cache!)!");
        }
        else {
            throw new UnsupportedOperationException("The data is neither present in ImageJ, nor located within the filesystem!");
        }
    }

    @Override
    public void installToFilesystem(String path, boolean forceCopy) {
        if(isInImageJ()) {
            exportImageJ(path);
        }
        else if(isInFilesystem()) {
            throw new UnsupportedOperationException("Unsupported operation (this should be done by the cache!)!");
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
            return folderPath.toString();
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

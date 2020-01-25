/**
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

package org.hkijena.segment_glomeruli.tasks;

import net.imglib2.Cursor;
import net.imglib2.RandomAccess;
import net.imglib2.algorithm.morphology.Opening;
import net.imglib2.algorithm.morphology.TopHat;
import net.imglib2.algorithm.neighborhood.HyperSphereShape;
import net.imglib2.img.Img;
import net.imglib2.img.display.imagej.ImageJFunctions;
import net.imglib2.type.numeric.integer.UnsignedByteType;
import net.imglib2.type.numeric.real.FloatType;
import org.hkijena.segment_glomeruli.DataInterface;
import org.hkijena.segment_glomeruli.Filters;

import java.util.Arrays;
import java.util.List;

public class SegmentGlomeruli2D extends DAGTask {
    private int planeZIndex;

    private int medianFilterSize = 3;
    private double glomeruliMinRad = 15;
    private double glomeruliMaxRad = 65;
    private double thresholdPercentile = 75;
    private double thresholdFactor = 1.5;

    public SegmentGlomeruli2D(Integer tid, DataInterface dataInterface, int planeZIndex) {
        super(tid, dataInterface);
        this.planeZIndex = planeZIndex;
    }

    private Img<UnsignedByteType> getPreprocessedImage() {
        Img<UnsignedByteType> importedImage = getDataInterface().getInputData().getOrCreatePlane(planeZIndex);
        Img<FloatType> img = ImageJFunctions.convertFloat(ImageJFunctions.wrap(importedImage, "img"));
        Filters.median(img.copy(), img, medianFilterSize);
        Filters.normalizeByMax(img);
        return Filters.convertFloatToUByte(img);
    }

    @Override
    public void work() {
        System.out.println("Running SegmentGlomeruli2D on " + getDataInterface().getInputData().toString() + " z=" + planeZIndex);
        Img<UnsignedByteType> tissue = getDataInterface().getTissueOutputData().getOrCreatePlane(planeZIndex);

        if(Filters.countNonZero(tissue) == 0) {
            getDataInterface().getGlomeruli2DOutputData().setPlane(planeZIndex, tissue);
            return;
        }

        // Preprocessing
        Img<UnsignedByteType> img8u = getPreprocessedImage();

        // Generated parameters
        final double voxel_xy = getDataInterface().getVoxelSizeXY();
        final int glomeruli_max_morph_disk_radius = (int)(glomeruliMaxRad / voxel_xy);
        final int glomeruli_min_morph_disk_radius = (int)((glomeruliMinRad / 2.0) / voxel_xy);

        // Morphological operation (opening)
        // Corresponds to only allowing objects > disk_size to be included
        // Also subtract the morph result from the initial to remove uneven background + normalize
        img8u = TopHat.topHat(img8u, new HyperSphereShape(glomeruli_max_morph_disk_radius), 1);
        Filters.normalizeByMax(img8u, 255);

        // Get the pixel values only where the tissue is located
        List<UnsignedByteType> kidneyPixels = Filters.getSortedPixelsWhere(img8u, tissue);

        // Only select glomeruli if the threshold is higher than 75-percentile of kidney tissue
        int img8u_tissue_only_percentile = Filters.getPercentiles(kidneyPixels, Arrays.asList(thresholdPercentile)).get(0).getInteger();

        // Threshold the main image
        int otsu_threshold = Filters.Otsu(img8u).getInteger();
        img8u = Filters.threshold(img8u.copy(), new UnsignedByteType(otsu_threshold));

        if(otsu_threshold > img8u_tissue_only_percentile * thresholdFactor ) {

            // Get rid of non-tissue
            {
                RandomAccess<UnsignedByteType> tissueAccess = tissue.randomAccess();
                Cursor<UnsignedByteType> cursor = img8u.cursor();
                while(cursor.hasNext()) {
                    cursor.fwd();
                    tissueAccess.setPosition(cursor);

                    if(tissueAccess.get().getInteger() == 0) {
                        cursor.get().set(0);
                    }
                }
            }

            // Morphological operation (object should have min. radius)
            img8u = Opening.open(img8u, new HyperSphereShape(glomeruli_min_morph_disk_radius), 1);
        }
        else {
            Filters.setTo(img8u, new UnsignedByteType(0));
        }

        getDataInterface().getGlomeruli2DOutputData().setPlane(planeZIndex, img8u);
    }
}

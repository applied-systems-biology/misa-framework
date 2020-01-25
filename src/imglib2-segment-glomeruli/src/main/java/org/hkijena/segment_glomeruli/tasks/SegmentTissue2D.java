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
import net.imglib2.algorithm.gauss3.Gauss3;
import net.imglib2.algorithm.labeling.ConnectedComponents;
import net.imglib2.algorithm.morphology.Dilation;
import net.imglib2.algorithm.morphology.Erosion;
import net.imglib2.algorithm.neighborhood.HyperSphereShape;
import net.imglib2.img.Img;
import net.imglib2.img.array.ArrayImgFactory;
import net.imglib2.img.display.imagej.ImageJFunctions;
import net.imglib2.interpolation.randomaccess.NLinearInterpolatorFactory;
import net.imglib2.interpolation.randomaccess.NearestNeighborInterpolatorFactory;
import net.imglib2.type.numeric.integer.UnsignedByteType;
import net.imglib2.type.numeric.integer.UnsignedIntType;
import net.imglib2.type.numeric.real.FloatType;
import net.imglib2.view.Views;
import org.hkijena.segment_glomeruli.DataInterface;
import org.hkijena.segment_glomeruli.Filters;

import java.util.*;

public class SegmentTissue2D extends DAGTask {

    private int planeZIndex;

    private int medianFilterSize = 3;
    private double downscaleFactor = 10;
    private double thresholdingPercentile = 40;
    private double thresholdingPercentileFactor = 1.5;
    private int morphDiskRadius = 5;
    private double labelMinPercentile = 2;

    public SegmentTissue2D(Integer tid, DataInterface dataInterface, int planeZIndex) {
        super(tid, dataInterface);
        this.planeZIndex = planeZIndex;
    }

    @Override
    public void work() {
        System.out.println("Running SegmentTissue2D on " + getDataInterface().getInputData().toString() + " z=" + planeZIndex);
        final Img<UnsignedByteType> importedImage = getDataInterface().getInputData().getOrCreatePlane(planeZIndex);

        Img<FloatType> img = ImageJFunctions.convertFloat(ImageJFunctions.wrap(importedImage, "img"));
        Filters.median(img.copy(), img, medianFilterSize);
        Filters.normalizeByMax(img);

        final double xsize = getDataInterface().getVoxelSizeXY();
        final double gaussSigma = 3.0 / xsize;

        Img<FloatType> imgSmall = Filters.rescale(img, new NLinearInterpolatorFactory<>(), 1.0 / downscaleFactor, 1.0 / downscaleFactor);

        Gauss3.gauss(gaussSigma, Views.extendZero(imgSmall.copy()), imgSmall);

        final float tissuePercentile = Filters.getPercentiles(Filters.getSortedPixels(imgSmall), Arrays.asList(thresholdingPercentile)).get(0).get();
        final float tissueThreshold = tissuePercentile * (float)thresholdingPercentileFactor;

        // Thresholding & cleaning
        Img<UnsignedByteType> smallMask = Filters.threshold(imgSmall, new FloatType(tissueThreshold));

        HyperSphereShape disk = new HyperSphereShape(morphDiskRadius);
        smallMask = Dilation.dilate(smallMask, disk, 1);
        Filters.closeHoles(smallMask);
        smallMask = Erosion.erode(smallMask, disk, 1);

        // Remove low average intensity objects
        final double minAverageIntensity = tissuePercentile * labelMinPercentile;
        Img<UnsignedIntType> labels = (new ArrayImgFactory<>(new UnsignedIntType())).create(Filters.getDimensions(smallMask));
        ConnectedComponents.labelAllConnectedComponents(smallMask, labels, ConnectedComponents.StructuringElement.FOUR_CONNECTED);
        Map<Integer, Long> labelCounts = new HashMap<>();
        Map<Integer, Double> labelIntensitySums = new HashMap<>();

        {
            Cursor<UnsignedIntType> labelCursor = labels.cursor();
            RandomAccess<FloatType> imgSmallAccess = imgSmall.randomAccess();
            while(labelCursor.hasNext()) {
                labelCursor.fwd();
                int label = labelCursor.get().getInteger();
                if(label > 0) {
                    imgSmallAccess.setPosition(labelCursor);
                    labelCounts.put(label, labelCounts.getOrDefault(label, 0L) + 1);

                    double value = imgSmallAccess.get().get();
                    labelIntensitySums.put(label, labelIntensitySums.getOrDefault(label, 0.0) + value);
                }
            }
        }

        Set<Integer> invalidObjects = new HashSet<>();
        for(Integer label : labelCounts.keySet()) {
            long count = labelCounts.get(label);
            double intensitySum = labelIntensitySums.get(label);
            double avgIntensity = intensitySum / count;
            if(avgIntensity < minAverageIntensity)
                invalidObjects.add(label);
        }

        {
            Cursor<UnsignedIntType> labelCursor = labels.cursor();
            RandomAccess<UnsignedByteType> smallMaskAccess = smallMask.randomAccess();
            while(labelCursor.hasNext()) {
                labelCursor.fwd();
                int label = labelCursor.get().getInteger();
                if(invalidObjects.contains(label)) {
                    smallMaskAccess.setPosition(labelCursor);
                    smallMaskAccess.get().set(0);
                }
            }
        }

        // Scale back to original size
        Img<UnsignedByteType> mask = Filters.resize(smallMask, new NearestNeighborInterpolatorFactory<>(), Filters.getDimensions(img));
        Filters.threshold(mask, Filters.Otsu(mask));

        // Count for quantification
        getDataInterface().addTissuePixelCount(Filters.countNonZero(mask));

        getDataInterface().getTissueOutputData().setPlane(planeZIndex, mask);

    }
}

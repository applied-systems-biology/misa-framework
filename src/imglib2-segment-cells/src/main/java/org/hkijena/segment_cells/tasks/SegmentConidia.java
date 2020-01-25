package org.hkijena.segment_cells.tasks;

import net.imglib2.algorithm.gauss3.Gauss3;
import net.imglib2.img.Img;
import net.imglib2.img.display.imagej.ImageJFunctions;
import net.imglib2.type.logic.BitType;
import net.imglib2.type.numeric.integer.IntType;
import net.imglib2.type.numeric.integer.UnsignedShortType;
import net.imglib2.type.numeric.real.FloatType;
import net.imglib2.view.Views;
import org.hkijena.segment_cells.ExperimentDataInterface;
import org.hkijena.segment_cells.Filters;

public class SegmentConidia extends DAGTask {

    private ExperimentDataInterface experimentDataInterface;
    private double gaussSigma = 1.0;

    public SegmentConidia(Integer tid, ExperimentDataInterface experimentDataInterface) {
        super(tid, experimentDataInterface.getSampleDataInterface());
        this.experimentDataInterface = experimentDataInterface;
    }

    @Override
    public void work() {
        System.out.println("Running SegmentConidia on " + experimentDataInterface.toString());
        Img<UnsignedShortType> img_ = experimentDataInterface.getInputImage().getOrCreate();
        Img<FloatType> img = ImageJFunctions.convertFloat(ImageJFunctions.wrap(img_, "img"));
        Gauss3.gauss(gaussSigma, Views.extendZero(img.copy()), img);
        Filters.normalizeByMax(img);

        Img<BitType> thresholded = Filters.threshold(img, new FloatType(Filters.Otsu(Filters.convertFloatToUByte(img)).getInteger() / 255.0f));
        Filters.closeHoles(thresholded);

        Img<IntType> labels = Filters.distanceTransformWatershed(img, thresholded);

        experimentDataInterface.getOutputLabel().set(labels);
    }
}

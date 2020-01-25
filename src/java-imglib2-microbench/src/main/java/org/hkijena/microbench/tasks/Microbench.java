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

package org.hkijena.microbench.tasks;

import ij.ImagePlus;
import net.imagej.ImageJ;
import net.imglib2.FinalDimensions;
import net.imglib2.RandomAccessibleInterval;
import net.imglib2.algorithm.convolution.Convolution;
import net.imglib2.algorithm.fft2.FFT;
import net.imglib2.algorithm.fft2.FFTConvolution;
import net.imglib2.algorithm.neighborhood.HyperSphereShape;
import net.imglib2.algorithm.neighborhood.RectangleShape;
import net.imglib2.img.Img;
import net.imglib2.img.array.ArrayImgFactory;
import net.imglib2.img.display.imagej.ImageJFunctions;
import net.imglib2.type.logic.NativeBoolType;
import net.imglib2.type.numeric.complex.ComplexFloatType;
import net.imglib2.type.numeric.integer.UnsignedByteType;
import net.imglib2.type.numeric.real.FloatType;
import net.imglib2.view.Views;
import org.hkijena.microbench.CannyEdgeDetector;
import org.hkijena.microbench.DataInterface;
import org.hkijena.microbench.Filters;
import org.hkijena.microbench.Main;

import java.util.*;

public class Microbench extends DAGTask {

    public Microbench(Integer tid, DataInterface dataInterface) {
        super(tid, dataInterface);
    }

    @Override
    public void work() {
        System.out.println("Running Microbenchmark on " + getDataInterface().toString());

        final ImageJ ij = Main.IMAGEJ;
        List<Long> times = new ArrayList<>();
        times.add(System.currentTimeMillis());

        Img<FloatType> img = ImageJFunctions.convertFloat(ImageJFunctions.wrap(getDataInterface().getInputImage().getOrCreate(), "img"));
        times.add(System.currentTimeMillis());

        // Median filter
        Img<FloatType> img_median_filtered = img.factory().create(Filters.getDimensions(img));
        ij.op().filter().median(img_median_filtered, Views.interval(Views.extendZero(img), img), new RectangleShape(10, false));
        times.add(System.currentTimeMillis());

        // Morphology benchmark
        Img<FloatType> img_dilated = img.factory().create(Filters.getDimensions(img));
        ij.op().morphology().dilate(img_dilated, Views.interval(Views.extendZero(img), img), new HyperSphereShape(15));
        times.add(System.currentTimeMillis());

        // FFT-iFFT benchmark
        RandomAccessibleInterval<ComplexFloatType> img_fft = ij.op().filter().fft(img);
        Img<FloatType> img_ifft = img.factory().create(Filters.getDimensions(img));
        ij.op().filter().ifft(img_ifft, img_fft);
        times.add(System.currentTimeMillis());

        // Otsu thresholding
        Img<FloatType> img_otsu;
        {
            Img<UnsignedByteType> img_8u = Filters.convertFloatToUByte(img);
            UnsignedByteType threshold = Filters.Otsu(img_8u);
            Img<NativeBoolType> thresholded = Filters.threshold(img_8u, threshold);
            img_otsu = Filters.convertBooleanToFloat(thresholded);
        }
        times.add(System.currentTimeMillis());

        // Percentile thresholding
        Img<FloatType> img_percentile;
        {
            List<FloatType> percentiles = Filters.getPercentiles(Filters.getSortedPixels(img), Arrays.asList(65.0));
            Img<NativeBoolType> thresholded = Filters.threshold(img, percentiles.get(0));
            img_percentile = Filters.convertBooleanToFloat(thresholded);
        }
        times.add(System.currentTimeMillis());

        // Canny edge detector
        Img<FloatType> img_canny;
        {
            Img<UnsignedByteType> img_8u = Filters.convertFloatToUByte(img);
            CannyEdgeDetector cannyEdgeDetector = new CannyEdgeDetector();
            cannyEdgeDetector.setGaussianKernelRadius(1);
            cannyEdgeDetector.setLowThreshold(0.1f * 255f);
            cannyEdgeDetector.setHighThreshold(0.2f * 255f);
            cannyEdgeDetector.setContrastNormalized(false);
            ImagePlus imagePlus = ImageJFunctions.wrap(img_8u, "img");
            ImagePlus result = cannyEdgeDetector.process(imagePlus);
            img_canny = ImageJFunctions.convertFloat(result);
        }
        times.add(System.currentTimeMillis());

        // Wiener2
        Img<FloatType> img_wiener2 = Filters.wiener2(img, 3, -1);
        times.add(System.currentTimeMillis());

        // IO
        getDataInterface().getOutputMedian().set(img_median_filtered);
        getDataInterface().getOutputMorphology().set(img_dilated);
        getDataInterface().getOutputFFTIFFT().set(img_ifft);
        getDataInterface().getOutputOtsu().set(img_otsu);
        getDataInterface().getOutputPercentile().set(img_percentile);
        getDataInterface().getOutputCanny().set(img_canny);
        getDataInterface().getOutputWiener().set(img_wiener2);
        times.add(System.currentTimeMillis());

        // Save benchmark results
        Map<String, Double> runtimes = new HashMap<>();
        String[] timepoints = new String[] { null, "io", "median", "morphology", "fft-ifft", "otsu", "percentile", "canny", "wiener2", "io" };
        for(int i = 1; i < timepoints.length; ++i) {
            double seconds = (times.get(i) - times.get(i - 1)) / 1000.0;
            runtimes.put(timepoints[i], runtimes.getOrDefault(timepoints[i], 0.0) + seconds);
        }
        getDataInterface().getBenchmark().put(runtimes);
    }
}

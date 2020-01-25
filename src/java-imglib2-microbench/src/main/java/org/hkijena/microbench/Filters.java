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

package org.hkijena.microbench;

import net.imagej.ImageJ;
import net.imglib2.RandomAccess;
import net.imglib2.*;
import net.imglib2.algorithm.fft2.FFT;
import net.imglib2.algorithm.fft2.FFTMethods;
import net.imglib2.algorithm.labeling.ConnectedComponents;
import net.imglib2.algorithm.morphology.Dilation;
import net.imglib2.algorithm.morphology.distance.DistanceTransform;
import net.imglib2.algorithm.neighborhood.CenteredRectangleShape;
import net.imglib2.algorithm.neighborhood.Neighborhood;
import net.imglib2.algorithm.neighborhood.RectangleShape;
import net.imglib2.algorithm.neighborhood.Shape;
import net.imglib2.img.Img;
import net.imglib2.img.ImgFactory;
import net.imglib2.img.array.ArrayImg;
import net.imglib2.img.array.ArrayImgFactory;
import net.imglib2.interpolation.InterpolatorFactory;
import net.imglib2.loops.LoopBuilder;
import net.imglib2.realtransform.AffineGet;
import net.imglib2.realtransform.AffineRandomAccessible;
import net.imglib2.realtransform.RealViews;
import net.imglib2.realtransform.Scale;
import net.imglib2.roi.labeling.ImgLabeling;
import net.imglib2.roi.labeling.LabelingType;
import net.imglib2.type.NativeType;
import net.imglib2.type.Type;
import net.imglib2.type.logic.NativeBoolType;
import net.imglib2.type.numeric.NumericType;
import net.imglib2.type.numeric.RealType;
import net.imglib2.type.numeric.complex.ComplexFloatType;
import net.imglib2.type.numeric.integer.IntType;
import net.imglib2.type.numeric.integer.UnsignedByteType;
import net.imglib2.type.numeric.real.DoubleType;
import net.imglib2.type.numeric.real.FloatType;
import net.imglib2.util.Util;
import net.imglib2.view.ExtendedRandomAccessibleInterval;
import net.imglib2.view.IntervalView;
import net.imglib2.view.RandomAccessibleOnRealRandomAccessible;
import net.imglib2.view.Views;

import java.util.*;

public class Filters {

    public static <T extends Type<T>> void setTo(Img<T> target, T value) {
        Cursor<T> cursor = target.cursor();
        RandomAccess<T> access = target.randomAccess();
        while(cursor.hasNext()) {
            cursor.fwd();
            access.setPosition(cursor);
            access.get().set(value);
        }
    }

    public static <T extends RealType<T>> void median(Img<T> src, Img<T> target, int sz) {
        setTo(target, src.factory().type().createVariable());
        int border = sz / 2;
        long[] border_arr = new long[src.numDimensions()];
        Arrays.fill(border_arr, -border);

        RandomAccessibleInterval<T> src_ = Views.interval(src, Views.expandZero(src, border_arr));
        RandomAccessibleInterval<T> target_ = Views.interval(target, Views.expandZero(target, border_arr));

        final RectangleShape shape = new RectangleShape( border, true );
        RectangleShape.NeighborhoodsIterableInterval<T> neighborhoods = shape.neighborhoods(src_);

        Cursor<Neighborhood<T>> shape_cursor = neighborhoods.cursor();
        Cursor<T> src_center_cursor = Views.iterable(src_).cursor();
        Cursor<T> target_center_cursor = Views.iterable(target_).cursor();

        List<T> buffer = new ArrayList<>();

        while(shape_cursor.hasNext()) {
            shape_cursor.fwd();
            src_center_cursor.fwd();
            target_center_cursor.fwd();

            Cursor<T> ncursor = shape_cursor.get().cursor();

            buffer.clear();
            buffer.add(src_center_cursor.get());
            while(ncursor.hasNext()) {
                ncursor.fwd();
                buffer.add(ncursor.get().copy());
            }
            buffer.sort(Comparable::compareTo);

            target_center_cursor.get().set((T)buffer.get(buffer.size() / 2));
        }
    }

    public static <T extends RealType<T>> T getMax(Img<T> src) {
        Cursor<T> cursor = src.cursor();
        T max = null;
        while(cursor.hasNext()) {
            cursor.fwd();
            if(max == null || cursor.get().compareTo(max) > 0)
                max = cursor.get().copy();
        }
        return max;
    }

    public static <T extends RealType<T>> void normalizeByMax(Img<T> target) {
       normalizeByMax(target, 1);
    }

    public static <T extends RealType<T>> void normalizeByMax(Img<T> target, double mul) {
        T max_value = getMax(target);
        Cursor<T> cursor = target.cursor();
        while(cursor.hasNext()) {
            cursor.fwd();
            double v = cursor.get().getRealDouble();
            v = v * mul / max_value.getRealDouble();
            cursor.get().setReal(v);
        }
    }

    public static <T extends NativeType<T> & NumericType<T>> Img<T> rescale(Img<T> src, InterpolatorFactory<T, RandomAccessible<T>> interpolation, double... factors) {
        ExtendedRandomAccessibleInterval<T, RandomAccessibleInterval<T>> extended = Views.extendZero(src);
        RealRandomAccessible<T> field = Views.interpolate(extended, interpolation);
        Scale affine = new Scale(factors);
        AffineRandomAccessible<T, AffineGet> scaled = RealViews.affine(field, affine);
        RandomAccessibleOnRealRandomAccessible<T> raster = Views.raster(scaled);

        ImgFactory<T> factory = new ArrayImgFactory<>(src.factory().type());
        long[] dimensions = new long[src.numDimensions()];
        for(int i = 0; i < src.numDimensions(); ++i) {
            dimensions[i] = (long)(src.dimension(i) * factors[i]);
        }
        Img<T> result = factory.create(dimensions);
        copy(raster, result);
        return result;
    }

    public static <T extends NativeType<T> & NumericType<T>> Img<T> resize(Img<T> src, InterpolatorFactory<T, RandomAccessible<T>> interpolation, long... dimensions) {
        ExtendedRandomAccessibleInterval<T, RandomAccessibleInterval<T>> extended = Views.extendZero(src);
        RealRandomAccessible<T> field = Views.interpolate(extended, interpolation);

        double[] factors = new double[dimensions.length];
        for(int i = 0; i < dimensions.length; ++i) {
            factors[i] = dimensions[i] / (double)src.dimension(i);
        }

        Scale affine = new Scale(factors);
        AffineRandomAccessible<T, AffineGet> scaled = RealViews.affine(field, affine);
        RandomAccessibleOnRealRandomAccessible<T> raster = Views.raster(scaled);

        ImgFactory<T> factory = new ArrayImgFactory<>(src.factory().type());
        Img<T> result = factory.create(dimensions);
        copy(raster, result);
        return result;
    }

    /**
     * Copy from a source that is just RandomAccessible to an IterableInterval. Latter one defines
     * size and location of the copy operation. It will query the same pixel locations of the
     * IterableInterval in the RandomAccessible. It is up to the developer to ensure that these
     * coordinates match.
     *
     * Note that both, input and output could be Views, Img or anything that implements
     * those interfaces.
     *
     * @param source - a RandomAccess as source that can be infinite
     * @param target - an IterableInterval as target
     */
    public static < T extends Type< T >> void copy(final RandomAccessible< T > source,
                                                   final IterableInterval< T > target )
    {
        // create a cursor that automatically localizes itself on every move
        Cursor< T > targetCursor = target.localizingCursor();
        RandomAccess< T > sourceRandomAccess = source.randomAccess();

        // iterate over the input cursor
        while ( targetCursor.hasNext())
        {
            // move input cursor forward
            targetCursor.fwd();

            // set the output cursor to the position of the input cursor
            sourceRandomAccess.setPosition( targetCursor );

            // set the value of this pixel of the output image, every Type supports T.set( T type )
            targetCursor.get().set( sourceRandomAccess.get() );
        }
    }

    public static < T extends Type< T >> void copy2(final IntervalView< T > source,
                                                   final IterableInterval< T > target )
    {
        // create a cursor that automatically localizes itself on every move
        Cursor< T > targetCursor = target.localizingCursor();
        RandomAccess< T > sourceRandomAccess = source.randomAccess();

        long[] diff = new long[source.numDimensions()];
        for(int i = 0; i < target.numDimensions(); ++i) {
            diff[i] = target.min(i) - source.min(i);
        }

        long[] loc = new long[source.numDimensions()];

        // iterate over the input cursor
        while (targetCursor.hasNext())
        {
            targetCursor.fwd();
            targetCursor.localize(loc);

            for(int i = 0; i < source.numDimensions(); ++i) {
                loc[i] -= diff[i];
            }
            sourceRandomAccess.setPosition(loc);

            // set the value of this pixel of the output image, every Type supports T.set( T type )
            targetCursor.get().set( sourceRandomAccess.get() );
        }
    }

    public static <T extends Type<T>> void copyInterval(final RandomAccessibleInterval<T> source, final RandomAccessibleInterval<T> target) {
        LoopBuilder.setImages(source, target).forEachPixel(Type::set);
    }

    public static <T extends RealType<T>> List<T> getSortedPixels(Img<T> src) {
        List<T> pixels = new ArrayList<>((int)src.size());

        Cursor<T> cursor = src.cursor();
        while(cursor.hasNext()) {
            cursor.fwd();
            pixels.add(cursor.get().copy());
        }

        pixels.sort(Comparable::compareTo);

        return pixels;
    }

    public static <T extends RealType<T>> List<T> getSortedPixelsWhere(Img<T> src, Img<UnsignedByteType> mask) {
        List<T> pixels = new ArrayList<>((int)src.size());

        Cursor<T> cursor = src.cursor();
        RandomAccess<UnsignedByteType> maskAccess = mask.randomAccess();
        while(cursor.hasNext()) {
            cursor.fwd();
            maskAccess.setPosition(cursor);
            if(maskAccess.get().getInteger() > 0)
                pixels.add(cursor.get().copy());
        }

        pixels.sort(Comparable::compareTo);

        return pixels;
    }

    public static <T extends RealType<T>> List<T> getPercentiles(List<T> sortedPixels, List<Double> percentiles) {
        List<T> result = new ArrayList<>();

        for(double percentile : percentiles) {
            double rank = percentile / 100.0 * (sortedPixels.size() - 1);
            int lower_rank = (int)Math.floor(rank);
            int higher_rank = (int)Math.ceil(rank);
            double frac = rank - lower_rank; // fractional section

            // p = lower_rank + (higher_rank - lower_rank) * frac
            T p = sortedPixels.get(lower_rank).copy();
            T p0 = sortedPixels.get(higher_rank).copy();
            p0.sub(sortedPixels.get(lower_rank));
            p0.mul(frac);
            p.add(p0);
            result.add(p);
        }

        return result;
    }

    public static <T> long[] getDimensions(RandomAccessibleInterval<T> src) {
        long[] result = new long[src.numDimensions()];
        src.dimensions(result);
        return result;
    }

    public static <T extends Comparable<T>> Img<NativeBoolType> threshold(Img<T> src, T threshold) {
        Img<NativeBoolType> result = (new ArrayImgFactory<>(new NativeBoolType()).create(getDimensions(src)));

        Cursor<T> srcCursor = src.cursor();
        Cursor<NativeBoolType> targetCursor = result.cursor();

        while(srcCursor.hasNext()) {
            srcCursor.fwd();
            targetCursor.fwd();
            if(srcCursor.get().compareTo(threshold) > 0) {
                targetCursor.get().set(new NativeBoolType(true));
            }
        }

        return result;
    }

    public static Img<NativeBoolType> invertBoolean(Img<NativeBoolType> src) {
        Img<NativeBoolType> result = (new ArrayImgFactory<>(new NativeBoolType()).create(getDimensions(src)));

        Cursor<NativeBoolType> srcCursor = src.cursor();
        Cursor<NativeBoolType> targetCursor = result.cursor();

        while(srcCursor.hasNext()) {
            srcCursor.fwd();
            targetCursor.fwd();
            targetCursor.get().set(!srcCursor.get().get());
        }

        return result;
    }

    public static UnsignedByteType Otsu(IterableInterval<UnsignedByteType> pixels) {
        Map<Integer, Long> histogram = new HashMap<>();
        Cursor<UnsignedByteType> cursor = pixels.cursor();

        long histogramSum = 0;
        while(cursor.hasNext()) {
            cursor.fwd();
            ++histogramSum;
            histogram.put(cursor.get().getInteger(), histogram.getOrDefault(cursor.get().getInteger(), 0L) + 1);
        }

        // i cumulative p-histogram for the means
        double cumulativeipSum = 0;
        for(int t = 0; t <= 255; ++t) {
            cumulativeipSum += t * 1.0 * histogram.getOrDefault(t, 0L);
        }

        cumulativeipSum *= 1.0 / histogramSum;

        // Initial values for t < 0
        int t_best = 0;
        double var_best = 0;
        double w0 = 0;
        double mu0 = 0;

        for (int t = 0; t <= 255; ++t) {

            // This cannot change the threshold
            if (histogram.getOrDefault(t, 0L) == 0)
                continue;

            double p_i = 1.0 * histogram.get(t) / histogramSum;
            mu0 *= w0;
            w0 += p_i;
            double w1 = 1.0 - w0;

            mu0 = (mu0 + t * p_i) / w0;
            double mu1 = (cumulativeipSum - w0 * mu0) / w1;
            double var = w0 * w1 * Math.pow(mu0 - mu1, 2.0);

            if (var > var_best) {
                var_best = var;
                t_best = t;
            }
        }

        return new UnsignedByteType(t_best);
    }

    public static void closeHoles(Img<UnsignedByteType> mask) {
        Stack<long[]> borderLocations = new Stack<>();

        RandomAccess<UnsignedByteType> access = Views.extendValue(mask, new UnsignedByteType(255)).randomAccess();
        Img<UnsignedByteType> buffer = mask.factory().create(getDimensions(mask));
        RandomAccess<UnsignedByteType> buffer_access = Views.extendValue(buffer, new UnsignedByteType(255)).randomAccess();

        {
            long cols = mask.dimension(0);
            long rows = mask.dimension(1);
            long[] pos = new long[2];
            for(long row = 0; row < rows; ++row) {
                pos[1] = row;
                if(row == 0 || row == rows - 1) {
                    for(long col = 0; col < cols; ++col) {
                        pos[0] = col;
                        access.setPosition(pos);
                        if(access.get().getInteger() == 0) {
                            borderLocations.push(pos.clone());

                            buffer_access.setPosition(pos);
                            buffer_access.get().set(new UnsignedByteType(255));
                        }
                    }
                }
                else {
                    pos[0] = 0;
                    access.setPosition(pos);
                    if(access.get().getInteger() == 0) {
                        borderLocations.push(pos.clone());

                        buffer_access.setPosition(pos);
                        buffer_access.get().set(new UnsignedByteType(255));
                    }

                    pos[0] = cols - 1;
                    access.setPosition(pos);
                    if(access.get().getInteger() == 0) {
                        borderLocations.push(pos.clone());

                        buffer_access.setPosition(pos);
                        buffer_access.get().set(new UnsignedByteType(255));
                    }
                }
            }
        }

        while(!borderLocations.empty()) {
            long[] pos2 = borderLocations.pop();
            long[] pos3 = new long[pos2.length];

            for(int dx = -1; dx < 2; ++dx) {
                for(int dy = -1; dy < 2; ++dy) {
                    if(dx != 0 || dy != 0) {
                        pos3[0] = pos2[0] + dx;
                        pos3[1] = pos2[1] + dy;

                        access.setPosition(pos3);
                        buffer_access.setPosition(pos3);

                        if(access.get().getInteger() == 0 && buffer_access.get().getInteger() == 0) {
                            buffer_access.get().set(new UnsignedByteType(255));
                            borderLocations.push(pos3.clone());
                        }
                    }
                }
            }
        }

        {
            Cursor<UnsignedByteType> targetCursor = mask.cursor();
            Cursor<UnsignedByteType> bufferCursor = buffer.cursor();

            while(targetCursor.hasNext()) {
                targetCursor.fwd();
                bufferCursor.fwd();

                if(bufferCursor.get().getInteger() > 0) {
                    targetCursor.get().set(new UnsignedByteType(0));
                }
                else {
                    targetCursor.get().set(new UnsignedByteType(255));
                }
            }
        }
    }

    public static void erodeImageBorders(Img<NativeBoolType> mask) {

        RandomAccess<NativeBoolType> access = mask.randomAccess();

        for(int k = 0; k < 2; ++k) {
            long cols = mask.dimension(0);
            long rows = mask.dimension(1);
            long[] pos = new long[2];
            for (long row = k; row < rows - k; ++row) {
                pos[1] = row;
                if (row == k || row == rows - k - 1) {
                    for (long col = k; col < cols - k; ++col) {
                        pos[0] = col;
                        access.setPosition(pos);
                        access.get().set(false);
                    }
                } else {
                    pos[0] = 0;
                    access.setPosition(pos);
                    access.get().set(false);

                    pos[0] = cols - k - 1;
                    access.setPosition(pos);
                    access.get().set(false);
                }
            }
        }
    }

    public static <T extends RealType<T>> long countNonZero(IterableInterval<T> img) {
        long count = 0;
        Cursor<T> cursor = img.cursor();
        while(cursor.hasNext()) {
            cursor.fwd();
            if(cursor.get().getRealDouble() > 0) {
                ++count;
            }
        }

        return count;
    }

    public static Img<UnsignedByteType> convertFloatToUByte(Img<FloatType> src) {
        Img<UnsignedByteType> target = (new ArrayImgFactory<>(new UnsignedByteType())).create(getDimensions(src));
        RandomAccess<UnsignedByteType> targetAccess = target.randomAccess();
        Cursor<FloatType> cursor = src.cursor();

        float imax = 0;

        while(cursor.hasNext()) {
            cursor.fwd();
            imax = Math.max(imax, cursor.get().get());
        }

        cursor = src.cursor();
        while(cursor.hasNext()) {
            cursor.fwd();
            targetAccess.setPosition(cursor);
            targetAccess.get().set((int)(cursor.get().get() / imax * 255));
        }

        return target;
    }

    public static Img<FloatType> convertBooleanToFloat(Img<NativeBoolType> src) {
        Img<FloatType> target = (new ArrayImgFactory<>(new FloatType())).create(getDimensions(src));
        RandomAccess<FloatType> targetAccess = target.randomAccess();
        Cursor<NativeBoolType> cursor = src.cursor();

        while(cursor.hasNext()) {
            cursor.fwd();
            targetAccess.setPosition(cursor);
            targetAccess.get().set(cursor.get().get() ? 1.0f : 0.0f);
        }

        return target;
    }

    public static <T extends RealType<T>> Img<NativeBoolType> localMaxima(Img<T> source, Shape strel, Img<NativeBoolType> mask) {
        Img<T> dilated = source.copy();
        dilated = Dilation.dilate(dilated, strel, 1);

        Img<NativeBoolType> result = (new ArrayImgFactory<>(new NativeBoolType())).create(getDimensions(source));
        Cursor<T> cursor = source.localizingCursor();
        RandomAccess<T> dilatedAccess = dilated.randomAccess();
        RandomAccess<NativeBoolType> resultAccess = result.randomAccess();
        RandomAccess<NativeBoolType> maskAccess = mask.randomAccess();
        while(cursor.hasNext()) {
            cursor.fwd();
            dilatedAccess.setPosition(cursor);
            resultAccess.setPosition(cursor);
            maskAccess.setPosition(cursor);
            if(cursor.get().valueEquals(dilatedAccess.get()) && maskAccess.get().get()) {
                resultAccess.get().set(true);
            }
        }
        return result;
    }

    public static <T extends RealType<T>> Img<IntType> distanceTransformWatershed(Img<T> img, Img<NativeBoolType> thresholded) {
        Img<NativeBoolType> thresholdedInv = invertBoolean(thresholded);
        Img<DoubleType> distance = (new ArrayImgFactory<>(new DoubleType())).create(Filters.getDimensions(thresholdedInv));
        DistanceTransform.binaryTransform(thresholdedInv, distance, DistanceTransform.DISTANCE_TYPE.EUCLIDIAN);

        Img<NativeBoolType> localMaxi = Filters.localMaxima(distance, new CenteredRectangleShape(new int[] {2, 2}, false), thresholded);
        ImgLabeling<Integer, IntType> localMaxiLabeling = Main.IMAGEJ.op().labeling().cca(localMaxi, ConnectedComponents.StructuringElement.FOUR_CONNECTED);

        // Info: Affected by https://github.com/imagej/imagej-ops/issues/579
        // We'll get at least one additional component
        ImgLabeling<Integer, IntType> watershedResult = Main.IMAGEJ.op().image().watershed(thresholded, localMaxiLabeling, false, false);

        Img<IntType> result = (new ArrayImgFactory<>(new IntType())).create(getDimensions(img));

        {
            Cursor<LabelingType<Integer>> cursor = watershedResult.localizingCursor();
            RandomAccess<IntType> target = result.randomAccess();
            while(cursor.hasNext()) {
                cursor.fwd();
                target.setPosition(cursor);
                if(!cursor.get().isEmpty())
                    target.get().set(cursor.get().iterator().next());
                else
                    target.get().set(0);
            }
        }

        return result;
    }

    public static <T extends RealType<T>> IntervalView<T> fftpad(Img<T> img, long[] fftDims, boolean shift) {
        long[] ap = new long[img.numDimensions()];
        for(int i = 0; i < img.numDimensions(); ++i) {
            if(img.dimension(i) % 2 == 0)
                ap[i] = 1;
        }
        long[] c = new long[img.numDimensions()];
        for(int i = 0; i < img.numDimensions(); ++i) {
            c[i] = (fftDims[i] - img.dimension(i) - ap[i]) / 2;
        }
        long[] min = new long[img.numDimensions()];
        long[] max = new long[img.numDimensions()];
        for(int i = 0; i < img.numDimensions(); ++i) {
            min[i] = -c[i];
            max[i] = img.dimension(i) + c[i] - 1 + ap[i];
        }

        IntervalView<T> result = Views.interval(Views.extendZero(img), min, max);

        if(shift) {
            long[] smin = new long[img.numDimensions()];
            long[] smax = new long[img.numDimensions()];
            for(int i = 0; i < img.numDimensions(); ++i) {
                smin[i] = -(result.dimension(i) / 2 - 1);
                smax[i] = result.dimension(i) + smin[i] - 1;
            }
            IntervalView<T> sresult = Views.interval(Views.extendPeriodic(result), smin, smax);
            return sresult;
        }
        else {
            return result;
        }
    }


    public static long[] getPaddedDimensions(Img<?> img, long[] fftDims) {
        long[] ap = new long[img.numDimensions()];
        for(int i = 0; i < img.numDimensions(); ++i) {
            if(img.dimension(i) % 2 == 0)
                ap[i] = 1;
        }
        long[] c = new long[img.numDimensions()];
        for(int i = 0; i < img.numDimensions(); ++i) {
            c[i] = (fftDims[i] - img.dimension(i) - ap[i]) / 2;
        }
        long[] o = new long[img.numDimensions()];
        for(int i = 0; i < img.numDimensions(); ++i) {
            o[i] = 2 * c[i] + img.dimension(i) + ap[i] - 1;
        }
        return o;
    }

    public static Img<ComplexFloatType> fft(Img<FloatType> img, long[] fftDims, boolean shift) {
        final ImageJ ij = Main.IMAGEJ;
        RandomAccessibleInterval<FloatType> input = fftpad(img, fftDims, shift);
        RandomAccessibleInterval<ComplexFloatType> result_ = ij.op().filter().fft(input);
        Img<ComplexFloatType> result = (new ArrayImgFactory<>(new ComplexFloatType())).create(getDimensions(result_));
        copy(result_, result);
        return result;
    }

    public static Img<FloatType> unshift(Img<FloatType> img) {
        long[] min = new long[img.numDimensions()];
        long[] max = new long[img.numDimensions()];
        for(int i = 0; i < img.numDimensions(); ++i) {
            min[i] = +(img.dimension(i) / 2 - 1);
            max[i] = min[i] + img.dimension(i);
        }
        IntervalView<FloatType> interval = Views.interval(Views.extendPeriodic(img), min, max);
        Img<FloatType> target = (new ArrayImgFactory<>(new FloatType())).create(getDimensions(interval));
        copy2(interval, target);
        return target;
    }

    public static Img<FloatType> cropCentered(Img<FloatType> img, long[] targetSize) {
        long[] min = new long[img.numDimensions()];
        long[] max = new long[img.numDimensions()];
        for(int i = 0; i < img.numDimensions(); ++i) {
            min[i] = img.dimension(i) / 2 - targetSize[i] / 2;
            max[i] = min[i] + img.dimension(i);
        }

        Img<FloatType> result = (new ArrayImgFactory<>(new FloatType())).create(targetSize);
        IntervalView<FloatType> interval = Views.interval(img, min, max);
        copy2(interval, result);
        return result;
    }

    public static void clamp(Img<FloatType> img) {
        Cursor<FloatType> cursor = img.cursor();
        while(cursor.hasNext()) {
            cursor.fwd();
            cursor.get().set(Math.max(0, Math.min(1, cursor.get().get())));
        }
    }

    public static Img<FloatType> blur(Img<FloatType> img, int kernelSize) {
        Img<FloatType> kernel = img.factory().create(kernelSize, kernelSize);
        setTo(kernel, new FloatType(1.0f / (kernelSize * kernelSize)));

        final ImageJ ij = Main.IMAGEJ;
        RandomAccessibleInterval<FloatType> res = ij.op().filter().convolve(Views.interval(Views.extendZero(img), img), kernel);
        Img<FloatType> result = img.factory().create(getDimensions(img));
        copy(res, result);
        return result;
    }

    public static Img<FloatType> squared(Img<FloatType> img) {
        Img<FloatType> result = img.copy();
        Cursor<FloatType> cursor = result.cursor();
        while(cursor.hasNext()) {
            cursor.fwd();
            cursor.get().set(cursor.get().get() * cursor.get().get());
        }
        return result;
    }

    public static float mean(Img<FloatType> img) {
        Cursor<FloatType> cursor = img.cursor();
        float sum = 0;
        while(cursor.hasNext()) {
            cursor.fwd();
            sum += cursor.get().get();
        }
        return sum / (img.dimension(0) * img.dimension(1));
    }

    public static Img<FloatType> calculateVariance(Img<FloatType> imgMean, Img<FloatType> img2Mean) {
        Img<FloatType> result = imgMean.factory().create(getDimensions(imgMean));
        Cursor<FloatType> cursor = result.localizingCursor();
        RandomAccess<FloatType> meanAccess = imgMean.randomAccess();
        RandomAccess<FloatType> sqMeanAccess = img2Mean.randomAccess();
        while(cursor.hasNext()) {
            cursor.fwd();
            meanAccess.setPosition(cursor);
            sqMeanAccess.setPosition(cursor);
            cursor.get().set(sqMeanAccess.get().get() - (meanAccess.get().get() * meanAccess.get().get()) + (float)1e-06);
        }
        return result;
    }

    /**
     * Implementation of Matlab's wiener2 deconvolution
     * @param img
     * @return
     */
    public static Img<FloatType> wiener2(Img<FloatType> img, int neighborship, float noiseVariance) {
        final ImageJ ij = Main.IMAGEJ;
        Img<FloatType> img_mean = blur(img, neighborship);
        Img<FloatType> img2_mean = blur(squared(img), neighborship);
        Img<FloatType> variance = calculateVariance(img_mean, img2_mean);
        if(noiseVariance <= 0) {
            noiseVariance = mean(variance);
        }

        Img<FloatType> result = img.factory().create(getDimensions(img));
        Cursor<FloatType> resultCursor = result.localizingCursor();
        RandomAccess<FloatType> inputAccess = img.randomAccess();
        RandomAccess<FloatType> meanAccess = img_mean.randomAccess();
        RandomAccess<FloatType> varAccess = variance.randomAccess();
        while(resultCursor.hasNext()) {
            resultCursor.fwd();
            inputAccess.setPosition(resultCursor);
            meanAccess.setPosition(resultCursor);
            varAccess.setPosition(resultCursor);

            float x = inputAccess.get().get();
            float mu = meanAccess.get().get();
            float sigma2 = varAccess.get().get();

            resultCursor.get().set(mu + ((sigma2 - noiseVariance) / sigma2) * (x - mu));
        }
        return result;
    }
}

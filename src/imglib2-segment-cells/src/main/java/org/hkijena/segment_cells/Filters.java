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

package org.hkijena.segment_cells;

import net.imagej.ImageJ;
import net.imglib2.RandomAccess;
import net.imglib2.*;
import net.imglib2.algorithm.labeling.ConnectedComponents;
import net.imglib2.algorithm.morphology.Dilation;
import net.imglib2.algorithm.morphology.distance.DistanceTransform;
import net.imglib2.algorithm.neighborhood.CenteredRectangleShape;
import net.imglib2.algorithm.neighborhood.Neighborhood;
import net.imglib2.algorithm.neighborhood.RectangleShape;
import net.imglib2.algorithm.neighborhood.Shape;
import net.imglib2.img.Img;
import net.imglib2.img.ImgFactory;
import net.imglib2.img.array.ArrayImgFactory;
import net.imglib2.interpolation.InterpolatorFactory;
import net.imglib2.realtransform.AffineGet;
import net.imglib2.realtransform.AffineRandomAccessible;
import net.imglib2.realtransform.RealViews;
import net.imglib2.realtransform.Scale;
import net.imglib2.roi.labeling.ImgLabeling;
import net.imglib2.roi.labeling.LabelingType;
import net.imglib2.type.BooleanType;
import net.imglib2.type.NativeType;
import net.imglib2.type.Type;
import net.imglib2.type.logic.BitType;
import net.imglib2.type.numeric.NumericType;
import net.imglib2.type.numeric.RealType;
import net.imglib2.type.numeric.integer.IntType;
import net.imglib2.type.numeric.integer.UnsignedByteType;
import net.imglib2.type.numeric.real.DoubleType;
import net.imglib2.type.numeric.real.FloatType;
import net.imglib2.view.ExtendedRandomAccessibleInterval;
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

    public static <T> long[] getDimensions(Img<T> src) {
        long[] result = new long[src.numDimensions()];
        src.dimensions(result);
        return result;
    }

    public static <T extends Comparable<T>> Img<BitType> threshold(Img<T> src, T threshold) {
        Img<BitType> result = (new ArrayImgFactory<>(new BitType()).create(getDimensions(src)));

        Cursor<T> srcCursor = src.cursor();
        Cursor<BitType> targetCursor = result.cursor();

        while(srcCursor.hasNext()) {
            srcCursor.fwd();
            targetCursor.fwd();
            if(srcCursor.get().compareTo(threshold) > 0) {
                targetCursor.get().set(new BitType(true));
            }
        }

        return result;
    }

    public static Img<BitType> invertBoolean(Img<BitType> src) {
        Img<BitType> result = (new ArrayImgFactory<>(new BitType()).create(getDimensions(src)));

        Cursor<BitType> srcCursor = src.cursor();
        Cursor<BitType> targetCursor = result.cursor();

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

//    public static void closeHoles(Img<UnsignedByteType> mask) {
//        Stack<long[]> borderLocations = new Stack<>();
//
//        RandomAccess<UnsignedByteType> access = Views.extendValue(mask, new UnsignedByteType(255)).randomAccess();
//        Img<UnsignedByteType> buffer = mask.factory().create(getDimensions(mask));
//        RandomAccess<UnsignedByteType> buffer_access = Views.extendValue(buffer, new UnsignedByteType(255)).randomAccess();
//
//        {
//            long cols = mask.dimension(0);
//            long rows = mask.dimension(1);
//            long[] pos = new long[2];
//            for(long row = 0; row < rows; ++row) {
//                pos[1] = row;
//                if(row == 0 || row == rows - 1) {
//                    for(long col = 0; col < cols; ++col) {
//                        pos[0] = col;
//                        access.setPosition(pos);
//                        if(access.get().getInteger() == 0) {
//                            borderLocations.push(pos.clone());
//
//                            buffer_access.setPosition(pos);
//                            buffer_access.get().set(new UnsignedByteType(255));
//                        }
//                    }
//                }
//                else {
//                    pos[0] = 0;
//                    access.setPosition(pos);
//                    if(access.get().getInteger() == 0) {
//                        borderLocations.push(pos.clone());
//
//                        buffer_access.setPosition(pos);
//                        buffer_access.get().set(new UnsignedByteType(255));
//                    }
//
//                    pos[0] = cols - 1;
//                    access.setPosition(pos);
//                    if(access.get().getInteger() == 0) {
//                        borderLocations.push(pos.clone());
//
//                        buffer_access.setPosition(pos);
//                        buffer_access.get().set(new UnsignedByteType(255));
//                    }
//                }
//            }
//        }
//
//        while(!borderLocations.empty()) {
//            long[] pos2 = borderLocations.pop();
//            long[] pos3 = new long[pos2.length];
//
//            for(int dx = -1; dx < 2; ++dx) {
//                for(int dy = -1; dy < 2; ++dy) {
//                    if(dx != 0 || dy != 0) {
//                        pos3[0] = pos2[0] + dx;
//                        pos3[1] = pos2[1] + dy;
//
//                        access.setPosition(pos3);
//                        buffer_access.setPosition(pos3);
//
//                        if(access.get().getInteger() == 0 && buffer_access.get().getInteger() == 0) {
//                            buffer_access.get().set(new UnsignedByteType(255));
//                            borderLocations.push(pos3.clone());
//                        }
//                    }
//                }
//            }
//        }
//
//        {
//            Cursor<UnsignedByteType> targetCursor = mask.cursor();
//            Cursor<UnsignedByteType> bufferCursor = buffer.cursor();
//
//            while(targetCursor.hasNext()) {
//                targetCursor.fwd();
//                bufferCursor.fwd();
//
//                if(bufferCursor.get().getInteger() > 0) {
//                    targetCursor.get().set(new UnsignedByteType(0));
//                }
//                else {
//                    targetCursor.get().set(new UnsignedByteType(255));
//                }
//            }
//        }
//    }

    public static void erodeImageBorders(Img<BitType> mask) {

        RandomAccess<BitType> access = mask.randomAccess();

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

        while(cursor.hasNext()) {
            cursor.fwd();
            targetAccess.setPosition(cursor);
            targetAccess.get().set((int)(cursor.get().get() * 255));
        }

        return target;
    }

    public static<T extends BooleanType<T>> Img<UnsignedByteType> convertBoolToUByte(Img<T> src) {
        Img<UnsignedByteType> target = (new ArrayImgFactory<>(new UnsignedByteType())).create(getDimensions(src));
        RandomAccess<UnsignedByteType> targetAccess = target.randomAccess();
        Cursor<T> cursor = src.cursor();

        while(cursor.hasNext()) {
            cursor.fwd();
            targetAccess.setPosition(cursor);
            targetAccess.get().set((int)(cursor.get().get() ? 255 : 0));
        }

        return target;
    }

    public static <T extends RealType<T>> Img<BitType> localMaxima(Img<T> source, Shape strel, Img<BitType> mask) {
        Img<T> dilated = source.copy();
        dilated = Dilation.dilate(dilated, strel, 1);

        Img<BitType> result = (new ArrayImgFactory<>(new BitType())).create(getDimensions(source));
        Cursor<T> cursor = source.localizingCursor();
        RandomAccess<T> dilatedAccess = dilated.randomAccess();
        RandomAccess<BitType> resultAccess = result.randomAccess();
        RandomAccess<BitType> maskAccess = mask.randomAccess();
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

    public static void toNegative(Img<DoubleType> img) {
        Cursor<DoubleType> cursor = img.cursor();
        while(cursor.hasNext()) {
            cursor.fwd();
            cursor.get().mul(-1);
        }
    }


    public static void closeHoles(Img<BitType> mask) {
        final ImageJ ij = Main.IMAGEJ;
        ij.op().morphology().fillHoles(mask, mask.copy());
    }

    public static <T extends RealType<T>> Img<IntType> distanceTransformWatershed(Img<T> img, Img<BitType> thresholded) {
        Img<BitType> thresholdedInv = invertBoolean(thresholded);
        Img<DoubleType> distance = (new ArrayImgFactory<>(new DoubleType())).create(Filters.getDimensions(thresholdedInv));
        DistanceTransform.binaryTransform(thresholdedInv, distance, DistanceTransform.DISTANCE_TYPE.EUCLIDIAN);
//        RandomAccessibleInterval<DoubleType> distance_ = Main.IMAGEJ.op().image().distancetransform(thresholdedInv);
//        Img<DoubleType> distance = (new ArrayImgFactory<>(new DoubleType())).create(Filters.getDimensions(thresholded));
//        copy(distance_, distance);

        Img<BitType> localMaxi = Filters.localMaxima(distance, new CenteredRectangleShape(new int[] {2, 2}, false), thresholded);
        ImgLabeling<Integer, IntType> localMaxiLabeling = Main.IMAGEJ.op().labeling().cca(localMaxi, ConnectedComponents.StructuringElement.FOUR_CONNECTED);

        // Info: Affected by https://github.com/imagej/imagej-ops/issues/579
        // We have to modify the mask via erosion to prevent crashing
        Img<BitType> mask = thresholded.copy();
        Dilation.dilate(Views.extendValue(thresholded, new BitType(true)), mask, new CenteredRectangleShape(new int[] { 1, 1 }, false), 1);

        Img<IntType> watershedResultBackend = (new ArrayImgFactory<>(new IntType())).create(getDimensions(img));
        ImgLabeling<Integer, IntType> watershedResult = new ImgLabeling<>(watershedResultBackend);

        toNegative(distance);
        Main.IMAGEJ.op().image().watershed(watershedResult, distance, localMaxiLabeling, false, false, mask);

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
}

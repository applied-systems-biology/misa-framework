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

package org.hkijena.deconvolve_rif.tasks;

import net.imagej.ImageJ;
import net.imglib2.Cursor;
import net.imglib2.RandomAccess;
import net.imglib2.RandomAccessibleInterval;
import net.imglib2.algorithm.fft2.FFT;
import net.imglib2.img.Img;
import net.imglib2.img.array.ArrayImgFactory;
import net.imglib2.img.display.imagej.ImageJFunctions;
import net.imglib2.interpolation.randomaccess.NLinearInterpolatorFactory;
import net.imglib2.type.numeric.complex.ComplexFloatType;
import net.imglib2.type.numeric.real.FloatType;
import org.hkijena.deconvolve_rif.DataInterface;
import org.hkijena.deconvolve_rif.Filters;
import org.hkijena.deconvolve_rif.Main;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Deconvolve extends DAGTask {

    private float rifLambda = 0.001f;
    private ExecutorService service = Executors.newFixedThreadPool(1);

    public Deconvolve(Integer tid, DataInterface dataInterface) {
        super(tid, dataInterface);
    }

    private Img<ComplexFloatType> getLaplacianFFT(long[] fftDims) {
        Img<FloatType> kernel = (new ArrayImgFactory<>(new FloatType())).create(3,3);
        {
            Filters.setTo(kernel, new FloatType(1.0f / 8));
            RandomAccess<FloatType> access = kernel.randomAccess();
            access.setPosition(new long[]{ 1, 1 });
            access.get().set(-1);
        }

        return Filters.fft(kernel, fftDims, true);
    }

    private long[] getFFTDimensions(Img<FloatType> img, Img<FloatType> psf) {
        long[] result = new long[img.numDimensions()];
        for(int i = 0; i < img.numDimensions(); ++i) {
            result[i] = img.dimension(i) + psf.dimension(i) - 1;
        }
        return result;
    }

    @Override
    public void work() {
        final ImageJ ij = Main.IMAGEJ;
        System.out.println("Running Deconvolve on " + getDataInterface().toString());

        Img<FloatType> img = getDataInterface().getConvolvedImage().getOrCreate();
        Img<FloatType> psf = getDataInterface().getPsfImage().getOrCreate();

        // Transform into Fourier space
        long[] fftDims = getFFTDimensions(img, psf);
        Img<ComplexFloatType> imgFFT = Filters.fft(img, fftDims, false);
        Img<ComplexFloatType> psfFFT = Filters.fft(psf, fftDims, true);

        // Apply RIF
        // Adapted from DeconvolutionLab2 code
        // See https://github.com/Biomedical-Imaging-Group/DeconvolutionLab2/blob/master/src/main/java/deconvolution/algorithm/RegularizedInverseFilter.java
        Img<ComplexFloatType> Y = imgFFT;
        Img<ComplexFloatType> H = psfFFT;
        Img<ComplexFloatType> L = getLaplacianFFT(fftDims);
        Img<ComplexFloatType> X = imgFFT.factory().create(Filters.getDimensions(Y));

        // Apply calculations
        {
            Cursor<ComplexFloatType> cY = Y.localizingCursor();
            RandomAccess<ComplexFloatType> cH = H.randomAccess();
            RandomAccess<ComplexFloatType> cL = L.randomAccess();
            RandomAccess<ComplexFloatType> cX = X.randomAccess();

            // Buffer variables
            ComplexFloatType H2 = new ComplexFloatType();
            ComplexFloatType L2 = new ComplexFloatType();
            ComplexFloatType FA = new ComplexFloatType();

            while(cY.hasNext()) {
                cY.fwd();
                cH.setPosition(cY);
                cL.setPosition(cY);
                cX.setPosition(cY);

                // H2 = H * H
                H2.setReal(cH.get().getRealDouble());
                H2.setImaginary(cH.get().getImaginaryDouble());
                H2.mul(cH.get());

                // L2 = L * lambda * L
                L2.setReal(cL.get().getRealDouble());
                L2.setImaginary(cL.get().getImaginaryDouble());
                L2.mul(rifLambda);
                L2.mul(cL.get());

                // FA = H2 + L2
                FA.setReal(H2.getRealDouble());
                FA.setImaginary(H2.getImaginaryDouble());
                FA.add(L2);

                // X = Y * H / FA
                cX.get().set(cY.get().copy());
                cX.get().mul(cH.get());
                cX.get().div(FA);
            }
        }

        // Inverse FFT
        long[] ifftDims = Filters.getPaddedDimensions(img, fftDims);
        Img<FloatType> deconvolved = img.factory().create(ifftDims); //Filters.fftpad(img, fftDims, false)
        ij.op().filter().ifft(deconvolved, X);
//        Filters.clamp(deconvolved);
        deconvolved = Filters.unshift(deconvolved);
        deconvolved = Filters.cropCentered(deconvolved, Filters.getDimensions(img));
        getDataInterface().getDeconvolvedImage().set(deconvolved);
    }


}

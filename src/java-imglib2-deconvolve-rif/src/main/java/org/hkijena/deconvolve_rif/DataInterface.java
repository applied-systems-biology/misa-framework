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

package org.hkijena.deconvolve_rif;

import net.imglib2.type.numeric.integer.UnsignedByteType;
import net.imglib2.type.numeric.integer.UnsignedShortType;
import net.imglib2.type.numeric.real.FloatType;
import org.hkijena.deconvolve_rif.caches.TIFFImageCache;

import java.nio.file.Path;

public class DataInterface {

    private Path outputDirectory;
    private TIFFImageCache<UnsignedShortType> inputImage;
    private TIFFImageCache<FloatType> psfImage;
    private TIFFImageCache<FloatType> convolvedImage;
    private TIFFImageCache<FloatType> deconvolvedImage;

    public DataInterface(Path inputDirectory, Path outputDirectory) {
        this.outputDirectory = outputDirectory;
        inputImage = new TIFFImageCache<>(inputDirectory.resolve("in").resolve("data.tif"), new UnsignedShortType());
        psfImage = new TIFFImageCache<>(inputDirectory.resolve("psf").resolve("psf.tif"), new FloatType());
        convolvedImage = new TIFFImageCache<>(outputDirectory.resolve("convolved.tif"), new FloatType(), getInputImage());
        deconvolvedImage = new TIFFImageCache<>(outputDirectory.resolve("deconvolved.tif"), new FloatType(), getInputImage());
    }

    @Override
    public String toString() {
        return outputDirectory.toString();
    }

    public Path getOutputDirectory() {
        return outputDirectory;
    }

    public TIFFImageCache<UnsignedShortType> getInputImage() {
        return inputImage;
    }

    public TIFFImageCache<FloatType> getPsfImage() {
        return psfImage;
    }

    public TIFFImageCache<FloatType> getDeconvolvedImage() {
        return deconvolvedImage;
    }

    public TIFFImageCache<FloatType> getConvolvedImage() {
        return convolvedImage;
    }
}

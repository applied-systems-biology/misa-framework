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

import net.imglib2.type.numeric.integer.IntType;
import net.imglib2.type.numeric.integer.UnsignedShortType;
import org.hkijena.segment_cells.caches.TIFFImageCache;

import java.nio.file.Path;

public class ExperimentDataInterface {
    private Path inputDirectory;
    private SampleDataInterface sample;

    private TIFFImageCache<UnsignedShortType> inputImage;
    private TIFFImageCache<IntType> outputLabel;

    public ExperimentDataInterface(Path inputDirectory, SampleDataInterface sample) {
        this.inputDirectory = inputDirectory;
        this.sample = sample;
        this.inputImage = new TIFFImageCache<>(inputDirectory.resolve("channel1.tif"), new UnsignedShortType());
        this.outputLabel = new TIFFImageCache<>(sample.getOutputDirectory().resolve(inputDirectory.getFileName().toString() + ".tif"), new IntType(), this.inputImage);
    }

    public SampleDataInterface getSampleDataInterface() {
        return sample;
    }

    @Override
    public String toString() {
        return inputDirectory.getFileName().toString();
    }

    public TIFFImageCache<UnsignedShortType> getInputImage() {
        return inputImage;
    }

    public TIFFImageCache<IntType> getOutputLabel() {
        return outputLabel;
    }
}

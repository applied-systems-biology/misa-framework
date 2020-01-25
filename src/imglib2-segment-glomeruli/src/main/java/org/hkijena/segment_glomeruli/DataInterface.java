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

package org.hkijena.segment_glomeruli;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import net.imglib2.type.numeric.integer.UnsignedByteType;
import net.imglib2.type.numeric.integer.UnsignedIntType;
import org.hkijena.segment_glomeruli.caches.TIFFPlanesImageCache;
import org.hkijena.segment_glomeruli.data.GlomeruliQuantificationResult;
import org.hkijena.segment_glomeruli.data.TissueQuantificationResult;

import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Path;

public class DataInterface {

    private Path outputDirectory;

    private double voxelSizeXY;
    private double voxelSizeZ;

    private TIFFPlanesImageCache<UnsignedByteType> inputData;
    private TIFFPlanesImageCache<UnsignedByteType> tissueOutputData;
    private TIFFPlanesImageCache<UnsignedByteType> glomeruli2DOutputData;
    private TIFFPlanesImageCache<UnsignedIntType> glomeruli3DOutputData;

    private TissueQuantificationResult tissueQuantificationResult = new TissueQuantificationResult();
    private GlomeruliQuantificationResult glomeruliQuantificationResult = new GlomeruliQuantificationResult();

    private volatile long tissuePixelCount = 0;

    public DataInterface(Path inputImagePath, Path outputDirectory, double voxelSizeXY, double voxelSizeZ) {
        this.outputDirectory = outputDirectory;
        this.voxelSizeXY = voxelSizeXY;
        this.voxelSizeZ = voxelSizeZ;
        try {
            Utils.ensureDirectory(outputDirectory);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        this.inputData = new TIFFPlanesImageCache<>(inputImagePath, new UnsignedByteType());
        this.tissueOutputData = new TIFFPlanesImageCache<>(outputDirectory.resolve("tissue"), new UnsignedByteType(), inputData);
        this.glomeruli2DOutputData = new TIFFPlanesImageCache<>(outputDirectory.resolve("glomeruli2d"), new UnsignedByteType(), inputData);
        this.glomeruli3DOutputData = new TIFFPlanesImageCache<>(outputDirectory.resolve("glomeruli3d"), new UnsignedIntType(), inputData);
    }

    public TIFFPlanesImageCache<UnsignedByteType> getInputData() { return inputData; }

    public TIFFPlanesImageCache<UnsignedByteType> getTissueOutputData() {
        return tissueOutputData;
    }

    public TIFFPlanesImageCache<UnsignedByteType> getGlomeruli2DOutputData() {
        return glomeruli2DOutputData;
    }

    public TIFFPlanesImageCache<UnsignedIntType> getGlomeruli3DOutputData() {
        return glomeruli3DOutputData;
    }

    public double getVoxelSizeXY() {
        return voxelSizeXY;
    }

    public double getVoxelSizeZ() {
        return voxelSizeZ;
    }

    public long getTissuePixelCount() {
        return tissuePixelCount;
    }

    public synchronized void addTissuePixelCount(long count) {
        this.tissuePixelCount += count;
    }

    private void saveTissueQuantificationResults() {
        try(FileWriter writer = new FileWriter(outputDirectory.resolve("tissue_quantified.json").toFile())) {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            String json = gson.toJson(tissueQuantificationResult);
            writer.write(json);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void saveGlomeruliQuantificationResults() {
        try(FileWriter writer = new FileWriter(outputDirectory.resolve("glomeruli.json").toFile())) {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            String json = gson.toJson(glomeruliQuantificationResult);
            writer.write(json);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void saveQuantificationResults() {
        saveTissueQuantificationResults();
        saveGlomeruliQuantificationResults();
    }

    public TissueQuantificationResult getTissueQuantificationResult() {
        return tissueQuantificationResult;
    }

    public GlomeruliQuantificationResult getGlomeruliQuantificationResult() {
        return glomeruliQuantificationResult;
    }
}

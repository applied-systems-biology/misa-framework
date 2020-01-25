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

package org.hkijena.segment_glomeruli.caches;

import net.imglib2.img.Img;
import net.imglib2.img.ImgFactory;
import net.imglib2.img.array.ArrayImgFactory;
import net.imglib2.type.NativeType;
import net.imglib2.type.numeric.RealType;
import org.hkijena.segment_glomeruli.Main;
import org.hkijena.segment_glomeruli.Utils;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class TIFFPlanesImageCache<T extends RealType<T> & NativeType<T>> {

    private Path directory;
    private T imgDataType;
    private ImgFactory<T> factory;
    private long width;
    private long height;
    private List<String> filenames;

    public TIFFPlanesImageCache(Path directory, T imgDataType) {
        this.directory = directory;
        this.imgDataType = imgDataType;
        this.factory = new ArrayImgFactory<>(imgDataType);
        this.filenames = new ArrayList<>();

        try {
            for(Path file : Files.walk(directory).filter(path -> path.toString().endsWith(".tif")).collect(Collectors.toList())) {
                this.filenames.add(file.getFileName().toString());
            }
            this.filenames.sort(String::compareTo);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        // Load width and height
        Img<T> referenceImage = Main.IMGOPENER.openImgs(directory.resolve(filenames.get(0)).toString(), imgDataType).get(0);
        this.width = referenceImage.dimension(0);
        this.height = referenceImage.dimension(1);
    }

    public TIFFPlanesImageCache(Path directory, T imgDataType, TIFFPlanesImageCache<?> reference) {
        this.directory = directory;
        this.imgDataType = imgDataType;
        this.factory = new ArrayImgFactory<>(imgDataType);
        this.width = reference.width;
        this.height = reference.height;
        this.filenames = reference.filenames;

        try {
            Utils.ensureDirectory(directory);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public Path getPathForPlane(int z) {
        return directory.resolve(filenames.get(z));
    }

    public Img<T> getOrCreatePlane(int z) {
        if(Files.exists(getPathForPlane(z))) {
            return Main.IMGOPENER.openImgs(getPathForPlane(z).toString(), imgDataType).get(0);
        }
        else {
            return factory.create(getXSize(), getYSize());
        }
    }

    public void setPlane(int z, Img<T> img) {
        Utils.writeAsCompressedTIFF(img, getPathForPlane(z));
    }

    public long getXSize() {
        return width;
    }

    public long getYSize() {
        return height;
    }

    public int getZSize() {
        return filenames.size();
    }
}

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

package org.hkijena.segment_cells.caches;

import net.imglib2.img.Img;
import net.imglib2.img.ImgFactory;
import net.imglib2.img.array.ArrayImgFactory;
import net.imglib2.type.NativeType;
import net.imglib2.type.numeric.RealType;
import org.hkijena.segment_cells.Main;
import org.hkijena.segment_cells.Utils;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class TIFFImageCache<T extends RealType<T> & NativeType<T>> {

    private Path file;
    private T imgDataType;
    private ImgFactory<T> factory;
    private long width;
    private long height;

    public TIFFImageCache(Path file, T imgDataType) {
        this.file = file;
        this.imgDataType = imgDataType;
        this.factory = new ArrayImgFactory<>(imgDataType);

        // Load width and height
        Img<T> referenceImage = Main.IMGOPENER.openImgs(file.toString(), imgDataType).get(0);
        this.width = referenceImage.dimension(0);
        this.height = referenceImage.dimension(1);
    }

    public TIFFImageCache(Path file, T imgDataType, TIFFImageCache<?> reference) {
        this.file = file;
        this.imgDataType = imgDataType;
        this.factory = new ArrayImgFactory<>(imgDataType);
        this.width = reference.width;
        this.height = reference.height;

        try {
            Utils.ensureDirectory(file.getParent());
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public Img<T> getOrCreate() {
        if(Files.exists(file)) {
            return Main.IMGOPENER.openImgs(file.toString(), imgDataType).get(0);
        }
        else {
            return factory.create(getXSize(), getYSize());
        }
    }

    public void set(Img<T> img) {
        Utils.writeAsCompressedTIFF(img, file);
    }

    public long getXSize() {
        return width;
    }

    public long getYSize() {
        return height;
    }
}

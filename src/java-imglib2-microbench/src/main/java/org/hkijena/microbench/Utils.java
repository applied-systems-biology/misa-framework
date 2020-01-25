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

import io.scif.config.SCIFIOConfig;
import io.scif.formats.tiff.TiffCompression;
import io.scif.formats.tiff.TiffSaver;
import net.imglib2.img.Img;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class Utils {

    public static void ensureDirectory(Path path) throws IOException {
        if(!Files.exists(path)) {
            Files.createDirectories(path);
        }
    }

    public static <T> void writeAsCompressedTIFF(Img<T> img, Path filename) {
        SCIFIOConfig config = new SCIFIOConfig();
        config.writerSetCompression("LZW");
        Main.IMGSAVER.saveImg(filename.toString(), img, config);
    }
}

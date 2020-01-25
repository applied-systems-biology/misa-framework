/*
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

package org.hkijena.misa_imagej.ui.workbench;

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISACacheIOType;
import org.hkijena.misa_imagej.api.workbench.MISAOutput;

import javax.swing.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class MISAOutputCacheUI extends JPanel {

    private MISAOutput misaOutput;
    private MISACache cache;

    public MISAOutputCacheUI(MISAOutput misaOutput, MISACache cache) {
        this.misaOutput = misaOutput;
        this.cache = cache;
    }

    public MISAOutput getMisaOutput() {
        return misaOutput;
    }

    public MISACache getCache() {
        return cache;
    }

    /**
     * Returns the path of this cache within the filesystem
     * If returns null, the folder does not exist
     * @return
     */
    public Path getFilesystemPath() {
        if(getCache().getIOType() == MISACacheIOType.Exported) {
            Path path = misaOutput.getRootPath().resolve(cache.getSample().getName()).resolve(cache.getRelativePath());
            if(!Files.exists(path))
                return null;
            else
                return path;
        }
        else {
            if(misaOutput.getParameters() != null) {
                if(misaOutput.getParameters().has("filesystem")) {
                    if(misaOutput.getParameters().getAsJsonObject("filesystem").has("input-directory")) {
                        Path directory = Paths.get(misaOutput.getParameters().getAsJsonObject("filesystem").
                                getAsJsonPrimitive("input-directory").getAsString());
                        directory = directory.resolve(cache.getSample().getName()).resolve(cache.getRelativePath());
                        if(Files.exists(directory)) {
                            return directory;
                        }
                    }
                }
            }
            return null;
        }
    }
}

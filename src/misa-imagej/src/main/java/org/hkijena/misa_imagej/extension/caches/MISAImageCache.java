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

package org.hkijena.misa_imagej.extension.caches;

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISADataSource;
import org.hkijena.misa_imagej.api.MISAFilesystemEntry;
import org.hkijena.misa_imagej.api.MISASample;
import org.hkijena.misa_imagej.extension.datasources.MISAImageDataSource;
import org.hkijena.misa_imagej.extension.datasources.MISAOMETiffDataSource;

public class MISAImageCache extends MISACache {

    private MISAImageDataSource nativeDataSource;


    public MISAImageCache(MISASample sample, MISAFilesystemEntry filesystemEntry) {
        super(sample, filesystemEntry);
        nativeDataSource = new MISAImageDataSource(this);
        addAvailableDataSource(nativeDataSource);
    }

    @Override
    public String getCacheTypeName() {
        return "Image";
    }

    @Override
    public MISADataSource getPreferredDataSource() {
        return nativeDataSource;
    }
}

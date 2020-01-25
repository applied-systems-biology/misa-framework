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

package org.hkijena.misa_imagej.extension.datasources;

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISADataSource;
import org.hkijena.misa_imagej.api.MISAValidityReport;

import java.nio.file.Path;

/**
 * Data source that does nothing.
 * Used for internal purposes
 */
public class MISADummyDataSource implements MISADataSource {

    private final MISACache cache;

    public MISADummyDataSource(MISACache cache) {
        this.cache = cache;
    }

    @Override
    public void install(Path installFolder, boolean forceCopy) {

    }

    @Override
    public String getName() {
        return "Dummy";
    }

    @Override
    public boolean isEditable() {
        return false;
    }

    @Override
    public MISACache getCache() {
        return cache;
    }

    @Override
    public MISAValidityReport getValidityReport() {
        return new MISAValidityReport();
    }
}

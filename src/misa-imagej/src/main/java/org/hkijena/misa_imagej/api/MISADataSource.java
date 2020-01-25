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

package org.hkijena.misa_imagej.api;

import java.nio.file.Path;

/**
 * Interface for any type of data that is an input of a cache
 */
public interface MISADataSource extends MISAValidatable {
    /**
     *  Installs this cache into the install folder
     * @param installFolder
     * @param forceCopy forces copying all files into the install folder
     */
    void install(Path installFolder, boolean forceCopy);

    /**
     * Returns a descriptive name for this data source
     * @return
     */
    String getName();

    /**
     * Returns true if this data source should be editable
     * @return
     */
    boolean isEditable();

    /**
     * Returns the cache that this data source is assigned to
     * @return
     */
    MISACache getCache();
}

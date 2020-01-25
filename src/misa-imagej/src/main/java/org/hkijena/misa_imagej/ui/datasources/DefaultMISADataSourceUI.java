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

package org.hkijena.misa_imagej.ui.datasources;

import org.hkijena.misa_imagej.api.MISADataSource;

import java.awt.*;

public class DefaultMISADataSourceUI extends MISADataSourceUI {

    public DefaultMISADataSourceUI(MISADataSource dataSource) {
        super(dataSource);
    }

    @Override
    protected void initialize() {
        setLayout(new GridBagLayout());
//        UIUtils.createDescriptionLabelUI(this, "No settings available for data of type '" + getDataSource().getCacheTypeName() + "'", 0, 0);
    }
}

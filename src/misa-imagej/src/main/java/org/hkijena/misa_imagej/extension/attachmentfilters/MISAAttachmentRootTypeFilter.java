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

package org.hkijena.misa_imagej.extension.attachmentfilters;

import org.hkijena.misa_imagej.api.workbench.MISAAttachmentDatabase;
import org.hkijena.misa_imagej.api.workbench.PreparedStatementValuesBuilder;
import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilter;

import java.sql.SQLException;

public class MISAAttachmentRootTypeFilter extends MISAAttachmentFilter {

    public MISAAttachmentRootTypeFilter(MISAAttachmentDatabase database) {
        super(database);
    }

    @Override
    public String toSQLQuery() {
        return toSQLStatement();
    }

    @Override
    public String toSQLStatement() {
        return "\"serialization-id\" is property or property is '\"' || \"serialization-id\" || '\"'";
    }

    @Override
    public void setSQLStatementVariables(PreparedStatementValuesBuilder builder) throws SQLException {

    }
}

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
import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilterChangedEvent;

public class MISAAttachmentSQLFilter extends MISAAttachmentFilter {

    private String sql = "true";

    public MISAAttachmentSQLFilter(MISAAttachmentDatabase database) {
        super(database);
    }

    @Override
    public String toSQLQuery() {
        return toSQLStatement();
    }

    @Override
    public String toSQLStatement() {
        return sql;
    }

    @Override
    public void setSQLStatementVariables(PreparedStatementValuesBuilder builder) {

    }

    public void setSql(String sql) {
        this.sql = sql;
        getEventBus().post(new MISAAttachmentFilterChangedEvent(this));
    }
}

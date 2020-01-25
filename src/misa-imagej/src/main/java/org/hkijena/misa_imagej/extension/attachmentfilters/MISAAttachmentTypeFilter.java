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
import org.hkijena.misa_imagej.utils.SQLUtils;

import java.sql.SQLException;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

public class MISAAttachmentTypeFilter extends MISAAttachmentFilter {

    private Set<String> serializationIds = new HashSet<>();


    public MISAAttachmentTypeFilter(MISAAttachmentDatabase database) {
        super(database);
        if(database.getMisaOutput().hasAttachmentSchemas()) {
            serializationIds.addAll(database.getMisaOutput().getAttachmentSchemas().keySet());
        }
    }

    @Override
    public String toSQLQuery() {
        if(serializationIds.isEmpty())
            return "false";
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("(");
        boolean first = true;
        for(String id : serializationIds) {
            if(!first) {
                stringBuilder.append(" or ");
            }
            stringBuilder.append(" \"serialization-id\" is ").append(SQLUtils.value(id));
            first = false;
        }
        stringBuilder.append(" )");
        return stringBuilder.toString();
    }

    @Override
    public String toSQLStatement() {
        if(serializationIds.isEmpty())
            return "false";
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("(");
        boolean first = true;
        for(String id : serializationIds) {
            if(!first) {
                stringBuilder.append(" or ");
            }
            stringBuilder.append(" \"serialization-id\" is ?");
            first = false;
        }
        stringBuilder.append(" )");
        return stringBuilder.toString();
    }

    @Override
    public void setSQLStatementVariables(PreparedStatementValuesBuilder builder) throws SQLException {
        for(String id : serializationIds) {
            builder.addString(id);
        }
    }

    public void addSerializationId(String id) {
        serializationIds.add(id);
        getEventBus().post(new MISAAttachmentFilterChangedEvent(this));
    }

    public void removeSerializationId(String id) {
        serializationIds.remove(id);
        getEventBus().post(new MISAAttachmentFilterChangedEvent(this));
    }

    public Collection<String> getSerializationIds() {
        return Collections.unmodifiableCollection(serializationIds);
    }
}

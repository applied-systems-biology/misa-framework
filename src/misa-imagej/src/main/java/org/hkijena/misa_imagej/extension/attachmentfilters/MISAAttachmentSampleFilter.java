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

import org.hkijena.misa_imagej.api.MISASample;
import org.hkijena.misa_imagej.api.workbench.MISAAttachmentDatabase;
import org.hkijena.misa_imagej.api.workbench.PreparedStatementValuesBuilder;
import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilter;
import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilterChangedEvent;
import org.hkijena.misa_imagej.utils.SQLUtils;

import java.sql.SQLException;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;

public class MISAAttachmentSampleFilter extends MISAAttachmentFilter {

    private Set<MISASample> samples = new HashSet<>();

    public MISAAttachmentSampleFilter(MISAAttachmentDatabase database) {
        super(database);
        samples.addAll(database.getMisaOutput().getModuleInstance().getSamples().values());
    }

    public Collection<MISASample> getSamples() {
        return samples;
    }

    public void addSample(MISASample sample) {
        samples.add(sample);
        getEventBus().post(new MISAAttachmentFilterChangedEvent(this));
    }

    public void removeSample(MISASample sample) {
        samples.remove(sample);
        getEventBus().post(new MISAAttachmentFilterChangedEvent(this));
    }

    @Override
    public String toSQLQuery() {
        if(samples.isEmpty())
            return "false";
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("(");
        boolean first = true;
        for(MISASample sample : samples) {
            if(!first) {
                stringBuilder.append(" or ");
            }
            stringBuilder.append(" sample is ").append(SQLUtils.value(sample.getName()));
            first = false;
        }
        stringBuilder.append(" )");
        return stringBuilder.toString();
    }

    @Override
    public String toSQLStatement() {
        if(samples.isEmpty())
            return "false";
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("(");
        boolean first = true;
        for(MISASample sample : samples) {
            if(!first) {
                stringBuilder.append(" or ");
            }
            stringBuilder.append(" sample is ?");
            first = false;
        }
        stringBuilder.append(" )");
        return stringBuilder.toString();
    }

    @Override
    public void setSQLStatementVariables(PreparedStatementValuesBuilder builder) throws SQLException {
        for(MISASample sample : samples) {
            builder.addString(sample.getName());
        }
    }
}

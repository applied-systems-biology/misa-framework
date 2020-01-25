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

import org.hkijena.misa_imagej.api.MISACache;
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

public class MISAAttachmentCacheFilter extends MISAAttachmentFilter {

    private Set<String> caches = new HashSet<>();

    public MISAAttachmentCacheFilter(MISAAttachmentDatabase database) {
        super(database);
        MISASample sample = database.getMisaOutput().getModuleInstance().getOrCreateAnySample();
        for(MISACache cache : sample.getImportedCaches()) {
            String cacheName = "imported/" + cache.getRelativePath();
            caches.add(cacheName);
        }
        for(MISACache cache : sample.getExportedCaches()) {
            String cacheName = "exported/" + cache.getRelativePath();
            caches.add(cacheName);
        }
    }

    public Collection<String> getCaches() {
        return caches;
    }

    public void addCache(String cache) {
        caches.add(cache);
        getEventBus().post(new MISAAttachmentFilterChangedEvent(this));
    }

    public void removeCache(String cache) {
        caches.remove(cache);
        getEventBus().post(new MISAAttachmentFilterChangedEvent(this));
    }

    @Override
    public String toSQLStatement() {
        if(caches.isEmpty())
            return "false";
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("(");
        boolean first = true;
        for(String cache : caches) {
            if(!first) {
                stringBuilder.append(" or ");
            }
            stringBuilder.append(" cache like ? escape '\\'");
            first = false;
        }
        stringBuilder.append(" )");
        return stringBuilder.toString();
    }

    @Override
    public String toSQLQuery() {
        if(caches.isEmpty())
            return "false";
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("(");
        boolean first = true;
        for(String cache : caches) {
            if(!first) {
                stringBuilder.append(" or ");
            }
            stringBuilder.append(" cache like ' escape '\\'").append(SQLUtils.escapeWildcardsForSQLite(cache)).append("%").append("'");
            first = false;
        }
        stringBuilder.append(" )");
        return stringBuilder.toString();
    }

    @Override
    public void setSQLStatementVariables(PreparedStatementValuesBuilder builder) throws SQLException {
        for(String cache : caches) {
            builder.addString(cache + "%");
        }
    }
}

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

package org.hkijena.misa_imagej.api.workbench.table;

import org.hkijena.misa_imagej.api.MISAAttachment;
import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISASample;

import java.sql.SQLException;

public class MISAAttachmentTableCacheColumn implements MISAAttachmentTableColumn {

    @Override
    public Object getValue(MISAAttachmentTable table, int id, String sampleName, String cacheAndSubCache, String property, String serializationId, MISAAttachment attachment) throws SQLException {
        MISASample sample = table.getDatabase().getMisaOutput().getModuleInstance().getSample(sampleName);
        MISACache cache = sample.findMatchingCache(cacheAndSubCache);
        return cache.getFullRelativePath();
    }

    @Override
    public String getName() {
        return "Data";
    }
}

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
import org.hkijena.misa_imagej.api.MISACacheIOType;
import org.hkijena.misa_imagej.api.MISASample;
import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilter;
import org.hkijena.misa_imagej.utils.ui.ColorIcon;

import javax.swing.*;
import javax.swing.event.TableModelEvent;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableModel;
import java.awt.*;

public class MISAAttachmentCacheFilterUI extends DefaultTabularMISAAttachmentFilterUI {

    public MISAAttachmentCacheFilterUI(MISAAttachmentFilter filter) {
        super(filter);
        initialize();
    }

    private void initialize() {
        TableModel model = getSelectionTable().getModel();
       model.addTableModelListener(e -> {
            if(e.getType() == TableModelEvent.UPDATE && e.getColumn() == 0) {
                for(int i = e.getFirstRow(); i <= e.getLastRow(); ++i) {
                    MISACache cache = (MISACache) model.getValueAt(i, 1);
                    String cacheName;
                    if(cache.getIOType() == MISACacheIOType.Imported)
                        cacheName = "imported/" + cache.getRelativePath();
                    else
                        cacheName = "exported/" + cache.getRelativePath();
                    boolean isChecked = (boolean)model.getValueAt(i, 0);
                    if(isChecked != getNativeFilter().getCaches().contains(cacheName)) {
                        if(isChecked) {
                            getNativeFilter().addCache(cacheName);
                        }
                        else {
                            getNativeFilter().removeCache(cacheName);
                        }
                    }
                }
            }
        });

       getSelectionTable().setDefaultRenderer(MISACache.class, new CacheCellRenderer());
    }

    @Override
    protected Class getTableContentClass() {
        return MISACache.class;
    }

    @Override
    protected void initializeTableModel(DefaultTableModel model) {
        MISASample sample = getFilter().getDatabase().getMisaOutput().getModuleInstance().getOrCreateAnySample();
        for(MISACache cache : sample.getImportedCaches()) {
            String cacheName = "imported/" + cache.getRelativePath();
            model.addRow(new Object[]{ getNativeFilter().getCaches().contains(cacheName), cache });
        }
        for(MISACache cache : sample.getExportedCaches()) {
            String cacheName = "exported/" + cache.getRelativePath();
            model.addRow(new Object[]{ getNativeFilter().getCaches().contains(cacheName), cache });
        }
    }

    public MISAAttachmentCacheFilter getNativeFilter() {
        return (MISAAttachmentCacheFilter)getFilter();
    }

    public static class CacheCellRenderer extends JLabel implements TableCellRenderer {

        private ColorIcon icon = new ColorIcon(16,16);

        public CacheCellRenderer() {
        }

        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
            if(value instanceof MISACache) {
                MISACache cache = (MISACache)value;
                String cacheName;
                if(cache.getIOType() == MISACacheIOType.Imported)
                    cacheName = "imported/" + cache.getRelativePath();
                else
                    cacheName = "exported/" + cache.getRelativePath();
                setText(cacheName);
                icon.setColor(cache.toColor());
                setIcon(icon);

                if(isSelected || hasFocus) {
                    setBackground(new Color(184, 207, 229));
                }
                else {
                    setBackground(new Color(255,255,255));
                }

                setToolTipText("Data of type " + cache.getCacheTypeName());
            }
            else {
                setText(null);
                setIcon(null);
                setToolTipText(null);
            }
            return this;
        }
    }
}

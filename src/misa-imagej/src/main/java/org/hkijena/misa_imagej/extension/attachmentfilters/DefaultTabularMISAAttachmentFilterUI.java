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

import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilter;
import org.hkijena.misa_imagej.ui.workbench.objectbrowser.MISAAttachmentFilterUI;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableModel;
import java.awt.*;

public abstract class DefaultTabularMISAAttachmentFilterUI extends MISAAttachmentFilterUI {

    private JTable selectionTable;

    public DefaultTabularMISAAttachmentFilterUI(MISAAttachmentFilter filter) {
        super(filter);
        initialize();
    }

    private void initialize() {
        DefaultTableModel model = new DefaultTableModel() {
            @Override
            public Class<?> getColumnClass(int columnIndex) {
                if(columnIndex == 0)
                    return Boolean.class;
                else if (columnIndex == 1)
                    return getTableContentClass();
                return super.getColumnClass(columnIndex);
            }
        };
        model.setColumnCount(2);
        initializeTableModel(model);

        selectionTable = new JTable(model);
        selectionTable.setAutoResizeMode(JTable.AUTO_RESIZE_LAST_COLUMN);
        selectionTable.getColumnModel().getColumn(0).setMaxWidth(20);
        selectionTable.setShowGrid(false);
        selectionTable.setOpaque(false);

        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(selectionTable, BorderLayout.CENTER);

        JPanel bottomPanel = new JPanel();
        bottomPanel.setLayout(new BoxLayout(bottomPanel, BoxLayout.LINE_AXIS));
        bottomPanel.setBorder(BorderFactory.createEmptyBorder(8,0,0,0));

        // "Select all" button
        JButton selectAllButton = new JButton(UIUtils.getIconFromResources("select-all.png"));
        selectAllButton.setToolTipText("Select all");
        selectAllButton.addActionListener(e -> selectAll());
        UIUtils.makeFlatWithoutMargin(selectAllButton);
        bottomPanel.add(selectAllButton);

        // "Select all" button
        JButton deselectAllButton = new JButton(UIUtils.getIconFromResources("clear-brush.png"));
        deselectAllButton.setToolTipText("Clear selection");
        deselectAllButton.addActionListener(e -> deselectAll());
        UIUtils.makeFlatWithoutMargin(deselectAllButton);
        bottomPanel.add(Box.createHorizontalStrut(4));
        bottomPanel.add(deselectAllButton);

        // "Invert selection" button
        JButton invertSelectionButton = new JButton(UIUtils.getIconFromResources("invert.png"));
        invertSelectionButton.addActionListener(e -> invertSelection());
        UIUtils.makeFlatWithoutMargin(invertSelectionButton);
        bottomPanel.add(Box.createHorizontalStrut(4));
        bottomPanel.add(invertSelectionButton);

        bottomPanel.add(Box.createHorizontalGlue());
        getContentPane().add(bottomPanel, BorderLayout.SOUTH);
    }

    private void deselectAll() {
        TableModel model = selectionTable.getModel();
        for(int row = 0; row < model.getRowCount(); ++row) {
            model.setValueAt(false, row, 0);
        }
    }

    private void invertSelection() {
        TableModel model = selectionTable.getModel();
        for(int row = 0; row < model.getRowCount(); ++row) {
            model.setValueAt(!(boolean)model.getValueAt(row, 0), row, 0);
        }
    }

    private void selectAll() {
        TableModel model = selectionTable.getModel();
        for(int row = 0; row < model.getRowCount(); ++row) {
            model.setValueAt(true, row, 0);
        }
    }

    protected abstract Class getTableContentClass();

    protected abstract void initializeTableModel(DefaultTableModel model);

    public JTable getSelectionTable() {
        return selectionTable;
    }
}

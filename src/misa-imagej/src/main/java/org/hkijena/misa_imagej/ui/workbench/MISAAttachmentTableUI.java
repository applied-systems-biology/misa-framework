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

package org.hkijena.misa_imagej.ui.workbench;

import com.google.common.eventbus.Subscribe;
import org.hkijena.misa_imagej.api.workbench.table.MISAAttachmentTable;
import org.hkijena.misa_imagej.api.workbench.table.MISAAttachmentTableColumn;
import org.hkijena.misa_imagej.utils.BusyCursor;
import org.jdesktop.swingx.JXTable;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.sql.SQLException;

public class MISAAttachmentTableUI extends JPanel {

    private MISAAttachmentTable table;
    private MISAAttachmentTable.Iterator currentRow;
    private JXTable jTable;
    private JScrollPane scrollPane;

    public MISAAttachmentTableUI() {
        initialize();
    }

    private void initialize() {
        setLayout(new BorderLayout());
        jTable = new JXTable();
        jTable.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
        scrollPane = new JScrollPane(jTable);
        add(scrollPane, BorderLayout.CENTER);

        scrollPane.getVerticalScrollBar().addAdjustmentListener(e -> addRowIfNeeded());
    }

    public MISAAttachmentTable getTable() {
        return table;
    }

    public void setTable(MISAAttachmentTable table) {
        if(this.table != null)
            this.table.getEventBus().unregister(this);
        this.table = table;
        table.getEventBus().register(this);
        clearRows();
    }

    @Subscribe
    public void handleTableColumnsChangedEvent(MISAAttachmentTable.ColumnsChangedEvent event) {
        clearRows();
    }

    private void addRowIfNeeded() {
        if(currentRow != null) {
            JScrollBar scrollBar = scrollPane.getVerticalScrollBar();
            if(scrollBar.getMaximum() <= scrollBar.getVisibleAmount()
                    || (scrollBar.getValue() + scrollBar.getVisibleAmount()) > (scrollBar.getMaximum() * 0.9)) {
                try(BusyCursor busyCursor = new BusyCursor(this)) {
                    Object[] values = currentRow.nextRow();
                    if (values == null) {
                        currentRow = null;
                        return;
                    }

                    ((DefaultTableModel) jTable.getModel()).addRow(values);
                    revalidate();
                    repaint();
                    jTable.packAll();

                    SwingUtilities.invokeLater(() -> addRowIfNeeded());
                } catch (SQLException e) {
                    throw new RuntimeException(e);
                }
            }
        }
    }

    private void clearRows() {
        if(table.getColumns().isEmpty()) {
            currentRow = null;
            jTable.setModel(new DefaultTableModel());
        }
        else {
            if(currentRow != null) {
                try {
                    currentRow.close();
                } catch (SQLException e) {
                    throw new RuntimeException(e);
                }
            }
            currentRow = table.createIterator();
            DefaultTableModel model = new DefaultTableModel();
            for(MISAAttachmentTableColumn column : table.getColumns()) {
                model.addColumn(column.getName());
            }
            jTable.setModel(model);

            addRowIfNeeded();
        }
    }
}

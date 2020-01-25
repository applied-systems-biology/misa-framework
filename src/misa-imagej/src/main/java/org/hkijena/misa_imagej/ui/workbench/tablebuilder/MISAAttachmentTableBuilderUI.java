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

package org.hkijena.misa_imagej.ui.workbench.tablebuilder;

import org.hkijena.misa_imagej.api.json.JSONSchemaObject;
import org.hkijena.misa_imagej.api.workbench.MISAAttachmentDatabase;
import org.hkijena.misa_imagej.api.workbench.MISAOutput;
import org.hkijena.misa_imagej.api.workbench.table.*;
import org.hkijena.misa_imagej.ui.workbench.MISAAttachmentTableUI;
import org.hkijena.misa_imagej.ui.workbench.MISAWorkbenchUI;
import org.hkijena.misa_imagej.ui.workbench.tableanalyzer.MISATableAnalyzerUI;
import org.hkijena.misa_imagej.utils.BusyCursor;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.DocumentTabPane;
import org.hkijena.misa_imagej.utils.ui.MonochromeColorIcon;

import javax.swing.*;
import java.awt.*;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

public class MISAAttachmentTableBuilderUI extends JPanel {
    private MISAWorkbenchUI workbench;
    private MISAAttachmentDatabase database;
    private List<String> databaseFilters;
    private MISAAttachmentTable attachmentTable;
    private MISAAttachmentTableUI tableUI;

    private JComboBox<String> objectSelection;
    private JToggleButton toggleAutoUpdate;

    public MISAAttachmentTableBuilderUI(MISAWorkbenchUI workbench, MISAAttachmentDatabase database) {
        this.workbench = workbench;
        this.database = database;
        initialize();
    }

    private void initialize() {
        setLayout(new BorderLayout());

        JToolBar toolBar = new JToolBar();

        toggleAutoUpdate = new JToggleButton(UIUtils.getIconFromResources("cog.png"));
        toggleAutoUpdate.setSelected(true);
        toggleAutoUpdate.setToolTipText("Automatically update table");
        toolBar.add(toggleAutoUpdate);

        JButton syncFilters = new JButton("Update", UIUtils.getIconFromResources("refresh.png"));
        syncFilters.addActionListener(e -> updateTable());
        toolBar.add(syncFilters);

        JButton exportButton = new JButton("Export", UIUtils.getIconFromResources("save.png"));
        {
            JPopupMenu exportPopup = UIUtils.addPopupMenuToComponent(exportButton);

            JMenuItem exportAsCSV = new JMenuItem("as CSV table (*.csv)", UIUtils.getIconFromResources("filetype-csv.png"));
            exportAsCSV.addActionListener(e -> exportTable(MISAAttachmentTableExporterUI.FileType.CSV));
            exportPopup.add(exportAsCSV);

            JMenuItem exportAsXLSX = new JMenuItem("as Excel table (*.xlsx)", UIUtils.getIconFromResources("filetype-excel.png"));
            exportAsXLSX.addActionListener(e -> exportTable(MISAAttachmentTableExporterUI.FileType.XLSX));
            exportPopup.add(exportAsXLSX);
        }
        toolBar.add(exportButton);

        JButton sendToAnalyzer = new JButton("Analyze", UIUtils.getIconFromResources("graph.png"));
        sendToAnalyzer.addActionListener(e -> sendTableToAnalyzer());
        toolBar.add(sendToAnalyzer);

        toolBar.add(Box.createHorizontalGlue());

        objectSelection = new JComboBox<>();
        objectSelection.setRenderer(new ObjectTypeComboBoxRenderer(database.getMisaOutput()));
        objectSelection.addItemListener(e -> updateTable());
        toolBar.add(objectSelection);

        JButton editColumnsButton = new JButton("Edit columns ...", UIUtils.getIconFromResources("edit.png"));
        editColumnsButton.addActionListener(e -> editColumns());
        toolBar.add(editColumnsButton);

        add(toolBar, BorderLayout.NORTH);

        tableUI = new MISAAttachmentTableUI();
        add(tableUI, BorderLayout.CENTER);
    }

    private void sendTableToAnalyzer() {
        MISAAttachmentTableToModelExporterUI dialog = new MISAAttachmentTableToModelExporterUI(tableUI.getTable());
        dialog.setModal(true);
        dialog.pack();
        dialog.setSize(400,300);
        dialog.setLocationRelativeTo(SwingUtilities.getWindowAncestor(this));
        dialog.startOperation();
        dialog.setVisible(true);
        if(dialog.getModel() != null) {
            String name = "Table";
            if(objectSelection.getSelectedItem() != null) {
                JSONSchemaObject schemaObject = workbench.getMisaOutput().getAttachmentSchemas()
                        .getOrDefault(objectSelection.getSelectedItem().toString(), null);
                if(schemaObject != null) {
                    if(schemaObject.getDocumentationTypeTitle() != null && !schemaObject.getDocumentationTypeTitle().isEmpty())
                        name = "Table (" + schemaObject.getDocumentationTypeTitle() + ")";
                    else
                        name = "Table (" + schemaObject.getSerializationId() + ")";
                }
            }
            workbench.addTab(name,
                    UIUtils.getIconFromResources("table.png"),
                    new MISATableAnalyzerUI(workbench, dialog.getModel()),
                    DocumentTabPane.CloseMode.withAskOnCloseButton, true);
            workbench.setSelectedTab(workbench.getTabCount() - 1);
        }
    }

    private void exportTable(MISAAttachmentTableExporterUI.FileType type) {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle("Export table");
        if(fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            MISAAttachmentTableExporterUI dialog = new MISAAttachmentTableExporterUI(fileChooser.getSelectedFile().toPath(), type, tableUI.getTable());
            dialog.setModal(true);
            dialog.pack();
            dialog.setSize(400,300);
            dialog.setLocationRelativeTo(SwingUtilities.getWindowAncestor(this));
            dialog.startOperation();
            dialog.setVisible(true);
        }
    }

    private void editColumns() {
        if(attachmentTable != null) {
            MISAAttachmentTableColumnEditor editor = new MISAAttachmentTableColumnEditor(attachmentTable);
            editor.pack();
            editor.setSize(800,600);
            editor.setLocationRelativeTo(SwingUtilities.getWindowAncestor(this));
            editor.setModal(true);
            editor.setVisible(true);
        }
    }

    private void updateTable() {
        if(objectSelection.getSelectedItem() != null) {
            List<MISAAttachmentTableColumn> backup = null;
            if(attachmentTable != null && attachmentTable.getSerializationId().equals(objectSelection.getSelectedItem())) {
                backup = attachmentTable.getColumns();
            }
            attachmentTable = new MISAAttachmentTable(database, databaseFilters,
                    objectSelection.getSelectedItem().toString());
            if(backup == null) {
                attachmentTable.addColumn(new MISAAttachmentTableSampleColumn());
                attachmentTable.addColumn(new MISAAttachmentTableCacheColumn());
                attachmentTable.addColumn(new MISAAttachmentTableSubCacheColumn());
                attachmentTable.addColumn(new MISAAttachmentTablePropertyColumn());
                MISAAttachmentTableJsonValueColumn.addColumnsToTable(attachmentTable, false);
            }
            else {
                for(MISAAttachmentTableColumn column : backup) {
                    attachmentTable.addColumn(column);
                }
            }

            tableUI.setTable(attachmentTable);
        }
    }

    public void setDatabaseFilters(List<String> databaseFilters) {
        this.databaseFilters = databaseFilters;
        if(toggleAutoUpdate.isSelected()) {
            updateObjectSelection();
            updateTable();
        }
    }

    private void updateObjectSelection() {
        Object previousSelection = objectSelection.getSelectedItem();

        DefaultComboBoxModel<String> objectTypes = new DefaultComboBoxModel<>();
        try(BusyCursor cursor = new BusyCursor(this)) {
            try(ResultSet resultSet = database.query("distinct \"serialization-id\"",
                    databaseFilters, "")) {
                while(resultSet.next()) {
                    String item = resultSet.getString(1);
                    objectTypes.addElement(item);

                    if(item.equals(previousSelection)) {
                        objectTypes.setSelectedItem(item);
                    }
                }
            } catch (SQLException e) {
                throw new RuntimeException(e);
            }
        }

        objectSelection.setModel(objectTypes);
    }

    public static class ObjectTypeComboBoxRenderer extends JLabel implements ListCellRenderer<String> {

        private MISAOutput misaOutput;
        private MonochromeColorIcon icon = new MonochromeColorIcon(UIUtils.getIconFromResources("object-template.png"), Color.WHITE);

        public ObjectTypeComboBoxRenderer(MISAOutput misaOutput) {
            this.misaOutput = misaOutput;
            setIcon(icon);
        }

        @Override
        public Component getListCellRendererComponent(JList<? extends String> list, String value, int index, boolean isSelected, boolean cellHasFocus) {

            JSONSchemaObject schemaObject = misaOutput.getAttachmentSchemas().getOrDefault(value, null);
            if(schemaObject == null || schemaObject.getDocumentationTypeTitle() == null || schemaObject.getDocumentationTypeTitle().isEmpty()) {
                setText(value);
                icon.setColor(Color.GRAY);
            }
            else {
                setText(schemaObject.getDocumentationTypeTitle() + " (" + value + ")");
                icon.setColor(schemaObject.toColor());
            }

            if(isSelected) {
                setBackground(new Color(184, 207, 229));
            }
            else {
                setBackground(new Color(255,255,255));
            }

            return this;
        }
    }
}

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

package org.hkijena.misa_imagej.ui.workbench.tableanalyzer;

import org.apache.poi.openxml4j.exceptions.InvalidFormatException;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellType;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
import org.hkijena.misa_imagej.ui.workbench.MISAWorkbenchUI;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.DocumentTabPane;
import org.hkijena.misa_imagej.utils.ui.FileSelection;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class MISATableImporterDialog extends JDialog {

    private MISAWorkbenchUI workbench;
    private FileSelection fileSelection;
    private JComboBox<FileFormat> importFormat;

    public MISATableImporterDialog(MISAWorkbenchUI workbench) {
        this.workbench = workbench;
        initialize();
    }

    private void initialize() {
        setLayout(new GridBagLayout());
        setTitle("Import table");

        {
            add(new JLabel("Import path"), new GridBagConstraints() {
                {
                    gridx = 0;
                    gridy = 0;
                    anchor = GridBagConstraints.WEST;
                    insets = UIUtils.UI_PADDING;
                }
            });
            fileSelection = new FileSelection(FileSelection.Mode.OPEN);
            add(fileSelection, new GridBagConstraints() {
                {
                    gridx = 1;
                    gridy = 0;
                    fill = GridBagConstraints.HORIZONTAL;
                    gridwidth = 1;
                    insets = UIUtils.UI_PADDING;
                }
            });
        }
        {
            add(new JLabel("File format"), new GridBagConstraints() {
                {
                    gridx = 0;
                    gridy = 1;
                    anchor = GridBagConstraints.WEST;
                    insets = UIUtils.UI_PADDING;
                }
            });
            importFormat = new JComboBox<>(FileFormat.values());
            add(importFormat, new GridBagConstraints() {
                {
                    gridx = 1;
                    gridy = 1;
                    fill = GridBagConstraints.HORIZONTAL;
                    gridwidth = 1;
                    insets = UIUtils.UI_PADDING;
                }
            });
        }

        UIUtils.addFillerGridBagComponent(getContentPane(), 2, 1);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.LINE_AXIS));
        buttonPanel.add(Box.createHorizontalGlue());

        JButton cancelButton = new JButton("Cancel", UIUtils.getIconFromResources("remove.png"));
        cancelButton.addActionListener(e -> setVisible(false));
        buttonPanel.add(cancelButton);

        JButton exportButton = new JButton("Import", UIUtils.getIconFromResources("import.png"));
        exportButton.setDefaultCapable(true);
        exportButton.addActionListener(e -> {
            if(importFormat.getSelectedItem() == FileFormat.CSV) {
                importCSV();
            }
            else if(importFormat.getSelectedItem() == FileFormat.XLSX) {
                importExcel();
            }
            setVisible(false);
        });
        buttonPanel.add(exportButton);

        add(buttonPanel, new GridBagConstraints() {
            {
                gridx = 0;
                gridy = 3;
                gridwidth = 2;
                fill = GridBagConstraints.HORIZONTAL;
                insets = UIUtils.UI_PADDING;
            }
        });
    }

    private void importCSV() {
        if (fileSelection.getPath() == null)
            return;
        try(BufferedReader reader = new BufferedReader(new FileReader(fileSelection.getPath().toFile()))) {
            DefaultTableModel tableModel = new DefaultTableModel();
            String currentLine;
            boolean isFirstLine = true;
            ArrayList<String> buffer = new ArrayList<>();
            StringBuilder currentCellBuffer = new StringBuilder();
            while((currentLine = reader.readLine()) != null) {
                buffer.clear();
                currentCellBuffer.setLength(0);
                boolean isWithinQuote = false;

                for(int i = 0; i < currentLine.length(); ++i) {
                    char c = currentLine.charAt(i);
                    if(c == '\"') {
                        if(isWithinQuote) {
                            if(currentLine.charAt(i - 1) == '\"') {
                                currentCellBuffer.append("\"");
                            }
                            else {
                                isWithinQuote = false;
                            }
                        }
                        else {
                            isWithinQuote = true;
                        }
                    }
                    else if(c == ',') {
                        buffer.add(currentCellBuffer.toString());
                        currentCellBuffer.setLength(0);
                    }
                    else {
                        currentCellBuffer.append(c);
                    }
                }

                if(currentCellBuffer.length() > 0) {
                    buffer.add(currentCellBuffer.toString());
                }

                if(isFirstLine) {
                    for(String column : buffer) {
                        tableModel.addColumn(column);
                    }
                    isFirstLine = false;
                }
                else {
                    tableModel.addRow(buffer.toArray());
                }
            }

            // Create table analyzer
            workbench.addTab(fileSelection.getPath().getFileName().toString(), UIUtils.getIconFromResources("table.png"),
                    new MISATableAnalyzerUI(workbench, tableModel), DocumentTabPane.CloseMode.withAskOnCloseButton, true);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private void importExcel() {
        if (fileSelection.getPath() == null)
            return;
        try (XSSFWorkbook workbook = new XSSFWorkbook(fileSelection.getPath().toFile())) {
            for(int sheetIndex = 0; sheetIndex < workbook.getNumberOfSheets(); ++sheetIndex) {
                XSSFSheet sheet = workbook.getSheetAt(sheetIndex);
                DefaultTableModel tableModel = new DefaultTableModel();
                if(sheet.getPhysicalNumberOfRows() == 0)
                    continue;
                Row headerRow = sheet.getRow(0);
                for(int i = 0; i < headerRow.getPhysicalNumberOfCells(); ++i) {
                    tableModel.addColumn(headerRow.getCell(i).getStringCellValue());
                }

                ArrayList<Object> rowBuffer = new ArrayList<>();
                for(int row = 1; row < sheet.getPhysicalNumberOfRows(); ++row) {
                    Row xlsxRow = sheet.getRow(row);
                    rowBuffer.clear();

                    // Add missing columns
                    while(xlsxRow.getPhysicalNumberOfCells() > tableModel.getColumnCount()) {
                        tableModel.addColumn("");
                    }

                    for(int i = 0; i < xlsxRow.getPhysicalNumberOfCells(); ++i) {
                        Cell cell = xlsxRow.getCell(i);
                        if(cell.getCellType() == CellType.NUMERIC)
                            rowBuffer.add(cell.getNumericCellValue());
                        else if(cell.getCellType() == CellType.BOOLEAN)
                            rowBuffer.add(cell.getBooleanCellValue());
                        else
                            rowBuffer.add(cell.getStringCellValue());
                    }

                    tableModel.addRow(rowBuffer.toArray());
                }

                // Create table analyzer
                workbench.addTab(sheet.getSheetName(), UIUtils.getIconFromResources("table.png"),
                        new MISATableAnalyzerUI(workbench, tableModel), DocumentTabPane.CloseMode.withAskOnCloseButton, true);
            }

        } catch (IOException | InvalidFormatException e) {
            throw new RuntimeException(e);
        }
    }

    enum FileFormat {
        CSV,
        XLSX;

        @Override
        public String toString() {
            switch (this) {
                case CSV:
                    return "CSV (*.csv)";
                case XLSX:
                    return "Excel table (*.xlsx)";
                default:
                    throw new UnsupportedOperationException();
            }
        }

        public Icon toIcon() {
            switch (this) {
                case CSV:
                    return UIUtils.getIconFromResources("filetype-csv.png");
                case XLSX:
                    return UIUtils.getIconFromResources("filetype-excel.png");
                default:
                    throw new UnsupportedOperationException();
            }
        }
    }
}

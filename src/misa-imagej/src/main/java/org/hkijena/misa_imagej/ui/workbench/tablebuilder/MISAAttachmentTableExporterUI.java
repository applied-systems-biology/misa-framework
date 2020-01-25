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

import com.google.common.base.Charsets;
import com.google.common.base.Joiner;
import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellType;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
import org.hkijena.misa_imagej.api.workbench.table.MISAAttachmentTable;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.Path;

public class MISAAttachmentTableExporterUI extends JDialog {

    private Path exportPath;
    private FileType fileType;
    private MISAAttachmentTable table;
    private JProgressBar progressBar;
    private Worker worker;
    private volatile boolean isDone = false;

    public MISAAttachmentTableExporterUI(Path exportPath, FileType fileType, MISAAttachmentTable table) {
        this.exportPath = exportPath;
        this.fileType = fileType;
        this.table = table;

        initialize();
    }

    private void initialize() {
        setTitle("Exporting table");
        setLayout(new BoxLayout(getContentPane(), BoxLayout.PAGE_AXIS));
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);

        JTextArea infoLabel = new JTextArea();
        infoLabel.setBorder(null);
        infoLabel.setLineWrap(true);
        infoLabel.setWrapStyleWord(true);
        infoLabel.setEditable(false);
        infoLabel.setText("Please wait until the process is finished. This operation can take some time if there is a lot of data.");
        add(infoLabel);

        add(Box.createVerticalGlue());

        progressBar = new JProgressBar();
        progressBar.setStringPainted(true);
        progressBar.setString("0 rows exported");
        progressBar.setIndeterminate(true);
        add(progressBar);


        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.LINE_AXIS));

        buttonPanel.add(Box.createHorizontalGlue());
        buttonPanel.add(Box.createVerticalStrut(4));

        JButton cancelButton = new JButton("Cancel", UIUtils.getIconFromResources("remove.png"));
        cancelButton.addActionListener(e -> cancelOperation());
        buttonPanel.add(cancelButton);

        add(buttonPanel);

        addWindowListener(new WindowAdapter() {
            @Override
            public void windowOpened(WindowEvent e) {
                if(isDone) {
                    SwingUtilities.invokeLater(() -> setVisible(false));
                }
            }
        });
    }

    private void cancelOperation() {
        if(worker != null) {
            worker.cancel(false);
        }
        else {
            setVisible(false);
            isDone = true;
        }
    }

    public void startOperation() {
        worker = new Worker(exportPath, fileType, table);
        worker.getEventBus().register(this);
        worker.execute();
    }

    @Subscribe
    public void handleProgressEvent(ProgressEvent event) {
        progressBar.setString(event.getProgress() + " rows exported");
    }

    @Subscribe
    public void handleWorkDoneEvent(WorkDoneEvent event) {
        worker = null;
        setVisible(false);
        isDone = true;
    }

    public enum FileType {
        CSV,
        XLSX
    }

    private static class Worker extends SwingWorker {

        private Path exportPath;
        private FileType fileType;
        private MISAAttachmentTable table;
        private EventBus eventBus = new EventBus();

        private Worker(Path exportPath, FileType fileType, MISAAttachmentTable table) {
            this.exportPath = exportPath;
            this.fileType = fileType;
            this.table = table;
        }

        @Override
        protected Object doInBackground() throws Exception {
            if(fileType == FileType.CSV) {
                writeCSV();
            }
            else if(fileType == FileType.XLSX) {
                writeXLSX();
            }
            else {
                throw new UnsupportedOperationException();
            }
            return null;
        }

        private void writeCSV() throws Exception {
            try(MISAAttachmentTable.Iterator iterator = table.createIterator()) {
                int progress = 0;
                Object[] row = new Object[table.getColumns().size()];

                for(int i = 0; i < table.getColumns().size(); ++i) {
                    row[i] = table.getColumns().get(i).getName();
                }

                long lastTime = System.currentTimeMillis();

                try(BufferedOutputStream writer = new BufferedOutputStream(new FileOutputStream(exportPath.toFile()))) {
                    writeCSVRow(row, writer);
                    while((row = iterator.nextRow()) != null) {
                        ++progress;
                        if(System.currentTimeMillis() - lastTime > 1000) {
                            int finalProgress = progress;
                            SwingUtilities.invokeLater(() -> eventBus.post(new ProgressEvent(finalProgress)));
                            lastTime = System.currentTimeMillis();
                        }

                        writeCSVRow(row, writer);
                    }
                }
            }
        }

        private void writeCSVRow(Object[] row, BufferedOutputStream stream) throws IOException {
            for(int i = 0; i < row.length; ++i) {
                if(row[i] instanceof Boolean) {
                    row[i] = (Boolean)row[i] ? "TRUE" : "FALSE";
                }
                else if(row[i] instanceof Number) {
                    row[i] = row[i].toString();
                }
                else {
                    String content = "" + row[i];
                    content = content.replace("\"", "\"\"");
                    if(content.contains(",")) {
                        content = "\"" + content + "\"";
                    }
                    row[i] = content;
                }
            }
            stream.write(Joiner.on(',').join(row).getBytes(Charsets.UTF_8));
            stream.write("\n".getBytes(Charsets.UTF_8));
        }

        private void writeXLSX() throws Exception {
            try(MISAAttachmentTable.Iterator iterator = table.createIterator()) {
                int processedRows = 0;
                Object[] row;

                XSSFWorkbook workbook = new XSSFWorkbook();
                XSSFSheet sheet = workbook.createSheet("Quantification output");

                Row xlsxHeaderRow = sheet.createRow(0);
                for(int i = 0; i < table.getColumns().size(); ++i) {
                    Cell cell = xlsxHeaderRow.createCell(i, CellType.STRING);
                    cell.setCellValue(table.getColumns().get(i).getName());
                }

                long lastTime = System.currentTimeMillis();

                while((row = iterator.nextRow()) != null) {
                    Row xlsxRow = sheet.createRow(processedRows + 1);
                    for(int i = 0; i < row.length; ++i) {
                        if(row[i] instanceof Number) {
                            Cell cell = xlsxRow.createCell(i, CellType.NUMERIC);
                            cell.setCellValue(((Number)row[i]).doubleValue());
                        }
                        else if(row[i] instanceof Boolean) {
                            Cell cell = xlsxRow.createCell(i, CellType.BOOLEAN);
                            cell.setCellValue((Boolean) row[i]);
                        }
                        else {
                            Cell cell = xlsxRow.createCell(i, CellType.STRING);
                            cell.setCellValue("" + row[i]);
                        }
                    }

                    ++processedRows;
                    if(System.currentTimeMillis() - lastTime > 1000) {
                        int finalProgress = processedRows;
                        SwingUtilities.invokeLater(() -> eventBus.post(new ProgressEvent(finalProgress)));
                        lastTime = System.currentTimeMillis();
                    }
                }

                for(int i = 0; i < table.getColumns().size(); ++i) {
                    sheet.autoSizeColumn(i);
                }

                FileOutputStream stream = new FileOutputStream(exportPath.toFile());
                workbook.write(stream);
                workbook.close();
            }
        }

        @Override
        protected void done() {
            super.done();
            getEventBus().post(new WorkDoneEvent(isCancelled()));
        }

        public EventBus getEventBus() {
            return eventBus;
        }
    }

    public static class ProgressEvent {
        private int progress;

        private ProgressEvent(int progress) {
            this.progress = progress;
        }

        public int getProgress() {
            return progress;
        }
    }

    public static class WorkDoneEvent {
        private boolean canceled;

        public WorkDoneEvent(boolean canceled) {
            this.canceled = canceled;
        }

        public boolean isCanceled() {
            return canceled;
        }
    }

}

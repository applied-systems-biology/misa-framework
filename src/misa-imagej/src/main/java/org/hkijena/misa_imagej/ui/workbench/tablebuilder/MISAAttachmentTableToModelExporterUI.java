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

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import org.hkijena.misa_imagej.api.workbench.table.MISAAttachmentTable;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class MISAAttachmentTableToModelExporterUI extends JDialog {

    private DefaultTableModel model;
    private MISAAttachmentTable table;
    private JProgressBar progressBar;
    private Worker worker;
    private volatile boolean isDone = false;

    public MISAAttachmentTableToModelExporterUI(MISAAttachmentTable table) {
        this.table = table;
        initialize();
    }

    private void initialize() {
        setTitle("Building table");
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
                if (isDone) {
                    SwingUtilities.invokeLater(() -> setVisible(false));
                }
            }
        });
    }

    private void cancelOperation() {
        if (worker != null) {
            worker.cancel(false);
        } else {
            setVisible(false);
            isDone = true;
        }
    }

    public void startOperation() {
        worker = new Worker(table);
        worker.getEventBus().register(this);
        worker.execute();
    }

    @Subscribe
    public void handleProgressEvent(ProgressEvent event) {
        progressBar.setString(event.getProgress() + " rows exported");
    }

    @Subscribe
    public void handleWorkDoneEvent(WorkDoneEvent event) {
        model = worker.getResult();
        worker = null;
        setVisible(false);
        isDone = true;
    }

    public DefaultTableModel getModel() {
        return model;
    }

    private static class Worker extends SwingWorker {

        private MISAAttachmentTable table;
        private EventBus eventBus = new EventBus();
        private volatile DefaultTableModel result;

        private Worker(MISAAttachmentTable table) {
            this.table = table;
        }

        @Override
        protected Object doInBackground() throws Exception {
            try(MISAAttachmentTable.Iterator iterator = table.createIterator()) {
                int progress = 0;
                Object[] row;
                long lastTime = System.currentTimeMillis();

                DefaultTableModel model = new DefaultTableModel();

                for (int i = 0; i < table.getColumns().size(); ++i) {
                    model.addColumn(table.getColumns().get(i).getName());
                }

                while ((row = iterator.nextRow()) != null) {
                    ++progress;
                    if (System.currentTimeMillis() - lastTime > 1000) {
                        int finalProgress = progress;
                        SwingUtilities.invokeLater(() -> eventBus.post(new ProgressEvent(finalProgress)));
                        lastTime = System.currentTimeMillis();
                    }

                    model.addRow(row);
                }

                result = model;
                return null;
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

        public DefaultTableModel getResult() {
            return result;
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

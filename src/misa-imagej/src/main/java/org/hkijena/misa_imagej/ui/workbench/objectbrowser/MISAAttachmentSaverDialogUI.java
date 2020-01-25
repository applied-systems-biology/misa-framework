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

package org.hkijena.misa_imagej.ui.workbench.objectbrowser;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.stream.JsonWriter;
import org.hkijena.misa_imagej.api.MISAAttachment;
import org.hkijena.misa_imagej.api.workbench.MISAAttachmentDatabase;
import org.hkijena.misa_imagej.utils.GsonUtils;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.FileWriter;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.List;

/**
 * Responsible for threaded and cancelable saving of an Attachment
 */
public class MISAAttachmentSaverDialogUI extends JDialog {

    private Path exportPath;
    private List<MISAAttachment> attachments;
    private MISAAttachmentDatabase database;
    private List<String> databaseFilters;
    private JButton cancelButton;
    private JProgressBar progressBar;
    private Worker worker;
    private volatile boolean isDone = false;


    public MISAAttachmentSaverDialogUI(Path exportPath, MISAAttachment attachment) {
        this.exportPath = exportPath;
        this.attachments = Arrays.asList(attachment);
        initialize();
    }

    public MISAAttachmentSaverDialogUI(Path exportPath, List<MISAAttachment> attachments) {
        this.exportPath = exportPath;
        this.attachments = attachments;
        initialize();
    }

    public MISAAttachmentSaverDialogUI(Path exportPath, MISAAttachmentDatabase database, List<String> databaseFilters) {
        this.database = database;
        this.exportPath = exportPath;
        this.databaseFilters = databaseFilters;
        initialize();
    }

    private void initialize() {
        setTitle("Saving as JSON");

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
        progressBar.setIndeterminate(true);
        add(progressBar);


        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.LINE_AXIS));

        buttonPanel.add(Box.createHorizontalGlue());
        buttonPanel.add(Box.createVerticalStrut(4));

        cancelButton = new JButton("Cancel", UIUtils.getIconFromResources("remove.png"));
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
        worker = new Worker(exportPath, attachments, database, databaseFilters);
        worker.getEventBus().register(this);
        worker.execute();
    }

    @Subscribe
    public void handleProgressEvent(ProgressEvent event) {
        progressBar.setValue(event.getProgress());
        progressBar.setString(event.getProgress() + " objects written");
    }

    @Subscribe
    public void handleWorkDoneEvent(WorkDoneEvent event) {
        worker = null;
        setVisible(false);
        isDone = true;
    }

    private static class Worker extends SwingWorker {

        private Path exportPath;
        private EventBus eventBus = new EventBus();

        private List<MISAAttachment> attachments;
        private MISAAttachmentDatabase database;
        private List<String> databaseFilters;

        private Worker(Path exportPath, List<MISAAttachment> attachments, MISAAttachmentDatabase database, List<String> databaseFilters) {
            this.exportPath = exportPath;
            this.attachments = attachments;
            this.database = database;
            this.databaseFilters = databaseFilters;
        }

        @Override
        protected Object doInBackground() throws Exception {
            Gson gson = GsonUtils.getGson();
            try(JsonWriter writer = new JsonWriter(new FileWriter(exportPath.toFile()))) {
                writer.setIndent("    ");
                writer.setSerializeNulls(true);
                writer.beginObject();
                int total = 0;
                try (MISAAttachmentDatabase.Iterator iterator = database.createAttachmentIterator(databaseFilters)) {
                    MISAAttachment attachment;
                    while((attachment = iterator.nextAttachment()) != null) {
                        writer.name(attachment.getAttachmentFullPath());
                        gson.toJson(attachment.getFullJson(), JsonObject.class, writer);
                        // Update progress
                        ++total;
                        int finalTotal = total;
                        SwingUtilities.invokeLater(() -> eventBus.post(new ProgressEvent(finalTotal)));
                    }
                    writer.endObject();
                }
            }

            return null;
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

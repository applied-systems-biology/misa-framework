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
import org.hkijena.misa_imagej.api.MISAAttachment;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Arrays;
import java.util.List;

/**
 * Responsible for threaded and cancelable expansion of a MISAAttachment instance
 */
public class MISAAttachmentExpanderDialogUI extends JDialog {

    private List<MISAAttachment> attachments;
    private JButton cancelButton;
    private JButton acceptButton;
    private JProgressBar progressBar;
    private Worker worker;
    private JTextArea warningLabel;
    private volatile boolean isDone = false;

    public MISAAttachmentExpanderDialogUI(MISAAttachment attachment) {
        this.attachments = Arrays.asList(attachment);
        initialize();
    }

    public MISAAttachmentExpanderDialogUI(List<MISAAttachment> attachments) {
        this.attachments = attachments;
        initialize();
    }

    private void initialize() {
        setTitle("Loading database entries");

        setLayout(new BoxLayout(getContentPane(), BoxLayout.PAGE_AXIS));
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);

        warningLabel = new JTextArea();
        warningLabel.setBorder(null);
        warningLabel.setLineWrap(true);
        warningLabel.setWrapStyleWord(true);
        warningLabel.setEditable(false);
        warningLabel.setVisible(false);
        add(warningLabel);

        add(Box.createVerticalGlue());

        progressBar = new JProgressBar();
        progressBar.setStringPainted(true);
        progressBar.setString("- / -");
        add(progressBar);


        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.LINE_AXIS));

        buttonPanel.add(Box.createVerticalStrut(4));
        buttonPanel.add(Box.createHorizontalGlue());

        cancelButton = new JButton("Cancel", UIUtils.getIconFromResources("remove.png"));
        cancelButton.addActionListener(e -> cancelOperation());
        buttonPanel.add(cancelButton);

        acceptButton = new JButton("Load anyways", UIUtils.getIconFromResources("database.png"));
        acceptButton.addActionListener(e -> acceptOperation());
        acceptButton.setVisible(false);
        buttonPanel.add(acceptButton);

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

    private void acceptOperation() {
        for(MISAAttachment attachment : attachments) {
            attachment.stopLoadAllIteration(false);
        }
        setVisible(false);
        isDone = true;
    }

    private void cancelOperation() {
        if(worker != null) {
            worker.cancel(false);
        }
        else {
            for(MISAAttachment attachment : attachments) {
                attachment.stopLoadAllIteration(true);
            }
            setVisible(false);
            isDone = true;
        }
    }

    public void startOperation() {
        for(MISAAttachment attachment : attachments) {
            attachment.startLoadAllIteration();
        }
        worker = new Worker(attachments);
        worker.getEventBus().register(this);
        worker.execute();
    }

    @Subscribe
    public void handleProgressEvent(ProgressEvent event) {
        progressBar.setMaximum(event.getMaximum());
        progressBar.setValue(event.getProgress());
        progressBar.setString(event.getProgress() + " / " + event.getMaximum());
    }

    @Subscribe
    public void handleWorkDoneEvent(WorkDoneEvent event) {
        worker = null;

        if(event.isCanceled())
            cancelOperation();

        progressBar.setVisible(false);

        int properties = 0;
        for(MISAAttachment attachment : attachments) {
            properties += attachment.getProperties().size();
        }

        if(properties > 1000) {
            acceptButton.setVisible(true);
            warningLabel.setText("There are " + properties + " properties that will be displayed. " +
                    "If there are too many loaded properties, the application might slow down or crash.\n\n" +
                    "Do you really want to continue?");
            warningLabel.setVisible(true);
        }
        else {
            acceptOperation();
        }
    }

    private static class Worker extends SwingWorker {

        private EventBus eventBus = new EventBus();

        private List<MISAAttachment> attachments;

        private Worker(List<MISAAttachment> attachments) {
            this.attachments = attachments;
        }

        @Override
        protected Object doInBackground() throws Exception {
            int globalTotal = 0;

            for(MISAAttachment attachment : attachments) {
                int iteration = 0;
                while(!isCancelled() && attachment.doLoadAllIteration()) {
                    if(iteration % 100 == 0) {
                        final int unloaded = attachment.getUnloadedProperties().size();
                        final int progress = attachment.getProperties().size() - unloaded + globalTotal;
                        final int total = attachment.getProperties().size() + globalTotal;
                        SwingUtilities.invokeLater(() -> {
                            eventBus.post(new ProgressEvent(progress, total));
                        });
                    }
                    ++iteration;
                }
                globalTotal += attachment.getProperties().size();
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
        private int maximum;

        private ProgressEvent(int progress, int maximum) {
            this.progress = progress;
            this.maximum = maximum;
        }

        public int getProgress() {
            return progress;
        }

        public int getMaximum() {
            return maximum;
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

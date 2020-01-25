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

package org.hkijena.misa_imagej.ui.components;

import org.apache.commons.exec.*;
import org.hkijena.misa_imagej.ui.repository.MISAModuleRepositoryUI;

import javax.swing.*;
import java.awt.*;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.util.List;
import java.util.concurrent.CancellationException;
import java.util.concurrent.ExecutionException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CancelableProcessUI extends JDialog {

    public enum Status {
        Ready,
        Running,
        Done,
        Canceled,
        Failed
    }

    private Worker worker;
    private PropertyChangeSupport propertyChangeSupport = new PropertyChangeSupport(this);
    private Status status = Status.Ready;
    private JTextArea statusLabel;
    private JProgressBar currentTaskProgress;
    private JProgressBar processProgress;
    private int currentTask = 0;

    private static final Pattern percentagePattern = Pattern.compile(".*<(\\d+) / (\\d+)>.*");

    public CancelableProcessUI(List<CommandLine> processes) {
        setTitle("Working ...");
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        setSize(500, 400);
        setLayout(new BorderLayout(8, 8));

        JPanel centerPanel = new JPanel(new BorderLayout(8,8));

        statusLabel = new JTextArea("Please wait ...");
        statusLabel.setEditable(false);
        statusLabel.setBorder(null);
        statusLabel.setLineWrap(true);
        statusLabel.setWrapStyleWord(true);
        statusLabel.setOpaque(false);
        centerPanel.add(statusLabel, BorderLayout.CENTER);

        JPanel progressPanel = new JPanel();
        progressPanel.setLayout(new BoxLayout(progressPanel, BoxLayout.PAGE_AXIS));

        currentTaskProgress = new JProgressBar();
        currentTaskProgress.setIndeterminate(true);
        progressPanel.add(currentTaskProgress);

        processProgress = new JProgressBar();
        processProgress.setMaximum(processes.size());
        processProgress.setValue(0);
        processProgress.setString("0 / " + processes.size());
        processProgress.setStringPainted(true);
        progressPanel.add(processProgress);

        centerPanel.add(progressPanel, BorderLayout.SOUTH);
        add(centerPanel, BorderLayout.CENTER);

        worker = new Worker(this, processes);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.LINE_AXIS));
        buttonPanel.add(Box.createHorizontalGlue());

        JButton cancelButton = new JButton("Cancel");
        buttonPanel.add(cancelButton);
        cancelButton.addActionListener(actionEvent -> {
            worker.cancel(true);
            worker.cancelCurrentProcess();
        } );

        add(buttonPanel, BorderLayout.SOUTH);
    }

    public void start() {
        if(getStatus() != Status.Ready)
            throw new RuntimeException("Worker is not ready!");
        setStatus(Status.Running);
        setModal(false);
        pack();
        setSize(500,400);
        setVisible(true);
    }

    public Status getStatus() {
        return status;
    }

    private void setStatus(Status status) {
        Status old = this.status;
        this.status = status;
        propertyChangeSupport.firePropertyChange("status", old, status);
        if(status == Status.Canceled || status == Status.Done || status == Status.Failed) {
            setVisible(false);
            dispose();
        }
        else if(status == Status.Running) {
            worker.execute();
        }
    }

    public void addPropertyChangeListener( PropertyChangeListener l )
    {
        propertyChangeSupport.addPropertyChangeListener( l );
    }

    public void removePropertyChangeListener( PropertyChangeListener l )
    {
        propertyChangeSupport.removePropertyChangeListener( l );
    }

    private void updateProgressAndStatus(String stdout, int currentProcess, int numProcesses) {
        statusLabel.setText(stdout.replace("\t", "  "));

        processProgress.setValue(currentProcess);
        processProgress.setString(currentProcess + " / " + numProcesses);

        if(currentTask != currentProcess) {
            currentTaskProgress.setMaximum(100);
            currentTaskProgress.setValue(0);
            currentTaskProgress.setIndeterminate(true);
        }

        Matcher percentageMatch = percentagePattern.matcher(stdout.trim());
        if(percentageMatch.matches()) {
            try {
                int progress = Integer.parseInt(percentageMatch.group(1));
                int total = Integer.parseInt(percentageMatch.group(2));
                currentTaskProgress.setIndeterminate(false);
                currentTaskProgress.setMaximum(total);
                currentTaskProgress.setValue(progress);
            }
            catch(NumberFormatException e) {
            }
        }

    }

    public class Worker extends SwingWorker<Integer, Object> {

        private final List<CommandLine> processes;
        private volatile DefaultExecutor executor;
        private volatile int currentProcessIndex;
        private final CancelableProcessUI ui;

        public Worker(CancelableProcessUI ui, List<CommandLine> processes) {
            this.ui = ui;
            this.processes = processes;

            // Setup the executor
            this.executor = new DefaultExecutor();
            this.executor.setWatchdog(new ExecuteWatchdog(ExecuteWatchdog.INFINITE_TIMEOUT));

            LogOutputStream stdOutputStream = new LogOutputStream() {
                @Override
                protected void processLine(String s, int i) {
                    MISAModuleRepositoryUI.getInstance().getCommand().getLogService().info(s);
                    SwingUtilities.invokeLater(() -> {
                        ui.updateProgressAndStatus(s, currentProcessIndex, processes.size());
                    });
                }
            };
            LogOutputStream stdErrorStream = new LogOutputStream() {
                @Override
                protected void processLine(String s, int i) {
                    MISAModuleRepositoryUI.getInstance().getCommand().getLogService().error(s);
                }
            };
            this.executor.setStreamHandler(new PumpStreamHandler(stdOutputStream, stdErrorStream));
        }

        @Override
        protected Integer doInBackground() throws Exception {
            for(int i = 0; i < processes.size(); ++i) {
                if(this.isCancelled())
                    return 1;
                currentProcessIndex = i;
                int result = executor.execute(processes.get(i));
                if(executor.isFailure(result))
                    return result;
            }
            return 0;
        }

        @Override
        protected void done() {
            try {
                if(get() == 0) {
                    setStatus(Status.Done);
                }
                else {
                    setStatus(Status.Failed);
                }
            }
            catch (InterruptedException | ExecutionException | CancellationException e) {
                setStatus(Status.Canceled);
            }
        }

        public void cancelCurrentProcess() {
            executor.getWatchdog().destroyProcess();
        }
    }
}

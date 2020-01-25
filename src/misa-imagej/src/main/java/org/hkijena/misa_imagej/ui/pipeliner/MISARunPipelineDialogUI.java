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

package org.hkijena.misa_imagej.ui.pipeliner;

import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.FileSelection;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class MISARunPipelineDialogUI extends JDialog {

    public static final int ACCEPT_OPTION = 0;
    public static final int REJECT_OPTION = 1;

    private FileSelection exportPath;
    private boolean dialogOK = false;

    public MISARunPipelineDialogUI(Frame parent) {
        super(parent, "Run MISA++ pipeline");
        initialize();
        generatePaths();
    }

    private void generatePaths() {
        try {
            exportPath.setPath(Files.createTempDirectory("ImageJMISA"));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private void initialize() {
        setIconImage(UIUtils.getIconFromResources("misaxx.png").getImage());
        setSize(500, 400);
        getContentPane().setLayout(new BorderLayout(8, 8));

        JPanel formPanel = new JPanel();
        formPanel.setLayout(new GridBagLayout());

        exportPath = new FileSelection();
        exportPath.getFileChooser().setDialogTitle("Select temporary path for exported data");
        exportPath.getFileChooser().setMultiSelectionEnabled(false);
        exportPath.getFileChooser().setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

        JTextArea info = new JTextArea();
        info.setEditable(false);
        info.setText("To run the pipeline, data must be written into temporary directories. " +
                "You might need to change the input and output directories if your data set is very large.");
        info.setLineWrap(true);
        info.setWrapStyleWord(true);

        formPanel.add(info, new GridBagConstraints() {
            {
                anchor = GridBagConstraints.PAGE_START;
                gridx = 0;
                gridy = 0;
                fill = GridBagConstraints.HORIZONTAL;
                gridwidth = 2;
                insets = UIUtils.UI_PADDING;
            }
        });

        UIUtils.createDescriptionLabelUI(formPanel, "Export path", 4, 0);
        formPanel.add(exportPath, new GridBagConstraints() {
            {
                anchor = GridBagConstraints.PAGE_START;
                gridx = 1;
                gridy = 4;
                fill = GridBagConstraints.HORIZONTAL;
                weightx = 1;
                insets = UIUtils.UI_PADDING;
            }
        });

        UIUtils.addFillerGridBagComponent(formPanel, 5);

        formPanel.setBorder(BorderFactory.createTitledBorder("Temporary files for calculation"));
        add(formPanel, BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.LINE_AXIS));
        buttonPanel.add(Box.createHorizontalGlue());
        add(buttonPanel, BorderLayout.SOUTH);

        JButton exportButton = new JButton("Cancel");
        exportButton.addActionListener(actionEvent -> setVisible(false));
        buttonPanel.add(exportButton);

        JButton runButton = new JButton("Run now");
        runButton.addActionListener(actionEvent -> acceptOption());
        buttonPanel.add(runButton);
    }

    private void acceptOption() {
        dialogOK = true;
        setVisible(false);
    }

    public int showDialog() {
        setModal(true);
        pack();
        setSize(new Dimension(500,400));
        setVisible(true);
        if(dialogOK)
            return ACCEPT_OPTION;
        else
            return REJECT_OPTION;
    }

    public Path getExportPath() {
        return exportPath.getPath();
    }
}

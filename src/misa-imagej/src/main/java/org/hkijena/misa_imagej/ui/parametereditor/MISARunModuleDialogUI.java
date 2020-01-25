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

package org.hkijena.misa_imagej.ui.parametereditor;

import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.FileSelection;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class MISARunModuleDialogUI extends JDialog {

    public static final int ACCEPT_OPTION = 0;
    public static final int REJECT_OPTION = 1;

    private FileSelection parameterFilePath;
    private FileSelection importedPath;
    private FileSelection exportedPath;
    private boolean dialogOK = false;

    public MISARunModuleDialogUI(Frame parent) {
        super(parent, "Run MISA++ workload");
        initialize();
        generatePaths();
    }

    private void generatePaths() {
        try {
            Path tmppath = Files.createTempDirectory("ImageJMISA");
            parameterFilePath.setPath(tmppath.resolve("parameters.json"));
            importedPath.setPath(tmppath.resolve("imported"));
            exportedPath.setPath(tmppath.resolve("exported"));
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

        parameterFilePath = new FileSelection();
        parameterFilePath.getFileChooser().setDialogTitle("Select parameter file path");
        parameterFilePath.getFileChooser().setMultiSelectionEnabled(false);
        parameterFilePath.getFileChooser().setFileSelectionMode(JFileChooser.FILES_ONLY);

        importedPath = new FileSelection();
        importedPath.getFileChooser().setDialogTitle("Select temporary path for imported data");
        importedPath.getFileChooser().setMultiSelectionEnabled(false);
        importedPath.getFileChooser().setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

        exportedPath = new FileSelection();
        exportedPath.getFileChooser().setDialogTitle("Select temporary path for exported data");
        exportedPath.getFileChooser().setMultiSelectionEnabled(false);
        exportedPath.getFileChooser().setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

        JTextArea info = new JTextArea();
        info.setEditable(false);
        info.setText("To run the workload, data must be written into temporary directories. " +
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

        UIUtils.createDescriptionLabelUI(formPanel, "Parameter file", 1, 0);
        formPanel.add(parameterFilePath, new GridBagConstraints() {
            {
                anchor = GridBagConstraints.PAGE_START;
                gridx = 1;
                gridy = 1;
                fill = GridBagConstraints.HORIZONTAL;
                weightx = 1;
                insets = UIUtils.UI_PADDING;
            }
        });

        UIUtils.createDescriptionLabelUI(formPanel, "Input data", 3, 0);
        formPanel.add(importedPath, new GridBagConstraints() {
            {
                anchor = GridBagConstraints.PAGE_START;
                gridx = 1;
                gridy = 3;
                fill = GridBagConstraints.HORIZONTAL;
                weightx = 1;
                insets = UIUtils.UI_PADDING;
            }
        });

        UIUtils.createDescriptionLabelUI(formPanel, "Output data", 4, 0);
        formPanel.add(exportedPath, new GridBagConstraints() {
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
        pack();
        setSize(500, 400);
        setModal(true);
        setVisible(true);
        if(dialogOK)
            return ACCEPT_OPTION;
        else
            return REJECT_OPTION;
    }

    public Path getParameterFilePath() {
        return parameterFilePath.getPath();
    }

    public Path getImportedPath() {
        return importedPath.getPath();
    }

    public Path getExportedPath() {
        return exportedPath.getPath();
    }
}

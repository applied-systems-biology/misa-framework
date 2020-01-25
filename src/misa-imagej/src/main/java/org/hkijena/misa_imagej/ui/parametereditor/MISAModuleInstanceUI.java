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

import org.apache.commons.exec.ExecuteException;
import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISAModuleInstance;
import org.hkijena.misa_imagej.api.MISASamplePolicy;
import org.hkijena.misa_imagej.api.MISAValidityReport;
import org.hkijena.misa_imagej.extension.datasources.MISAFolderLinkDataSource;
import org.hkijena.misa_imagej.ui.components.CancelableProcessUI;
import org.hkijena.misa_imagej.ui.components.MISAValidityReportStatusUI;
import org.hkijena.misa_imagej.ui.components.MarkdownReader;
import org.hkijena.misa_imagej.ui.components.PDFReader;
import org.hkijena.misa_imagej.ui.repository.MISAModuleRepositoryUI;
import org.hkijena.misa_imagej.ui.workbench.MISAWorkbenchUI;
import org.hkijena.misa_imagej.utils.BusyCursor;
import org.hkijena.misa_imagej.utils.FilesystemUtils;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.DocumentTabPane;
import org.jdesktop.swingx.JXStatusBar;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.nio.file.FileVisitOption;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;

public class MISAModuleInstanceUI extends JFrame {



    private org.hkijena.misa_imagej.api.MISAModuleInstance moduleInstance;

    private MISAValidityReportStatusUI validityReportStatusUI;
//    private JComboBox<MISASample> sampleList;

    /**
     * Create the dialog.
     */
    public MISAModuleInstanceUI(org.hkijena.misa_imagej.api.MISAModuleInstance moduleInstance, boolean editOnlyMode, boolean addDefaultSample) {
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        this.moduleInstance = moduleInstance;
        initialize(editOnlyMode);

        // Create a new sample if necessary
        if(moduleInstance.getSamples().size() == 0 && addDefaultSample)
            this.moduleInstance.addSample("New Sample");
    }

    private void install(Path parameters, Path importedDirectory, Path exportedDirectory, boolean forceCopy, boolean relativeDirectories) {
        setEnabled(false);
        this.moduleInstance.install(parameters, importedDirectory, exportedDirectory, forceCopy, relativeDirectories);
        setEnabled(true);
    }

    private boolean parametersAreValid() {
        MISAValidityReport report = moduleInstance.getValidityReport();
        validityReportStatusUI.setReport(report);
        return report.isValid();
    }

    /**
     * Exports the current settings into a user-selected folder
     */
    private void exportMISARun() {

       if(!parametersAreValid())
           return;

        JFileChooser chooser = new JFileChooser();
        chooser.setDialogTitle("Open folder");
        chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        chooser.setMultiSelectionEnabled(false);
        chooser.setAcceptAllFileFilterUsed(false);

        if (chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            try (BusyCursor busyCursor = new BusyCursor(this)) {
                Path exportRunPath = chooser.getSelectedFile().toPath();

                if (!FilesystemUtils.directoryIsEmpty(exportRunPath)) {
                    JOptionPane.showMessageDialog(this,
                            "The directory " + exportRunPath.toString() + " must be empty!",
                            "Export run",
                            JOptionPane.ERROR_MESSAGE);
                    return;
                }

                Path importedPath = exportRunPath.resolve("imported");
                Path exportedPath = exportRunPath.resolve("exported");
                Files.createDirectories(exportRunPath);

                // Write the parameter schema
                install(exportRunPath.resolve("parameters.json"),
                        importedPath,
                        exportedPath,
                        true,
                        true);

                if (JOptionPane.showConfirmDialog(this,
                        "Export successful. Do you want to open the output directory?",
                        "Export run", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
                    Desktop.getDesktop().open(exportRunPath.toFile());
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void runMISA() {

        if(!parametersAreValid())
            return;

        MISARunModuleDialogUI dialog = new MISARunModuleDialogUI(this);
        dialog.setLocationRelativeTo(this);
        if (dialog.showDialog() == MISARunModuleDialogUI.ACCEPT_OPTION) {
            try (BusyCursor busyCursor = new BusyCursor(this)) {
                Files.createDirectories(dialog.getImportedPath());
                Files.createDirectories(dialog.getExportedPath());
                Files.createDirectories(dialog.getParameterFilePath().getParent());

                // Write the parameter schema
                install(dialog.getParameterFilePath(), dialog.getImportedPath(), dialog.getExportedPath(), false, false);

                // Run the executable
                MISAModuleRepositoryUI.getInstance().getCommand().getLogService().info("Starting worker process ...");
                CancelableProcessUI processUI = new CancelableProcessUI(Arrays.asList(getModuleInstance().getModule().run(dialog.getParameterFilePath())));
                processUI.setLocationRelativeTo(this);

                // React to changes in status
                processUI.addPropertyChangeListener(propertyChangeEvent -> {
                    if(processUI.getStatus() == CancelableProcessUI.Status.Done) {
                        setEnabled(true);
                        if(JOptionPane.showConfirmDialog(this, "The calculated finished. Do you want to analyze the results?",
                                "Calculation finished", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE) == JOptionPane.YES_OPTION) {
                            MISAWorkbenchUI ui = new MISAWorkbenchUI();
                            ui.setVisible(true);
                            ui.open(dialog.getExportedPath());
                            ui.pack();
                            ui.setSize(new Dimension(800,600));
                            ui.setExtendedState(ui.getExtendedState() | JFrame.MAXIMIZED_BOTH);
                        }
                    }
                    else if(processUI.getStatus() == CancelableProcessUI.Status.Failed ||
                            processUI.getStatus() == CancelableProcessUI.Status.Canceled) {
                        setEnabled(true);
                        if(processUI.getStatus() == CancelableProcessUI.Status.Failed) {
                            JOptionPane.showMessageDialog(this,
                                    "There was an error during calculation. Please check the console to see the cause of this error.",
                                    "Error",
                                    JOptionPane.ERROR_MESSAGE);
                        }
                    }
                });

                setEnabled(false);
                processUI.start();

            } catch (Exception e) {
                e.printStackTrace();
                JOptionPane.showMessageDialog(this, "There was an error during setting up the analysis. Please take a look at the console to find out more.",
                        "Error", JOptionPane.ERROR_MESSAGE);
                setEnabled(true);
            }
        }
    }

    private void initialize(boolean editOnlyMode) {
        setSize(800, 600);
        getContentPane().setLayout(new BorderLayout(8, 8));
        setTitle("MISA++ for ImageJ - " + getModuleInstance().getModuleInfo().toString());
        setIconImage(UIUtils.getIconFromResources("misaxx.png").getImage());
        if(!editOnlyMode)
            UIUtils.setToAskOnClose(this, "Do you really want to close this parameter editor?", "Close window");

        MISASampleManagerUI sampleManagerUI = new MISASampleManagerUI(getModuleInstance());
        MISASampleParametersUI sampleParametersEditorUI = new MISASampleParametersUI(getModuleInstance());
        AlgorithmParametersEditorUI algorithmParametersEditorUI = new AlgorithmParametersEditorUI(getModuleInstance());
        MISARuntimeParametersUI MISARuntimeParametersUI = new MISARuntimeParametersUI(getModuleInstance());
        MISASampleCachesUI MISASampleCachesUI = new MISASampleCachesUI(getModuleInstance());

        // Tabs with settings
        DocumentTabPane tabbedPane = new DocumentTabPane();
        tabbedPane.addTab("Samples", UIUtils.getIconFromResources("sample.png"), sampleManagerUI, DocumentTabPane.CloseMode.withoutCloseButton);
        tabbedPane.addTab("Data", UIUtils.getIconFromResources("database.png"), MISASampleCachesUI, DocumentTabPane.CloseMode.withoutCloseButton);
        tabbedPane.addTab("Sample parameters", UIUtils.getIconFromResources("edit.png"), sampleParametersEditorUI, DocumentTabPane.CloseMode.withoutCloseButton);
        tabbedPane.addTab("Algorithm parameters", UIUtils.getIconFromResources("edit.png"), algorithmParametersEditorUI, DocumentTabPane.CloseMode.withoutCloseButton);
        tabbedPane.addTab("Runtime", UIUtils.getIconFromResources("cog.png"), MISARuntimeParametersUI, DocumentTabPane.CloseMode.withoutCloseButton);

        // Documentation tabs
        tabbedPane.addSingletonTab("MODULE_HELP", "Module documentation", UIUtils.getIconFromResources("help.png"), new MarkdownReader() {
            {
                setMarkdown(moduleInstance.getModule().getREADME());
            }
        }, true);
        tabbedPane.addSingletonTab("HELP", "Documentation", UIUtils.getIconFromResources("help.png"),
                PDFReader.fromResource("documentation/parameter-editor.pdf"), true);

        add(tabbedPane, BorderLayout.CENTER);

        // Toolbar
        JToolBar toolBar = new JToolBar();

        JButton importParametersButton = new JButton("Import parameters", UIUtils.getIconFromResources("open.png"));
        importParametersButton.addActionListener(e -> importParameters());
        toolBar.add(importParametersButton);

        JButton importFolderButton = new JButton("Import folder", UIUtils.getIconFromResources("open.png"));
        importFolderButton.addActionListener(e -> importFolder());
        toolBar.add(importFolderButton);

        toolBar.add(Box.createHorizontalGlue());

        JButton validateButton = new JButton("Check parameters", UIUtils.getIconFromResources("checkmark.png"));
        validateButton.addActionListener(actionEvent -> parametersAreValid());
        toolBar.add(validateButton);

        if(!editOnlyMode) {
            JButton exportButton = new JButton("Export", UIUtils.getIconFromResources("export.png"));
            exportButton.setToolTipText("Instead of running the MISA++ module, export all necessary files into a folder. This folder for example can be put onto a server.");
            exportButton.addActionListener(actionEvent -> exportMISARun());
            toolBar.add(exportButton);

            JButton runButton = new JButton("Run", UIUtils.getIconFromResources("run.png"));
            runButton.setToolTipText("Runs the MISA++ module.");
            runButton.addActionListener(actionEvent -> runMISA());
            toolBar.add(runButton);
        }

        JButton helpButton = new JButton(UIUtils.getIconFromResources("help.png"));
        JPopupMenu helpButtonMenu = UIUtils.addPopupMenuToComponent(helpButton);
        {
            JMenuItem moduleHelpButton = new JMenuItem("Module documentation", UIUtils.getIconFromResources("module.png"));
            moduleHelpButton.addActionListener(e -> tabbedPane.selectSingletonTab("MODULE_HELP"));
            helpButtonMenu.add(moduleHelpButton);

            JMenuItem documentationButton = new JMenuItem("Documentation", UIUtils.getIconFromResources("help.png"));
            documentationButton.addActionListener(e -> tabbedPane.selectSingletonTab("HELP"));
            helpButtonMenu.add(documentationButton);
        }
        toolBar.add(helpButton);

        add(toolBar, BorderLayout.NORTH);

        // Status bar
        JXStatusBar statusBar = new JXStatusBar();
        validityReportStatusUI = new MISAValidityReportStatusUI();
        statusBar.add(validityReportStatusUI);
        add(statusBar, BorderLayout.SOUTH);
    }

    private void importFolder() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        fileChooser.setDialogTitle("Import directory structure");
        if(fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            boolean foundParameters = false;
            try {
                Path rootPath = fileChooser.getSelectedFile().toPath();
                if(Files.exists(rootPath.resolve("parameters.json"))) {
                    moduleInstance.loadParameters(fileChooser.getSelectedFile().toPath(), MISASamplePolicy.createMissingSamples);
                    foundParameters = true;
                }
            }
            catch(IOException e) {
            }
            try {
                Path rootPath = fileChooser.getSelectedFile().toPath();
                final boolean finalFoundParameters = foundParameters;
                Files.walk(rootPath, 1, FileVisitOption.FOLLOW_LINKS).filter(Files::isDirectory).forEach(absoluteSamplePath -> {
                    if(rootPath.equals(absoluteSamplePath))
                        return;
                    String sample = absoluteSamplePath.getFileName().toString();

                    // Create a new sample if applicable
                    if(!finalFoundParameters)
                        moduleInstance.addSample(sample);

                    // Look for a cache with matching internal path
                    for(MISACache cache : moduleInstance.getSample(sample).getImportedCaches()) {
                        Path absoluteCachePath = absoluteSamplePath.resolve(cache.getRelativePath());
                        if(Files.isDirectory(absoluteCachePath)) {
                            MISAFolderLinkDataSource dataSource = cache.getDataSourceByType(MISAFolderLinkDataSource.class);
                            dataSource.setSourceFolder(absoluteCachePath);
                            cache.setDataSource(dataSource);
                        }
                    }
                });
            }
            catch(IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    private void importParameters() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        fileChooser.setDialogTitle("Import parameter file");
        if(fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            try {
                moduleInstance.loadParameters(fileChooser.getSelectedFile().toPath(), MISASamplePolicy.createMissingSamples);
            }
            catch(IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    public MISAModuleInstance getModuleInstance() {
        return moduleInstance;
    }

}

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

package org.hkijena.misa_imagej.ui.workbench;

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISAModuleInstance;
import org.hkijena.misa_imagej.api.repository.MISAModule;
import org.hkijena.misa_imagej.api.repository.MISAModuleRepository;
import org.hkijena.misa_imagej.api.workbench.MISAOutput;
import org.hkijena.misa_imagej.extension.datasources.MISAFolderLinkDataSource;
import org.hkijena.misa_imagej.ui.components.CancelableProcessUI;
import org.hkijena.misa_imagej.ui.components.MemoryStatusUI;
import org.hkijena.misa_imagej.ui.components.PDFReader;
import org.hkijena.misa_imagej.ui.perfanalysis.MISARuntimeLogUI;
import org.hkijena.misa_imagej.ui.workbench.objectbrowser.MISAAttachmentBrowserUI;
import org.hkijena.misa_imagej.ui.workbench.tableanalyzer.MISATableImporterDialog;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.DocumentTabPane;
import org.jdesktop.swingx.JXStatusBar;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MISAWorkbenchUI extends JFrame{

    private JLabel statusLabel;
    private MISAOutput misaOutput;

    private DocumentTabPane documentTabPane;
    private MISACacheBrowserUI cacheBrowserUI;
    private MISARuntimeLogUI runtimeLogUI;
    private List<MISAAttachmentBrowserUI> attachmentBrowserUIList = new ArrayList<>();

    public MISAWorkbenchUI() {
        initialize();
    }

    private void initialize() {
        setSize(800, 600);
        getContentPane().setLayout(new BorderLayout(8, 8));
        setTitle("MISA++ Workbench for ImageJ");
        setIconImage(UIUtils.getIconFromResources("misaxx.png").getImage());
        UIUtils.setToAskOnClose(this, "Do you really want to close this analysis tool?", "Close window");

        cacheBrowserUI = new MISACacheBrowserUI();
        runtimeLogUI = new MISARuntimeLogUI();
        runtimeLogUI.setHideOpenButton(true);

        documentTabPane = new DocumentTabPane();
        documentTabPane.addSingletonTab("DATA_BROWSER", "Data browser", UIUtils.getIconFromResources("database.png"), cacheBrowserUI, false);
        documentTabPane.addSingletonTab("RUNTIME_LOG", "Runtime log",  UIUtils.getIconFromResources("clock.png"), runtimeLogUI, true);

        documentTabPane.addSingletonTab("HELP", "Documentation", UIUtils.getIconFromResources("help.png"),
                PDFReader.fromResource("documentation/workbench.pdf"), true);

        add(documentTabPane, BorderLayout.CENTER);

        initializeToolbar();

        initializeStatusBar();
        updateUI();
    }

    private void initializeToolbar() {
        JToolBar toolBar = new JToolBar();

        JButton openButton = new JButton("Open ...", UIUtils.getIconFromResources("open.png"));
        openButton.addActionListener(actionEvent -> open());
        toolBar.add(openButton);

        JButton importTableButton = new JButton("Import table", UIUtils.getIconFromResources("table.png"));
        importTableButton.addActionListener(e -> importTable());
        toolBar.add(importTableButton);

        toolBar.addSeparator();

        JButton openDataBrowserButton = new JButton("Browse data", UIUtils.getIconFromResources("database.png"));
        openDataBrowserButton.addActionListener(e -> documentTabPane.selectSingletonTab("DATA_BROWSER"));
        toolBar.add(openDataBrowserButton);

        JButton createAttachmentBrowserButton = new JButton("Browse quantification results", UIUtils.getIconFromResources("graph.png"));
        createAttachmentBrowserButton.addActionListener(e -> openAttachmentBrowserTab());
        toolBar.add(createAttachmentBrowserButton);

        JButton openRuntimeLogButton = new JButton("Analyze runtime",  UIUtils.getIconFromResources("clock.png"));
        openRuntimeLogButton.addActionListener(e -> {
            documentTabPane.selectSingletonTab("RUNTIME_LOG");
            runtimeLogUI.open(misaOutput.getRuntimeLog());
        });
        toolBar.add(openRuntimeLogButton);

        toolBar.add(Box.createHorizontalGlue());

        JButton helpButton = new JButton(UIUtils.getIconFromResources("help.png"));
        helpButton.addActionListener(e -> documentTabPane.selectSingletonTab("HELP"));
        toolBar.add(helpButton);

        add(toolBar, BorderLayout.NORTH);
    }

    private void importTable() {
        MISATableImporterDialog dialog = new MISATableImporterDialog(this);
        dialog.pack();
        dialog.setSize(400,300);
        dialog.setLocationRelativeTo(SwingUtilities.getWindowAncestor(this));
        dialog.setModal(true);
        dialog.setVisible(true);
    }

    private void openAttachmentBrowserTab() {
        if(misaOutput == null)
            return;
        if(!misaOutput.hasAttachmentIndex()) {
            MISAModule module = MISAModuleRepository.getInstance().getModule("misaxx-analyzer");
            if(module == null) {
                JOptionPane.showMessageDialog(this, "Please make sure that the 'MISA++ Result Analyzer' module is installed.",
                        "Unable to analyze quantification results", JOptionPane.ERROR_MESSAGE);
                return;
            }

            setEnabled(false);

            // Generate temporary "output"
            try {
                Path tmp = Files.createTempDirectory("misaxx-workbench");
                Files.createDirectories(tmp.resolve("input"));
                MISAModuleInstance moduleInstance = module.instantiate();
                moduleInstance.getRuntimeParameters().getPropertyFromPath("request-skipping").setValue(true);
                moduleInstance.addSample("misaxx-output");

                MISACache ioCache = moduleInstance.getSample("misaxx-output").getImportedCaches().get(0);
                MISAFolderLinkDataSource ioCacheDataSource = ioCache.getDataSourceByType(MISAFolderLinkDataSource.class);
                ioCacheDataSource.setSourceFolder(misaOutput.getRootPath());
                ioCache.setDataSource(ioCacheDataSource);

                moduleInstance.install(tmp.resolve("parameters.json"), tmp.resolve("input"), tmp.resolve("output"), false, false);

                CancelableProcessUI processUI = new CancelableProcessUI(Arrays.asList(module.run(tmp.resolve("parameters.json"))));
                processUI.setLocationRelativeTo(this);

                // React to changes in status
                processUI.addPropertyChangeListener(propertyChangeEvent -> {
                    if(processUI.getStatus() == CancelableProcessUI.Status.Done) {
                        setEnabled(true);
                        openAttachmentBrowserTab();
                    }
                    else if(processUI.getStatus() == CancelableProcessUI.Status.Failed ||
                            processUI.getStatus() == CancelableProcessUI.Status.Canceled) {
                        setEnabled(true);
                        if(processUI.getStatus() == CancelableProcessUI.Status.Failed) {
                            JOptionPane.showMessageDialog(this,
                                    "There was an error during preprocessing. Please check the console to see the cause of this error.",
                                    "Error",
                                    JOptionPane.ERROR_MESSAGE);
                        }
                    }
                });

                processUI.start();
            } catch (IOException e) {
                setEnabled(true);
                throw new RuntimeException(e);
            }
        }
        else {
            MISAAttachmentBrowserUI browserUI = new MISAAttachmentBrowserUI(this);
            documentTabPane.addTab("Attachment browser", UIUtils.getIconFromResources("attachment.png"), browserUI, DocumentTabPane.CloseMode.withAskOnCloseButton, true);
            documentTabPane.setSelectedIndex(documentTabPane.getTabCount() - 1);
        }
    }

    private void initializeStatusBar() {
        JXStatusBar statusBar = new JXStatusBar();
        statusLabel = new JLabel("Ready");
        statusBar.add(statusLabel);
        statusBar.add(Box.createHorizontalGlue(), new JXStatusBar.Constraint(JXStatusBar.Constraint.ResizeBehavior.FILL));
        statusBar.add(new MemoryStatusUI());

        add(statusBar, BorderLayout.SOUTH);
    }

    public void open(Path path) {
        setTitle("MISA++ Workbench for ImageJ");
        misaOutput = null;
        try {
            misaOutput = new MISAOutput(path);
            setTitle(misaOutput.getRootPath().toString() + " - MISA++ Workbench for ImageJ");

            cacheBrowserUI.setMisaOutput(misaOutput);
        }
        catch (IOException e) {
            throw new RuntimeException(e);
        }
        updateUI();
    }

    private void open() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setMultiSelectionEnabled(false);
        fileChooser.setDialogTitle("Open MISA++ output");
        fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        if(fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
           open(fileChooser.getSelectedFile().toPath());
        }
    }

    private void updateUI() {
    }

    public MISAOutput getMisaOutput() {
        return misaOutput;
    }

    public void addTab(String title, ImageIcon icon, Component component, DocumentTabPane.CloseMode closeMode, boolean allowRename) {
        documentTabPane.addTab(title, icon, component, closeMode, allowRename);
        documentTabPane.setSelectedIndex(documentTabPane.getTabCount() - 1);
    }

    public int getTabCount() {
        return documentTabPane.getTabCount();
    }

    public void setSelectedTab(int tabIndex) {
        documentTabPane.setSelectedIndex(tabIndex);
    }

    public List<DocumentTabPane.DocumentTab> getTabs() {
        return documentTabPane.getTabs();
    }

    public String findTabNameFor(Component component) {
        for(DocumentTabPane.DocumentTab tab : getTabs()) {
            if(tab.getContent() == component)
                return tab.getTitle();
        }
        return "Document";
    }
}

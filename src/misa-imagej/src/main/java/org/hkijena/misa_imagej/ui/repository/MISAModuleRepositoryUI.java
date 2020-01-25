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

package org.hkijena.misa_imagej.ui.repository;

import com.google.gson.Gson;
import com.google.gson.stream.JsonWriter;
import org.hkijena.misa_imagej.MISAImageJCommand;
import org.hkijena.misa_imagej.api.repository.MISAModule;
import org.hkijena.misa_imagej.api.repository.MISAModuleRepository;
import org.hkijena.misa_imagej.ui.components.MarkdownReader;
import org.hkijena.misa_imagej.ui.components.PDFReader;
import org.hkijena.misa_imagej.ui.components.renderers.MISAModuleListCellRenderer;
import org.hkijena.misa_imagej.ui.parametereditor.MISAModuleInstanceUI;
import org.hkijena.misa_imagej.ui.perfanalysis.MISARuntimeLogFrameUI;
import org.hkijena.misa_imagej.ui.pipeliner.MISAPipelinerUI;
import org.hkijena.misa_imagej.ui.workbench.MISAWorkbenchUI;
import org.hkijena.misa_imagej.utils.GsonUtils;
import org.hkijena.misa_imagej.utils.OSUtils;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import static org.hkijena.misa_imagej.utils.UIUtils.UI_PADDING;

/**
 * User interface that allows the user to manage and select MISA++ modules
 */
public class MISAModuleRepositoryUI extends JFrame {

    private MISAImageJCommand command;
    private static MISAModuleRepositoryUI instance;
    private JList<MISAModule> misaModuleJList;
    private JPanel detailPanel;

    public static MISAModuleRepositoryUI getInstance(MISAImageJCommand command) {
        if(instance == null)
            instance = new MISAModuleRepositoryUI(command);
        return instance;
    }

    public static MISAModuleRepositoryUI getInstance() {
        return instance;
    }

    private MISAModuleRepositoryUI(MISAImageJCommand command) {
        instance = this;
        this.command = command;
        initialize();
        refreshModuleList();
    }

    private void initialize() {
        setSize(800, 600);
        getContentPane().setLayout(new BorderLayout(8, 8));
        setTitle("MISA++ for ImageJ - Module manager");
        setIconImage(UIUtils.getIconFromResources("misaxx.png").getImage());

        // Toolbar
        JToolBar toolBar = new JToolBar();

        JButton refreshButton = new JButton("Refresh", UIUtils.getIconFromResources("refresh.png"));
        refreshButton.addActionListener(actionEvent -> refreshModuleList());
        toolBar.add(refreshButton);

        JButton addButton = new JButton("Add module ...", UIUtils.getIconFromResources("add.png"));
        addButton.addActionListener(actionEvent -> {
            JFileChooser fileChooser = new JFileChooser();
            fileChooser.setDialogTitle("Select the module exectuable");
            fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
            fileChooser.setMultiSelectionEnabled(false);

            if(fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
                MISAModule module = new MISAModule();
                module.setExecutablePath(Paths.get(fileChooser.getSelectedFile().getAbsolutePath()));
                module.setOperatingSystem(OSUtils.detectOperatingSystem());
                module.setOperatingSystemArchitecture(OSUtils.detectArchitecture());
                if(module.getModuleInfo() != null) {
                    Gson gson = GsonUtils.getGson();
                    try {
                        Files.createDirectories(MISAModuleRepository.USER_MODULE_PATH);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try(JsonWriter writer = new JsonWriter(new FileWriter(MISAModuleRepository.USER_MODULE_PATH.resolve(module.getGeneratedFileName() + ".json").toString()))) {
                        gson.toJson(module, MISAModule.class, writer);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    refreshModuleList();
                }
                else {
                    JOptionPane.showMessageDialog(this, fileChooser.getSelectedFile().getAbsolutePath() + " seems not to be a valid module.\nModule information could not be retrieved.", "Add module", JOptionPane.ERROR_MESSAGE);
                }
            }
        });
        toolBar.add(addButton);

        toolBar.add(Box.createHorizontalGlue());

        JButton launcherPipeliner = new JButton("Connect modules together ...", UIUtils.getIconFromResources("connect.png"));
        launcherPipeliner.addActionListener(actionEvent -> {
            MISAPipelinerUI pipelinerUI = new MISAPipelinerUI();
            pipelinerUI.setVisible(true);
            pipelinerUI.setExtendedState(pipelinerUI.getExtendedState() | JFrame.MAXIMIZED_BOTH);
        });
        toolBar.add(launcherPipeliner);

        JButton launchAnalyzer = new JButton("Analyze result ...", UIUtils.getIconFromResources("graph.png"));
        launchAnalyzer.addActionListener(actionEvent -> {
            MISAWorkbenchUI workbench = new MISAWorkbenchUI();
            workbench.pack();
            workbench.setSize(new Dimension(800,600));
            workbench.setVisible(true);
            workbench.setExtendedState(workbench.getExtendedState() | JFrame.MAXIMIZED_BOTH);
        });
        toolBar.add(launchAnalyzer);

        JButton launchRuntimeLogUI = new JButton(UIUtils.getIconFromResources("clock.png"));
        launchRuntimeLogUI.setToolTipText("Analyze runtime log");
        launchRuntimeLogUI.addActionListener(actionEvent -> {
            MISARuntimeLogFrameUI runtimeLogUI = new MISARuntimeLogFrameUI();
            runtimeLogUI.pack();
            runtimeLogUI.setSize(new Dimension(800,600));
            runtimeLogUI.setVisible(true);
            runtimeLogUI.setExtendedState(runtimeLogUI.getExtendedState() | JFrame.MAXIMIZED_BOTH);
        });
        toolBar.add(launchRuntimeLogUI);

        JButton helpButton = new JButton(UIUtils.getIconFromResources("help.png"));
        helpButton.addActionListener(e -> {
            JFrame frame = new JFrame();
            frame.setTitle("Module manager documentation - MISA++ for ImageJ");
            frame.setIconImage(UIUtils.getIconFromResources("misaxx.png").getImage());

            PDFReader reader = PDFReader.fromResource("documentation/launcher.pdf");
            frame.setContentPane(reader);

            frame.pack();
            frame.setSize(800,600);
            frame.setVisible(true);
            frame.setExtendedState(frame.getExtendedState() | JFrame.MAXIMIZED_BOTH);
        });
        toolBar.add(helpButton);

        add(toolBar, BorderLayout.NORTH);

        // Detail panel
        JLabel descriptionTitle;
        JLabel descriptionVersionId;
        JTextField descriptionSourceFile;
        JTextArea moduleDescription;
        JButton removeModuleButton;
        JButton openModuleDocumentationButton;

        detailPanel = new JPanel(new GridBagLayout());
        {
            descriptionTitle = new JLabel();
            descriptionTitle.setFont(descriptionTitle.getFont().deriveFont(18f));
            detailPanel.add(descriptionTitle, new GridBagConstraints() {
                {
                    anchor = GridBagConstraints.WEST;
                    gridx = 0;
                    gridy = 0;
                    insets = UI_PADDING;
                }
            });

            descriptionVersionId = new JLabel();
            detailPanel.add(descriptionVersionId, new GridBagConstraints() {
                {
                    anchor = GridBagConstraints.WEST;
                    gridx = 0;
                    gridy = 1;
                    insets = UI_PADDING;
                }
            });

            descriptionSourceFile = new JTextField();
            descriptionSourceFile.setBorder(null);
            descriptionSourceFile.setEditable(false);
            descriptionSourceFile.setOpaque(false);
            descriptionSourceFile.setFont(descriptionSourceFile.getFont().deriveFont(Font.ITALIC));
            detailPanel.add(descriptionSourceFile, new GridBagConstraints() {
                {
                    anchor = GridBagConstraints.WEST;
                    gridx = 0;
                    gridy = 2;
                    insets = UI_PADDING;
                    weightx = 1;
                    fill = GridBagConstraints.HORIZONTAL;
                }
            });

            moduleDescription = new JTextArea();
            moduleDescription.setBorder(null);
            moduleDescription.setEditable(false);
            moduleDescription.setOpaque(false);
            moduleDescription.setLineWrap(true);
            moduleDescription.setWrapStyleWord(true);
            detailPanel.add(moduleDescription, new GridBagConstraints() {
                {
                    anchor = GridBagConstraints.WEST;
                    gridx = 0;
                    gridy = 3;
                    insets = UI_PADDING;
                    weightx = 1;
                    weighty = 1;
                    fill = GridBagConstraints.BOTH;
                }
            });

            openModuleDocumentationButton = new JButton("Show documentation", UIUtils.getIconFromResources("help.png"));
            openModuleDocumentationButton.addActionListener(actionEvent -> {
                MISAModule selectedModule = misaModuleJList.getSelectedValue();
                if(selectedModule != null) {
                    JFrame frame = new JFrame();
                    frame.setTitle(selectedModule.getModuleInfo().getName() + " documentation - MISA++ for ImageJ");
                    frame.setIconImage(UIUtils.getIconFromResources("misaxx.png").getImage());

                    MarkdownReader reader = new MarkdownReader();
                    reader.setMarkdown(selectedModule.getREADME());
                    frame.setContentPane(reader);

                    frame.pack();
                    frame.setSize(800,600);
                    frame.setVisible(true);
                    frame.setExtendedState(frame.getExtendedState() | JFrame.MAXIMIZED_BOTH);
                }
            });
            UIUtils.addToGridBag(detailPanel, openModuleDocumentationButton, 4, 0);

            removeModuleButton = new JButton("Remove", UIUtils.getIconFromResources("delete.png"));
            removeModuleButton.addActionListener(actionEvent -> {
                MISAModule selectedModule = misaModuleJList.getSelectedValue();
                if(selectedModule != null) {
                    if(JOptionPane.showConfirmDialog(this, "Do you really want to remove the selected module from this list?",
                            "Remove module", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
                        try {
                            Files.delete(Paths.get(selectedModule.getLinkPath()));
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        refreshModuleList();
                    }
                }
            });
            UIUtils.addToGridBag(detailPanel, removeModuleButton, 5, 0);

            JButton launchButton = new JButton("Launch", UIUtils.getIconFromResources("run.png"));
            launchButton.addActionListener(actionEvent -> {
                MISAModuleInstanceUI launcher = new MISAModuleInstanceUI(misaModuleJList.getSelectedValue().instantiate(),
                        false, true);
                launcher.pack();
                launcher.setSize(new Dimension(800,600));
                launcher.setVisible(true);
            });
            UIUtils.addToGridBag(detailPanel, launchButton, 6, 0);
        }

        // List of modules
        misaModuleJList = new JList<>(new DefaultListModel<>());
        misaModuleJList.setCellRenderer(new MISAModuleListCellRenderer());
        misaModuleJList.addListSelectionListener(listSelectionEvent -> {
            MISAModule selectedModule = misaModuleJList.getSelectedValue();
            if(selectedModule != null && selectedModule.getModuleInfo() != null) {
                descriptionTitle.setText(selectedModule.getModuleInfo().getName());
                descriptionVersionId.setText(selectedModule.getModuleInfo().getId() + " version " + misaModuleJList.getSelectedValue().getModuleInfo().getVersion());
                descriptionSourceFile.setText(selectedModule.getLinkPath());
                moduleDescription.setText(selectedModule.getModuleInfo().getDescription());

                File linkLocation = new File(selectedModule.getLinkPath());
                if(linkLocation.getParentFile() != null && linkLocation.getParentFile().canWrite()) {
                    removeModuleButton.setEnabled(true);
                }
                else {
                    removeModuleButton.setEnabled(false);
                }
            }
        });

        // Arrange in split panel
        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, misaModuleJList, detailPanel);
        splitPane.setResizeWeight(1.0);
        add(splitPane, BorderLayout.CENTER);
    }

    private void refreshModuleList() {
        MISAModuleRepository.getInstance().refresh();
        DefaultListModel<MISAModule> model = (DefaultListModel<MISAModule>)misaModuleJList.getModel();
        model.clear();
        for(MISAModule module : MISAModuleRepository.getInstance().getModules()) {
            model.addElement(module);
        }
        if(MISAModuleRepository.getInstance().getModules().size() > 0) {
            detailPanel.setVisible(true);
            misaModuleJList.setSelectedIndex(0);
        }
        else {
            detailPanel.setVisible(false);
        }
    }

    public MISAImageJCommand getCommand() {
        return command;
    }
}

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

import com.google.common.eventbus.Subscribe;
import com.google.gson.Gson;
import org.apache.commons.exec.CommandLine;
import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISAModuleInstance;
import org.hkijena.misa_imagej.api.MISAValidityReport;
import org.hkijena.misa_imagej.api.pipelining.MISAPipeline;
import org.hkijena.misa_imagej.api.pipelining.MISAPipelineNode;
import org.hkijena.misa_imagej.api.repository.MISAModule;
import org.hkijena.misa_imagej.api.repository.MISAModuleRepository;
import org.hkijena.misa_imagej.ui.components.CancelableProcessUI;
import org.hkijena.misa_imagej.ui.components.MISACacheTreeUI;
import org.hkijena.misa_imagej.ui.components.MISAValidityReportStatusUI;
import org.hkijena.misa_imagej.ui.components.PDFReader;
import org.hkijena.misa_imagej.ui.components.renderers.MISAModuleListCellRenderer;
import org.hkijena.misa_imagej.ui.repository.MISAModuleRepositoryUI;
import org.hkijena.misa_imagej.utils.GsonUtils;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.DocumentTabPane;
import org.hkijena.misa_imagej.utils.ui.MonochromeColorIcon;
import org.jdesktop.swingx.JXStatusBar;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;
import java.util.*;
import java.util.List;

public class MISAPipelinerUI extends JFrame {

    private DocumentTabPane documentTabPane;

    private MISAPipeline pipeline = new MISAPipeline();
    private JList<MISAModule> moduleList;
    private MISACacheTreeUI cacheTree;
    private MISAPipelineUI pipelineEditor;
    private JList<String> sampleList;
    private JToggleButton synchronizeAllSamplesToggle;

    private boolean isCurrentlySynchronizing = false;

    /**
     * We use this to give users an easy overview of a module
     */
    private Map<MISAModule, MISAModuleInstance> uiParameterSchemata = new HashMap<>();
    private MISAValidityReportStatusUI validityReportStatusUI;

    public MISAPipelinerUI()
    {
        initialize();
        refresh();
    }

    private void initialize() {
        setSize(800, 600);
        getContentPane().setLayout(new BorderLayout());
        setTitle("MISA++ for ImageJ - Pipeline tool");
        setIconImage(UIUtils.getIconFromResources("misaxx.png").getImage());
        UIUtils.setToAskOnClose(this, "Do you really want to close this pipeline builder?", "Close window");

        documentTabPane = new DocumentTabPane();

        JToolBar toolBar = new JToolBar();

        JButton openButton = new JButton("Open", UIUtils.getIconFromResources("open.png"));
        openButton.addActionListener(actionEvent -> open());
        toolBar.add(openButton);

        JButton saveButton = new JButton("Save", UIUtils.getIconFromResources("save.png"));
        saveButton.addActionListener(actionEvent -> save());
        toolBar.add(saveButton);

        toolBar.add(Box.createHorizontalGlue());

        JButton validateButton = new JButton("Check parameters", UIUtils.getIconFromResources("checkmark.png"));
        validateButton.addActionListener(actionEvent -> validityReportStatusUI.setReport(pipeline.getValidityReport()));
        toolBar.add(validateButton);

        JButton exportButton = new JButton("Export", UIUtils.getIconFromResources("export.png"));
        exportButton.addActionListener(actionEvent -> export());
        toolBar.add(exportButton);

        JButton runButton = new JButton("Run", UIUtils.getIconFromResources("run.png"));
        runButton.addActionListener(actionEvent -> runPipeline());
        toolBar.add(runButton);

        JButton helpButton = new JButton(UIUtils.getIconFromResources("help.png"));
        helpButton.addActionListener(e -> documentTabPane.selectSingletonTab("HELP"));
        toolBar.add(helpButton);

        add(toolBar, BorderLayout.NORTH);
        JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);
        tabbedPane.addTab("Available modules", createModuleList());
        tabbedPane.addTab("Samples", createSampleManager());

        pipelineEditor = new MISAPipelineUI();
        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, new JScrollPane(pipelineEditor) {
            {
                setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
                setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
            }
        }, tabbedPane);
        splitPane.setResizeWeight(1);

        documentTabPane.addTab("Pipeline", UIUtils.getIconFromResources("connect.png"), splitPane, DocumentTabPane.CloseMode.withoutCloseButton);
        documentTabPane.addSingletonTab("HELP", "Documentation", UIUtils.getIconFromResources("help.png"),
                PDFReader.fromResource("documentation/pipeliner.pdf"), true);

        add(documentTabPane, BorderLayout.CENTER);

        // Status bar
        JXStatusBar statusBar = new JXStatusBar();
        validityReportStatusUI = new MISAValidityReportStatusUI();
        statusBar.add(validityReportStatusUI);
        add(statusBar, BorderLayout.SOUTH);

        // Connect events
        pipeline.getEventBus().register(this);
    }

    private JPanel createModuleList() {
        JPanel  toolboxPanel = new JPanel(new BorderLayout());
        moduleList = new JList<>();
        moduleList.setCellRenderer(new MISAModuleListCellRenderer());
        moduleList.addListSelectionListener(listSelectionEvent -> updateCacheTree());
        moduleList.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent mouseEvent) {
                if(mouseEvent.getClickCount() == 2) {
                    addInstance();
                }
            }

            @Override
            public void mousePressed(MouseEvent mouseEvent) {}

            @Override
            public void mouseReleased(MouseEvent mouseEvent) {}

            @Override
            public void mouseEntered(MouseEvent mouseEvent) {}

            @Override
            public void mouseExited(MouseEvent mouseEvent) {}
        });

        cacheTree = new MISACacheTreeUI() {
            @Override
            protected Entry createRootEntry() {
                Entry result = super.createRootEntry();
                result.name = "Input and output preview";
                return result;
            }

            @Override
            protected Entry createEntry(MISACache cache) {
                Entry result = super.createEntry(cache);
                result.name = cache.getCacheTypeName() + ": " + result.name;
                return result;
            }
        };

        toolboxPanel.add(new JSplitPane(JSplitPane.VERTICAL_SPLIT, moduleList, cacheTree), BorderLayout.CENTER);

        JToolBar toolboxToolbar = new JToolBar();

        JButton instantiateButton = new JButton("Add to pipeline", UIUtils.getIconFromResources("add.png"));
        instantiateButton.addActionListener(actionEvent -> addInstance());
        toolboxToolbar.add(instantiateButton);

        toolboxToolbar.add(Box.createHorizontalGlue());

        JButton refreshButton = new JButton(UIUtils.getIconFromResources("refresh.png"));
        refreshButton.setToolTipText("Refresh list of available modules");
        refreshButton.addActionListener(actionEvent -> refresh());
        toolboxToolbar.add(refreshButton);

        toolboxPanel.add(toolboxToolbar, BorderLayout.SOUTH);

        return toolboxPanel;
    }

    private JPanel createSampleManager() {
        JPanel  toolboxPanel = new JPanel(new BorderLayout());
        sampleList = new JList<>();
        sampleList.setCellRenderer(new SampleListRenderer(pipeline));
        sampleList.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        toolboxPanel.add(new JScrollPane(sampleList), BorderLayout.CENTER);

        JToolBar toolboxToolbar = new JToolBar();

        JButton synchronizeSelected = new JButton("Synchronize selected", UIUtils.getIconFromResources("connect.png"));
        synchronizeSelected.setToolTipText("Synchronizes the selected samples between all nodes that are working on the samples");
        synchronizeSelected.addActionListener(actionEvent -> synchronizeSelectedSamples());
        toolboxToolbar.add(synchronizeSelected);

        toolboxToolbar.add(Box.createHorizontalGlue());

        synchronizeAllSamplesToggle = new JToggleButton("Autosync", UIUtils.getIconFromResources("connect.png"));
        synchronizeAllSamplesToggle.setSelected(true);
        synchronizeAllSamplesToggle.setToolTipText("If enabled, all nodes have the same set of samples");
        synchronizeAllSamplesToggle.addActionListener(actionEvent -> {
            if(synchronizeAllSamplesToggle.isSelected())
                synchronizeAllSamples();
        });
        toolboxToolbar.add(synchronizeAllSamplesToggle);

        toolboxPanel.add(toolboxToolbar, BorderLayout.SOUTH);

        return toolboxPanel;
    }

    private void synchronizeAllSamples() {
        for(String sample : pipeline.getSampleNames()) {
            for(MISAPipelineNode node : pipeline.getNodes()) {
                if(!node.getModuleInstance().getSamples().containsKey(sample))
                    node.getModuleInstance().addSample(sample);
            }
        }
    }

    private void synchronizeSelectedSamples() {
        Set<MISAPipelineNode> synchronizedNodes = new HashSet<>();
        for(String sample : new ArrayList<>(sampleList.getSelectedValuesList())) {
            synchronizedNodes.addAll(pipeline.getNodesContainingSample(sample));
        }
        for(String sample : new ArrayList<>(sampleList.getSelectedValuesList())) {
            for(MISAPipelineNode node : synchronizedNodes) {
                if(!node.getModuleInstance().getSamples().containsKey(sample))
                    node.getModuleInstance().addSample(sample);
            }
        }
    }

    private void export() {
        MISAValidityReport report = pipeline.getValidityReport();
        validityReportStatusUI.setReport(report);
        if(!report.isValid())
            return;

        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        fileChooser.setDialogTitle("Export pipeline");
        if(fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            try {
               pipeline.export(fileChooser.getSelectedFile().toPath(), true, true, false);
            }
            catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    private void runPipeline() {
        MISAValidityReport report = pipeline.getValidityReport();
        validityReportStatusUI.setReport(report);
        if(!report.isValid())
            return;
        MISARunPipelineDialogUI dialogUI = new MISARunPipelineDialogUI(this);
        if(dialogUI.showDialog() == MISARunPipelineDialogUI.ACCEPT_OPTION) {
            try {
                pipeline.export(dialogUI.getExportPath(), false, false, true);
                List<CommandLine> processes = new ArrayList<>();
                for(MISAPipelineNode node : pipeline.traverse()) {
                    processes.add(node.getModuleInstance().getModule().run(dialogUI.getExportPath().resolve(node.getId()).resolve("parameters.json")));
                }

                // Run the executable
                MISAModuleRepositoryUI.getInstance().getCommand().getLogService().info("Starting worker process ...");
                CancelableProcessUI processUI = new CancelableProcessUI(processes);
                processUI.setLocationRelativeTo(this);

                // React to changes in status
                processUI.addPropertyChangeListener(propertyChangeEvent -> {
                    if(processUI.getStatus() == CancelableProcessUI.Status.Done ||
                            processUI.getStatus() == CancelableProcessUI.Status.Canceled ||
                            processUI.getStatus() == CancelableProcessUI.Status.Failed) {
                        setEnabled(true);
                        if(processUI.getStatus() == CancelableProcessUI.Status.Failed) {
                            JOptionPane.showMessageDialog(this,
                                    "There was an error during calculation. Please check the console to see the cause of this error.",
                                    "Error", JOptionPane.ERROR_MESSAGE);
                        }
                        MISAPipelineOutputUI ui = new MISAPipelineOutputUI(pipeline, dialogUI.getExportPath());
                        ui.setLocationRelativeTo(this);
                        ui.pack();
                        ui.setSize(800, 600);
                        ui.setVisible(true);
                    }
                });

                setEnabled(false);
                processUI.start();
            }
            catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    private void save() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        if(fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            try {
               pipeline.save(fileChooser.getSelectedFile().toPath());
            }
            catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    private void open() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        if(fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            try {
                Gson gson = GsonUtils.getGson();
                pipeline = GsonUtils.fromJsonFile(gson, fileChooser.getSelectedFile().toPath(), MISAPipeline.class);
                refresh();
            }
            catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    private void refreshModuleList() {
        // Refresh the list of available modules
        uiParameterSchemata.clear();
        MISAModuleRepository.getInstance().refresh();
        DefaultListModel<MISAModule> model = new DefaultListModel<>();
        for(MISAModule module : MISAModuleRepository.getInstance().getModules()) {
            model.addElement(module);
            MISAModuleInstance instance = module.instantiate();
            instance.addSample("Preview");
            uiParameterSchemata.put(module, instance);
        }
        moduleList.setModel(model);
    }

    private void refreshSampleList() {
        List<String> samples = new ArrayList<>(pipeline.getSampleNames());
        samples.sort(Comparator.naturalOrder());
        DefaultListModel<String> model = new DefaultListModel<>();
        for(String sample : samples) {
            model.addElement(sample);
        }
        sampleList.setModel(model);
    }

    private void refresh() {
        refreshModuleList();
        refreshSampleList();

        // Update the editor UI
        pipelineEditor.setPipeline(pipeline);
    }

    private void addInstance() {
        if(moduleList.getSelectedValue() != null) {
            MISAPipelineNode node = pipeline.addNode(moduleList.getSelectedValue());

            if(synchronizeAllSamplesToggle.isSelected()) {
                isCurrentlySynchronizing = true;
                for(MISAPipelineNode nd : pipeline.getNodes()) {
                    for(String sample : nd.getModuleInstance().getSamples().keySet()) {
                        if(!node.getModuleInstance().getSamples().containsKey(sample)) {
                            node.getModuleInstance().addSample(sample);
                        }
                    }
                }
                isCurrentlySynchronizing = false;
                refreshSampleList();
            }
        }
    }

    private void updateCacheTree() {
        if(moduleList.getSelectedValue() != null) {
            cacheTree.setSample(uiParameterSchemata.get(moduleList.getSelectedValue()).getSample("Preview"));
        }
    }

    @Subscribe
    public void handleSampleChangedEvents(MISAPipeline.AddedSampleEvent event) {
        if(isCurrentlySynchronizing)
            return;

        if(synchronizeAllSamplesToggle.isSelected()) {
            isCurrentlySynchronizing = true;
            String sample = event.getSample().getName();
            for(MISAPipelineNode node : pipeline.getNodes()) {
                if(!node.getModuleInstance().getSamples().containsKey(sample)) {
                    node.getModuleInstance().addSample(sample);
                }
            }
            isCurrentlySynchronizing = false;
        }
        refreshSampleList();
    }

    @Subscribe
    public void handleSampleChangedEvents(MISAPipeline.RemovedSampleEvent event) {
        if(isCurrentlySynchronizing)
            return;

        if(synchronizeAllSamplesToggle.isSelected()) {
            isCurrentlySynchronizing = true;
            String sample = event.getRemovedSampleName();
            for(MISAPipelineNode node : pipeline.getNodes()) {
                if(node.getModuleInstance().getSamples().containsKey(sample)) {
                    node.getModuleInstance().removeSample(sample);
                }
            }
            isCurrentlySynchronizing = false;
        }

        refreshSampleList();
    }

    @Subscribe
    public void handleSampleChangedEvents(MISAPipeline.RenameSampleEvent event) {
        if(isCurrentlySynchronizing)
            return;

        if(synchronizeAllSamplesToggle.isSelected()) {
            isCurrentlySynchronizing = true;
            String sample = event.getSample().getName();
            for(MISAPipelineNode node : pipeline.getNodes()) {
                if(node.getModuleInstance().getSamples().containsKey(event.getOldName())) {
                    node.getModuleInstance().renameSample(node.getModuleInstance().getSample(event.getOldName()), sample);
                }
            }
            isCurrentlySynchronizing = false;
        }

        refreshSampleList();
    }

    @Subscribe
    public void handleSampleChangedEvents(MISAPipeline.RemovedNodeEvent event) {
        refreshSampleList();
    }

    @Subscribe
    public void handleSampleChangedEvents(MISAPipeline.AddedNodeEvent event) {
        refreshSampleList();
    }

    public static class SampleListRenderer extends JLabel implements ListCellRenderer<String> {

        private MISAPipeline pipeline;
        private MonochromeColorIcon icon = new MonochromeColorIcon(UIUtils.getIconFromResources("sample-template.png"));

        public SampleListRenderer(MISAPipeline pipeline) {
            this.pipeline = pipeline;
            this.setIcon(icon);
            this.setOpaque(true);
            setBorder(BorderFactory.createEmptyBorder(4, 4, 4, 4));
        }

        @Override
        public Component getListCellRendererComponent(JList<? extends String> list, String sample, int index, boolean isSelected, boolean cellHasFocus) {

            if(sample != null) {
                setText(sample);
                Collection<MISAPipelineNode> nodes = pipeline.getNodesContainingSample(sample);

                // Generate a color for the set of samples
                int hashCode = 0;
                for(MISAPipelineNode node : nodes) {
                    hashCode += node.hashCode();
                }
                float h = Math.abs(hashCode % 256) / 255.0f;
                icon.setColor(Color.getHSBColor(h, 0.5f, 1));
            }

            if(isSelected || cellHasFocus) {
                setBackground(new Color(184, 207, 229));
            }
            else {
                setBackground(new Color(255,255,255));
            }

            return this;
        }
    }

}

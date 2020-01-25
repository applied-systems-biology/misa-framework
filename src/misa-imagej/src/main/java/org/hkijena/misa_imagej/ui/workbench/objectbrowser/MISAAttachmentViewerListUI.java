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

import org.hkijena.misa_imagej.api.MISAAttachment;
import org.hkijena.misa_imagej.api.workbench.MISAAttachmentDatabase;
import org.hkijena.misa_imagej.utils.BusyCursor;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.jdesktop.swingx.JXPanel;
import org.jdesktop.swingx.ScrollableSizeHint;

import javax.swing.*;
import java.awt.*;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class MISAAttachmentViewerListUI extends JPanel {
    private JScrollPane scrollPane;
    private JXPanel listPanel;
    private JLabel statsLabel;

    private MISAAttachmentDatabase database;
    private List<String> databaseFilters;
    private int nextDisplayedId;
    private List<MISAAttachment> attachments = new ArrayList<>();
    private MISAAttachmentDatabase.Iterator databaseIterator;

    public MISAAttachmentViewerListUI(MISAAttachmentDatabase database) {
        this.database = database;
        initialize();
    }

    private void initialize() {
        setLayout(new BorderLayout());

        JToolBar toolBar = new JToolBar();

        JButton loadAllMissingDataButton = new JButton("Load all missing data", UIUtils.getIconFromResources("quickload.png"));
        loadAllMissingDataButton.addActionListener(e -> loadAllMissingData());
        toolBar.add(loadAllMissingDataButton);

        JButton unloadButton = new JButton("Unload all data", UIUtils.getIconFromResources("eye-slash.png"));
        unloadButton.addActionListener(e -> reloadData());
        toolBar.add(unloadButton);

        JButton exportButton = new JButton("Export", UIUtils.getIconFromResources("save.png"));
        exportButton.addActionListener(e -> exportData());
        toolBar.add(exportButton);

        toolBar.add(Box.createHorizontalGlue());
        statsLabel = new JLabel();
        toolBar.add(statsLabel);

        add(toolBar, BorderLayout.NORTH);

        listPanel = new JXPanel();
        listPanel.setScrollableWidthHint(ScrollableSizeHint.FIT);
        listPanel.setScrollableHeightHint(ScrollableSizeHint.NONE);
        listPanel.setLayout(new GridBagLayout());
//        listPanel.setBorder(BorderFactory.createLineBorder(Color.RED));

        JPanel internalPanel = new JPanel(new BorderLayout());
        internalPanel.add(listPanel, BorderLayout.NORTH);

        scrollPane = new JScrollPane(internalPanel);
        scrollPane.setViewportView(listPanel);
        add(scrollPane, BorderLayout.CENTER);
        scrollPane.getVerticalScrollBar().addAdjustmentListener(e -> {
            addItemIfNeeded();
        });
    }

    private void exportData() {
        if(databaseFilters != null) {
            JFileChooser fileChooser = new JFileChooser();
            fileChooser.setDialogTitle("Export as *.json");
            if(fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
                MISAAttachmentSaverDialogUI dialog = new MISAAttachmentSaverDialogUI(fileChooser.getSelectedFile().toPath(), database, databaseFilters);
                dialog.setModal(true);
                dialog.pack();
                dialog.setSize(400,300);
                dialog.setLocationRelativeTo(SwingUtilities.getWindowAncestor(this));
                dialog.startOperation();
                dialog.setVisible(true);
            }
        }
    }

    private void loadAllMissingData() {
        MISAAttachmentExpanderDialogUI dialog = new MISAAttachmentExpanderDialogUI(attachments);
        dialog.setModal(true);
        dialog.pack();
        dialog.setSize(400,300);
        dialog.setLocationRelativeTo(SwingUtilities.getWindowAncestor(this));
        dialog.startOperation();
        dialog.setVisible(true);
    }

    public void setDatabaseFilters(List<String> databaseFilters) {
        this.databaseFilters = databaseFilters;
        reloadData();
    }

    private void reloadData() {
        try(BusyCursor busyCursor = new BusyCursor(this)) {
            this.nextDisplayedId = 0;
            this.attachments.clear();
            listPanel.removeAll();
            listPanel.revalidate();
            listPanel.repaint();
            if (this.databaseIterator != null) {
                try {
                    this.databaseIterator.close();
                } catch (SQLException e) {
                    throw new RuntimeException(e);
                }
            }
            this.databaseIterator = database.createAttachmentIterator(databaseFilters);
        }
        addItem();
    }

    private void addItemIfNeeded() {
        JScrollBar scrollBar = scrollPane.getVerticalScrollBar();
        if(scrollBar.getMaximum() <= scrollBar.getVisibleAmount() || (scrollBar.getValue() + scrollBar.getVisibleAmount()) > (scrollBar.getMaximum() * 0.9)) {
            addItem();
        }
    }

    private void addItem() {
        try(BusyCursor busyCursor = new BusyCursor(this)) {
            MISAAttachment attachment;
            try {
                attachment = databaseIterator.nextAttachment();
            } catch (SQLException e) {
                throw new RuntimeException(e);
            }
            if (attachment == null)
                return;
            attachments.add(attachment);
            MISAAttachmentViewerUI viewer = new MISAAttachmentViewerUI(this, attachment);
            viewer.setAlignmentY(Component.TOP_ALIGNMENT);
            listPanel.add(viewer, new GridBagConstraints() {
                {
                    gridx = 0;
                    gridy = nextDisplayedId;
                    fill = GridBagConstraints.HORIZONTAL;
                    anchor = GridBagConstraints.NORTHWEST;
                    insets = UIUtils.UI_PADDING;
                    weightx = 1;
                }
            });
            ++nextDisplayedId;
        }
        revalidate();
        repaint();

        SwingUtilities.invokeLater(() -> addItemIfNeeded());
    }

    public void synchronizeViewerLabelProperties() {
        int preferredWidth = 0;
        for(int i = 0; i < listPanel.getComponentCount(); ++i) {
            if(listPanel.getComponent(i) instanceof MISAAttachmentViewerUI) {
                for(Component component : ((MISAAttachmentViewerUI) listPanel.getComponent(i)).getPropertyLabels()) {
                    preferredWidth = Math.max(preferredWidth, component.getPreferredSize().width);
                }
            }
        }
        for(int i = 0; i < listPanel.getComponentCount(); ++i) {
            if (listPanel.getComponent(i) instanceof MISAAttachmentViewerUI) {
                for(JLabel component : ((MISAAttachmentViewerUI) listPanel.getComponent(i)).getPropertyLabels()) {
                    component.setPreferredSize(new Dimension(preferredWidth, component.getPreferredSize().height));
                    SwingUtilities.invokeLater(() -> {
                        component.revalidate();
                        component.repaint();
                    });
                }
            }
        }

    }
}

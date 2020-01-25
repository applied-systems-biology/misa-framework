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

import com.google.common.eventbus.Subscribe;
import com.google.gson.JsonPrimitive;
import org.hkijena.misa_imagej.api.MISAAttachment;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.MonochromeColorIcon;
import org.hkijena.misa_imagej.utils.ui.ReadOnlyToggleButtonModel;

import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.util.List;

/**
 * Displays attachments from an attachment database
 */
public class MISAAttachmentViewerUI extends JPanel {

    private MISAAttachmentViewerListUI listUI;
    private MISAAttachment attachment;
    private JPanel headerPanel;
    private JPanel contentPanel;
    private Set<JLabel> propertyLabels = new HashSet<>();

    public MISAAttachmentViewerUI(MISAAttachmentViewerListUI listUI, MISAAttachment attachment) {
        this.listUI = listUI;
        this.attachment = attachment;

        if (!attachment.hasData())
            attachment.load();

        initialize();
        refreshContents();

        attachment.getEventBus().register(this);
    }

    @Subscribe
    public void handleAttachmentDataLoadedEvent(MISAAttachment.DataLoadedEvent event) {
        refreshContents();
    }

    private void initialize() {
        setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY));
        setLayout(new BorderLayout());
        headerPanel = new JPanel();
        headerPanel.setBackground(Color.LIGHT_GRAY);
        headerPanel.setBorder(BorderFactory.createLineBorder(Color.LIGHT_GRAY, 4));
        headerPanel.setLayout(new BoxLayout(headerPanel, BoxLayout.LINE_AXIS));

        JLabel titleLabel = new JLabel(attachment.getDocumentationTitle(),
                new MonochromeColorIcon(UIUtils.getIconFromResources("object-template.png"), attachment.toColor()),
                JLabel.LEFT);
        headerPanel.add(titleLabel);

        JTextField pathLabel = new JTextField();
        pathLabel.setEditable(false);
        pathLabel.setBorder(null);
        pathLabel.setOpaque(false);
        pathLabel.setText(attachment.getAttachmentFullPath());
        headerPanel.add(Box.createHorizontalStrut(8));
        headerPanel.add(pathLabel);
        headerPanel.add(Box.createHorizontalGlue());
        headerPanel.add(Box.createHorizontalStrut(8));

        JButton exportButton = new JButton(UIUtils.getIconFromResources("save.png"));
        UIUtils.makeFlatWithoutMargin(exportButton);
        exportButton.setToolTipText("Export as *.json");
        exportButton.addActionListener(e -> exportToJson());
        headerPanel.add(exportButton);

        headerPanel.add(Box.createHorizontalStrut(4));

        JButton loadAllLazy = new JButton(UIUtils.getIconFromResources("quickload.png"));
        UIUtils.makeFlatWithoutMargin(loadAllLazy);
        loadAllLazy.setToolTipText("Load all missing data");
        loadAllLazy.addActionListener(e -> loadMissingData());
        headerPanel.add(loadAllLazy);

        add(headerPanel, BorderLayout.NORTH);
        contentPanel = new JPanel();
        contentPanel.setLayout(new BoxLayout(contentPanel, BoxLayout.Y_AXIS));
        add(contentPanel, BorderLayout.CENTER);
    }

    private void exportToJson() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle("Export object as *.json");
        if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            MISAAttachmentSaverDialogUI dialog = new MISAAttachmentSaverDialogUI(fileChooser.getSelectedFile().toPath(), attachment);
            dialog.setModal(true);
            dialog.pack();
            dialog.setSize(400, 300);
            dialog.setLocationRelativeTo(SwingUtilities.getWindowAncestor(this));
            dialog.startOperation();
            dialog.setVisible(true);
        }
    }

    private void loadMissingData() {
        MISAAttachmentExpanderDialogUI dialog = new MISAAttachmentExpanderDialogUI(attachment);
        dialog.setModal(true);
        dialog.pack();
        dialog.setSize(400, 300);
        dialog.setLocationRelativeTo(SwingUtilities.getWindowAncestor(this));
        dialog.startOperation();
        dialog.setVisible(true);
    }

    private JLabel insertLabelFor(MISAAttachment.Property property, Icon icon, JPanel row) {
        JLabel label = new JLabel(property.getNamePath() != null ? property.getNamePath().substring(1) : null, icon, JLabel.LEFT);
        label.setToolTipText(createToolTipFor(property));
        propertyLabels.add(label);
        row.add(label, BorderLayout.WEST);
        return label;
    }

    private void insertComponent(Component ui, JPanel row) {
        row.add(ui, BorderLayout.CENTER);
    }

    private String createToolTipFor(MISAAttachment.Property property) {
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("<html>");
        stringBuilder.append("<i>").append(property.getPath().replace("/", "&frasl;")).append("</i><br/>");
        if (property.getDescription() != null && !property.getDescription().isEmpty())
            stringBuilder.append("<br/>").append(property.getDescription());
        stringBuilder.append("</html>");
        return stringBuilder.toString();
    }

    private void insertDisplayFor(MISAAttachment.Property property) {

        JPanel row = new JPanel(new BorderLayout(4, 4));
        row.setBorder(BorderFactory.createEmptyBorder(4, 4, 4, 4));

        if (property.hasValue()) {
            if (property instanceof MISAAttachment.MemoryProperty) {
                JsonPrimitive primitive = ((MISAAttachment.MemoryProperty) property).getValue();

                if (primitive.isString()) {
                    insertLabelFor(property, UIUtils.getIconFromResources("text.png"), row);
                    if (primitive.getAsString().contains("\n")) {
                        JTextArea textArea = new JTextArea(primitive.getAsString());
                        textArea.setEditable(false);
                        textArea.setToolTipText(createToolTipFor(property));
                        JScrollPane scrollPane = new JScrollPane(textArea);
                        scrollPane.setPreferredSize(new Dimension(100, 200));
                        insertComponent(scrollPane, row);
                    } else {
                        JTextField component = new JTextField(primitive.getAsString());
                        component.setToolTipText(createToolTipFor(property));
                        component.setEditable(false);
                        insertComponent(component, row);
                    }
                } else if (primitive.isBoolean()) {
                    JCheckBox component = new JCheckBox(property.getNamePath().substring(1));
                    component.setToolTipText(createToolTipFor(property));
                    component.setModel(new ReadOnlyToggleButtonModel(primitive.getAsBoolean()));
                    JLabel label = insertLabelFor(property, null, row);
                    label.setText("");

                    insertComponent(component, row);
                } else if (primitive.isNumber()) {
                    insertLabelFor(property, UIUtils.getIconFromResources("number.png"), row);
                    JTextField component = new JTextField(primitive.getAsNumber() + "");
                    component.setToolTipText(createToolTipFor(property));
                    component.setEditable(false);
                    insertComponent(component, row);
                }
            } else if (property instanceof MISAAttachment.LazyProperty) {
                insertLabelFor(property, UIUtils.getIconFromResources("object.png"), row);
                JLabel component = new JLabel(((MISAAttachment.LazyProperty) property).getDocumentationTitle(),
                        new MonochromeColorIcon(UIUtils.getIconFromResources("object-template.png"),
                                ((MISAAttachment.LazyProperty) property).toColor()),
                        JLabel.LEFT);
                insertComponent(component, row);
            }
        } else {
            insertLabelFor(property, UIUtils.getIconFromResources("object.png"), row);
            JButton loadButton = new JButton("Load missing data", UIUtils.getIconFromResources("database.png"));
            UIUtils.makeFlat(loadButton);
            loadButton.addActionListener(e -> property.loadValue());
            insertComponent(loadButton, row);
        }

        contentPanel.add(row);
    }

    public void refreshContents() {
        contentPanel.removeAll();
        propertyLabels.clear();

        List<MISAAttachment.Property> properties = new ArrayList<>(attachment.getProperties());
        properties.sort(Comparator.comparing(MISAAttachment.Property::getPath));

        for (int i = 0; i < properties.size(); ++i) {
            insertDisplayFor(properties.get(i));
        }

        SwingUtilities.invokeLater(() -> {
            revalidate();
            repaint();
            listUI.synchronizeViewerLabelProperties();
        });
    }

    public Set<JLabel> getPropertyLabels() {
        return Collections.unmodifiableSet(propertyLabels);
    }
}

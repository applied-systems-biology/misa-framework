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
import org.hkijena.misa_imagej.MISAImageJRegistryService;
import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilter;
import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilterChangedEvent;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.*;

public class MISAAttachmentFilterUI extends JPanel {

    private static final Color BORDER_COLOR = new Color(128, 128, 128);
    private MISAAttachmentFilter filter;
    private JPanel content;
    private JButton enableToggleButton;

    public MISAAttachmentFilterUI(MISAAttachmentFilter filter) {
        this.filter = filter;
        initialize();
        filter.getEventBus().register(this);
    }

    private void initialize() {
        setLayout(new BorderLayout());

        // Create content panel
        JPanel contentContainer = new JPanel(new BorderLayout());
        contentContainer.setOpaque(false);
        contentContainer.setBorder(BorderFactory.createEmptyBorder(4, 16, 8, 16));
        content = new JPanel();
        contentContainer.add(content, BorderLayout.CENTER);

        // Create title panel
        JPanel titlePanel = new JPanel();
        titlePanel.setLayout(new BoxLayout(titlePanel, BoxLayout.LINE_AXIS));
        titlePanel.setBorder(BorderFactory.createEmptyBorder(20, 16, 4, 16));
        titlePanel.setOpaque(false);

        JLabel nameLabel = new JLabel(MISAImageJRegistryService.getInstance().getAttachmentFilterUIRegistry()
                .getNameFilterName(filter.getClass()));
        titlePanel.add(nameLabel);
        titlePanel.add(Box.createHorizontalGlue());
        titlePanel.add(Box.createHorizontalStrut(8));

        JButton removeButton = new JButton(UIUtils.getIconFromResources("delete.png"));
        removeButton.setToolTipText("Remove filter");
        removeButton.addActionListener(e -> removeFilter());
        UIUtils.makeFlatWithoutMargin(removeButton);
        titlePanel.add(removeButton);

        enableToggleButton = new JButton();
        UIUtils.makeFlatWithoutMargin(enableToggleButton);
        enableToggleButton.addActionListener(e -> toggleEnableDisable());
        titlePanel.add(Box.createHorizontalStrut(4));
        titlePanel.add(enableToggleButton);
        updateEnableDisableToggleButton();

//        JButton moveUpButton = new JButton(UIUtils.getIconFromResources("arrow-up.png"));
//        UIUtils.makeFlatWithoutMargin(moveUpButton);
//        moveUpButton.addActionListener(e -> moveFilterUp());
//        titlePanel.add(moveUpButton);
//
//        JButton moveDownButton = new JButton(UIUtils.getIconFromResources("arrow-down.png"));
//        UIUtils.makeFlatWithoutMargin(moveDownButton);
//        moveDownButton.addActionListener(e -> moveFilterDown());
//        titlePanel.add(moveDownButton);

        add(titlePanel, BorderLayout.NORTH);
        add(contentContainer, BorderLayout.CENTER);
    }

    private void toggleEnableDisable() {
        filter.setEnabled(!filter.isEnabled());
        updateEnableDisableToggleButton();
    }

    private void updateEnableDisableToggleButton() {
        if (filter.isEnabled()) {
            enableToggleButton.setIcon(UIUtils.getIconFromResources("eye.png"));
            enableToggleButton.setToolTipText("Disable filter");
        } else {
            enableToggleButton.setIcon(UIUtils.getIconFromResources("eye-slash.png"));
            enableToggleButton.setToolTipText("Enable filter");
        }

    }

//    private void moveFilterDown() {
//    }
//
//    private void moveFilterUp() {
//    }

    private void removeFilter() {
        if (JOptionPane.showConfirmDialog(this, "Do you really want to remove this filter?", "Remove filter",
                JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE) == JOptionPane.YES_OPTION) {
            filter.getDatabase().removeFilter(filter);
        }
    }

    public MISAAttachmentFilter getFilter() {
        return filter;
    }

    public JPanel getContentPane() {
        return content;
    }

    @Override
    protected void paintComponent(Graphics graphics) {
        super.paintComponent(graphics);

        graphics.setColor(getFilterColor());
        graphics.fillRect(8, 8, getWidth() - 16 - 1, 8);
        graphics.setColor(BORDER_COLOR);
        graphics.drawRect(8, 8, getWidth() - 1 - 16, getHeight() - 1 - 8);

    }

    @Subscribe
    public void handleFilterChangedEvent(MISAAttachmentFilterChangedEvent event) {
        repaint();
    }

    private Color getFilterColor() {
        if (filter.isEnabled()) {
            float h = Math.abs(MISAImageJRegistryService.getInstance().getAttachmentFilterUIRegistry()
                    .getNameFilterName(filter.getClass()).hashCode() % 256) / 255.0f;
            return Color.getHSBColor(h, 0.8f, 0.9f);
        } else {
            return Color.GRAY;
        }
    }
}

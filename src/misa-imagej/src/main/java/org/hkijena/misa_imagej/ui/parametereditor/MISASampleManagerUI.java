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

import com.google.common.eventbus.Subscribe;
import org.hkijena.misa_imagej.api.MISAModuleInstance;
import org.hkijena.misa_imagej.api.MISASample;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.MonochromeColorIcon;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class MISASampleManagerUI extends JPanel {

    private MISAModuleInstance moduleInstance;
    private JPanel sampleList;

    public MISASampleManagerUI(MISAModuleInstance moduleInstance) {
        this.moduleInstance = moduleInstance;
        initialize();
        refreshList();
    }

    private void initialize() {
        setLayout(new BorderLayout());

        JToolBar toolBar = new JToolBar();

        JButton addSamplesButton = new JButton("Add samples", UIUtils.getIconFromResources("add.png"));
        addSamplesButton.addActionListener(e -> addSamples());
        toolBar.add(addSamplesButton);

        toolBar.add(Box.createHorizontalGlue());

        JButton clearSamplesButton = new JButton("Remove all samples", UIUtils.getIconFromResources("delete.png"));
        clearSamplesButton.addActionListener(e -> clearSamplesButton());
        toolBar.add(clearSamplesButton);

        add(toolBar, BorderLayout.NORTH);

        sampleList = new JPanel(new GridBagLayout());
        add(new JScrollPane(sampleList), BorderLayout.CENTER);

        moduleInstance.getEventBus().register(this);
    }

    @Subscribe
    public void handleSampleListChangeEvent(MISAModuleInstance.AddedSampleEvent event) {
        refreshList();
    }

    @Subscribe
    public void handleSampleListChangeEvent(MISAModuleInstance.RemovedSampleEvent event) {
        refreshList();
    }

    @Subscribe
    public void handleSampleListChangeEvent(MISAModuleInstance.RenamedSampleEvent event) {
        refreshList();
    }

    private void addSamples() {
        AddSamplesDialog dialog = new AddSamplesDialog(SwingUtilities.getWindowAncestor(this), moduleInstance);
        dialog.setModal(true);
        dialog.pack();
        dialog.setSize(new Dimension(500,400));
        dialog.setLocationRelativeTo(this);
        dialog.setVisible(true);
    }

    private void clearSamplesButton() {
        if(JOptionPane.showConfirmDialog(this,
                "Do you really want to remove all samples?",
                "Remove all samples", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE) == JOptionPane.YES_OPTION) {
            List<MISASample> samples = new ArrayList<>(moduleInstance.getSamples().values());
            for(MISASample sample : samples) {
                moduleInstance.removeSample(sample);
            }
        }
    }

    private void refreshList() {
        sampleList.removeAll();

        List<MISASample> samples = new ArrayList<>(moduleInstance.getSamples().values());
        samples.sort(Comparator.comparing(MISASample::getName));

        int row = 0;

        for(MISASample sample : samples) {
            Icon icon = new MonochromeColorIcon(UIUtils.getIconFromResources("sample-template.png"), sample.toColor());
            JLabel label = new JLabel(sample.getName(), icon, SwingConstants.LEFT);
            final int row_ = row++;
            JButton renameButton = new JButton(UIUtils.getIconFromResources("edit.png"));
            renameButton.setToolTipText("Rename sample");
            UIUtils.makeFlat(renameButton);
            renameButton.addActionListener(e -> renameSample(sample));
            sampleList.add(renameButton, new GridBagConstraints() {
                {
                    gridx = 0;
                    gridy = row_;
                    insets = UIUtils.UI_PADDING;
                    fill = GridBagConstraints.NONE;
                    weightx = 0;
                }
            });

            JButton removeButton = new JButton(UIUtils.getIconFromResources("delete.png"));
            removeButton.setToolTipText("Remove sample");
            UIUtils.makeFlat(removeButton);
            removeButton.addActionListener(e -> removeSample(sample));
            sampleList.add(removeButton, new GridBagConstraints() {
                {
                    gridx = 1;
                    gridy = row_;
                    insets = UIUtils.UI_PADDING;
                    anchor = GridBagConstraints.WEST;
                    weightx = 0;
                    fill = GridBagConstraints.NONE;
                }
            });
            sampleList.add(label, new GridBagConstraints() {
                {
                    gridx = 2;
                    gridy = row_;
                    weightx = 1;
                    insets = UIUtils.UI_PADDING;
                    fill = GridBagConstraints.HORIZONTAL;
                }
            });
        }

        UIUtils.addFillerGridBagComponent(sampleList, row, 2);

        revalidate();
        repaint();
    }

    private void renameSample(MISASample sample) {
        String newName = JOptionPane.showInputDialog(this,"Please input a new name", sample.getName());
        if(newName != null && !newName.isEmpty() && !newName.equals(sample.getName())) {
            moduleInstance.renameSample(sample, newName);
        }
    }

    private void removeSample(MISASample sample) {
        if(JOptionPane.showConfirmDialog(this,
                "Do you really want to remove the sample '" + sample.getName() + "'?",
                "Remove sample", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE) == JOptionPane.YES_OPTION) {
            moduleInstance.removeSample(sample);
        }
    }

}

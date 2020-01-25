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

package org.hkijena.misa_imagej.extension.datasources;

import org.hkijena.misa_imagej.api.MISADataSource;
import org.hkijena.misa_imagej.ui.datasources.MISADataSourceUI;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;

public class MISAFolderLinkDataSourceUI extends MISADataSourceUI {

    private JTextField display;

    public MISAFolderLinkDataSourceUI(MISADataSource dataSource) {
        super(dataSource);
        refreshDisplay();
    }

    @Override
    protected void initialize() {
        setLayout(new BoxLayout(this, BoxLayout.LINE_AXIS));

        if(getNativeDataSource().getCache().getCachePatternDocumentation() != null &&
                !getNativeDataSource().getCache().getCachePatternDocumentation().isEmpty()) {
            JButton infoButton = new JButton(UIUtils.getIconFromResources("info.png"));
            infoButton.setToolTipText(getNativeDataSource().getCache().getCacheTooltip());
            add(infoButton);
        }

        display = new JTextField();
        display.setEditable(false);
        add(display);

        JButton openInFilemanagerButton = new JButton(UIUtils.getIconFromResources("target.png"));
        openInFilemanagerButton.setToolTipText("Open in file manager");
        openInFilemanagerButton.addActionListener(e -> {
            if(getNativeDataSource().getSourceFolder() != null) {
                try {
                    Desktop.getDesktop().open(getNativeDataSource().getSourceFolder().toFile());
                } catch (IOException e1) {
                    throw new RuntimeException(e1);
                }
            }
        });
        add(openInFilemanagerButton);

        JButton selectButton = new JButton(UIUtils.getIconFromResources("open.png"));
        selectButton.addActionListener(actionEvent -> {
            JFileChooser chooser = new JFileChooser();
            chooser.setDialogTitle("Select folder");
            chooser.setMultiSelectionEnabled(false);
            chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
            if(chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
                getNativeDataSource().setSourceFolder(chooser.getSelectedFile().toPath());
            }
            refreshDisplay();
        });
        add(selectButton);


        refreshDisplay();
    }

    private void refreshDisplay() {
        if (getNativeDataSource() == null || display == null)
            return;
        if(getNativeDataSource().getSourceFolder() == null)
            display.setText("<No path set>");
        else
            display.setText(getNativeDataSource().getSourceFolder().toString());
    }

    private MISAFolderLinkDataSource getNativeDataSource() {
        return (MISAFolderLinkDataSource)getDataSource();
    }
}

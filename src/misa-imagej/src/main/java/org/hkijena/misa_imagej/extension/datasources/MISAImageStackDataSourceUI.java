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

import ij.ImagePlus;
import ij.WindowManager;
import org.hkijena.misa_imagej.api.MISADataSource;
import org.hkijena.misa_imagej.ui.datasources.MISADataSourceUI;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.swappers.ImageStackSwapper;
import org.hkijena.misa_imagej.utils.swappers.TiffSwapper;
import org.hkijena.misa_imagej.utils.ui.ImagePlusJMenuItem;

import javax.swing.*;
import java.nio.file.Paths;

/**
 * Editor for OME Tiff caches
 */
public class MISAImageStackDataSourceUI extends MISADataSourceUI {

    private JTextField display;
    private JButton optionButton;

    public MISAImageStackDataSourceUI(MISADataSource dataSource) {
        super(dataSource);
        refreshDisplay();
    }

    @Override
    protected void initialize() {
        setLayout(new BoxLayout(this, BoxLayout.LINE_AXIS));

        display = new JTextField();
        display.setEditable(false);
        add(display);

        JButton selectButton = new JButton(UIUtils.getIconFromResources("open.png"));
        createImportPopup(selectButton);
        add(selectButton);

        optionButton = new JButton(UIUtils.getIconFromResources("edit.png"));
        createEditPopup(optionButton);
        add(optionButton);

        refreshDisplay();
    }

    /**
     * Creates a popup menu that allows selecting data from filesystem or from ImageJ
     * @param selectButton
     */
    private void createImportPopup(JButton selectButton) {
        JPopupMenu selectOptions = UIUtils.addPopupMenuToComponent(selectButton);
        boolean hasImageJData = false;
        for(int i = 1; i <= WindowManager.getImageCount(); ++i) {
            final ImagePlus image = WindowManager.getImage(WindowManager.getNthImageID(i));
            ImagePlusJMenuItem item = new ImagePlusJMenuItem(image);
            item.addActionListener(actionEvent -> {
                getNativeDataSource().setStackSwapper(new ImageStackSwapper(image, null));
                refreshDisplay();
            });
            selectOptions.add(item);
            hasImageJData = true;
        }
        if(hasImageJData) {
            selectOptions.addSeparator();
        }

        // Allow selection from filesystem
        JMenuItem selectExternal = new JMenuItem("Import folder ...", UIUtils.getIconFromResources("import.png"));
        selectExternal.addActionListener(actionEvent -> {
            JFileChooser chooser = new JFileChooser();
            chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
            chooser.setAcceptAllFileFilterUsed(false);
            if(chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
                getNativeDataSource().setStackSwapper(new ImageStackSwapper(null, Paths.get(chooser.getSelectedFile().getAbsolutePath())));
                refreshDisplay();
            }
        });
        selectOptions.add(selectExternal);

        // Allow refresh of menu
        JMenuItem refresh = new JMenuItem("Refresh list", UIUtils.getIconFromResources("refresh.png"));
        refresh.addActionListener(actionEvent -> createImportPopup(selectButton));
        selectOptions.add(refresh);
    }

    private void createEditPopup(JButton selectButton) {
        JPopupMenu selectOptions = UIUtils.addPopupMenuToComponent(selectButton);

        if(getNativeDataSource() == null || display == null)
            return;
        if(getNativeDataSource().getStackSwapper() != null) {
            JMenuItem clearItem = new JMenuItem("Clear data", UIUtils.getIconFromResources("delete.png"));
            clearItem.addActionListener(actionEvent -> {
                getNativeDataSource().setStackSwapper(null);
                refreshDisplay();
            });
            selectOptions.add(clearItem);

            if(!getNativeDataSource().getStackSwapper().isInImageJ() && getNativeDataSource().getStackSwapper().isInFilesystem()) {
                JMenuItem importItem = new JMenuItem("Import into ImageJ", UIUtils.getIconFromResources("import.png"));
                importItem.addActionListener(actionEvent -> {
                    getNativeDataSource().getStackSwapper().importIntoImageJ(null);
                    refreshDisplay();
                });
                selectOptions.add(importItem);
            }

            if(getNativeDataSource().getStackSwapper().isInImageJ()) {
                JMenuItem selectItem = new JMenuItem("Select in ImageJ", UIUtils.getIconFromResources("target.png"));
                selectItem.addActionListener(actionEvent -> {
                    getNativeDataSource().getStackSwapper().editInImageJ();
                });
                selectOptions.add(selectItem);
            }
        }
    }

    private void refreshDisplay() {
        if(getNativeDataSource() == null || display == null)
            return;
        if(getNativeDataSource().getStackSwapper() != null)
            display.setText(getNativeDataSource().getStackSwapper().toString());
        else
            display.setText("<No data set>");
        createEditPopup(optionButton);
    }

    private MISAImageStackDataSource getNativeDataSource() {
        return (MISAImageStackDataSource)getDataSource();
    }
}

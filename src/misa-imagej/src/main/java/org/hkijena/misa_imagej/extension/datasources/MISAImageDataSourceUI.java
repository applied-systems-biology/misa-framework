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
import org.hkijena.misa_imagej.utils.swappers.OMETiffSwapper;
import org.hkijena.misa_imagej.utils.swappers.TiffSwapper;
import org.hkijena.misa_imagej.utils.ui.ImagePlusJMenuItem;

import javax.swing.*;

/**
 * Editor for OME Tiff caches
 */
public class MISAImageDataSourceUI extends MISADataSourceUI {

    private JTextField display;
    private JButton optionButton;

    public MISAImageDataSourceUI(MISADataSource dataSource) {
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
                getNativeDataSource().setTiffSwapper(new TiffSwapper(image, null));
                refreshDisplay();
            });
            selectOptions.add(item);
            hasImageJData = true;
        }
        if(hasImageJData) {
            selectOptions.addSeparator();
        }

        // Allow selection from filesystem
        JMenuItem selectExternal = new JMenuItem("From filesystem ...", UIUtils.getIconFromResources("import.png"));
        selectExternal.addActionListener(actionEvent -> {
//            java.awt.FileDialog dialog = new FileDialog((JFrame)SwingUtilities.getWindowAncestor(this), "Open image", FileDialog.LOAD);
//            dialog.setMultipleMode(false);
//            dialog.setFile("*.ome.tif;*.ome.tiff");
//            dialog.setAutoRequestFocus(true);
//            dialog.setVisible(true);
//            if(dialog.getFiles().length > 0) {
//                cache.setTiffSwapper(new OMETiffSwapper(null, dialog.getFiles()[0].getAbsolutePath()));
//                refreshDisplay();
//            }
            JFileChooser chooser = new JFileChooser();
            if(chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
                getNativeDataSource().setTiffSwapper(new TiffSwapper(null, chooser.getSelectedFile().getAbsolutePath()));
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
        if(getNativeDataSource().getTiffSwapper() != null) {
            JMenuItem clearItem = new JMenuItem("Clear data", UIUtils.getIconFromResources("delete.png"));
            clearItem.addActionListener(actionEvent -> {
                getNativeDataSource().setTiffSwapper(null);
                refreshDisplay();
            });
            selectOptions.add(clearItem);

            if(!getNativeDataSource().getTiffSwapper().isInImageJ() && getNativeDataSource().getTiffSwapper().isInFilesystem()) {
                JMenuItem importItem = new JMenuItem("Import into ImageJ", UIUtils.getIconFromResources("import.png"));
                importItem.addActionListener(actionEvent -> {
                    getNativeDataSource().getTiffSwapper().importIntoImageJ(null); // BioFormats decides by itself
                    refreshDisplay();
                });
                selectOptions.add(importItem);
            }

            if(getNativeDataSource().getTiffSwapper().isInImageJ()) {
                JMenuItem selectItem = new JMenuItem("Select in ImageJ", UIUtils.getIconFromResources("target.png"));
                selectItem.addActionListener(actionEvent -> {
                    getNativeDataSource().getTiffSwapper().editInImageJ();
                });
                selectOptions.add(selectItem);
            }
        }
    }

    private void refreshDisplay() {
        if(getNativeDataSource() == null || display == null)
            return;
        if(getNativeDataSource().getTiffSwapper() != null)
            display.setText(getNativeDataSource().getTiffSwapper().toString());
        else
            display.setText("<No data set>");
        createEditPopup(optionButton);
    }

    private MISAImageDataSource getNativeDataSource() {
        return (MISAImageDataSource)getDataSource();
    }
}

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

package org.hkijena.misa_imagej.extension.outputcaches;

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.workbench.MISAOutput;
import org.hkijena.misa_imagej.ui.workbench.MISAOutputCacheUI;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.StringSelection;
import java.io.IOException;

public class DefaultMISAOutputCacheUI extends MISAOutputCacheUI {

    private JButton firstButton;
    private JPopupMenu additionalActionsMenu;

    public DefaultMISAOutputCacheUI(MISAOutput misaOutput, MISACache cache) {
        super(misaOutput, cache);

        setLayout(new BoxLayout(this, BoxLayout.LINE_AXIS));
        add(Box.createHorizontalGlue());

        initialize();
    }

    protected void initialize() {

        if(getFilesystemPath() == null)
            return;

        AbstractButton openFolderButton = createButton("Open folder", UIUtils.getIconFromResources("open.png"));
        openFolderButton.addActionListener(e -> {
            if(Desktop.isDesktopSupported()) {
                try {
                    Desktop.getDesktop().open(getFilesystemPath().toFile());
                } catch (IOException e1) {
                    throw new RuntimeException(e1);
                }
            }
        });

        AbstractButton copyPathButton = createButton("Copy path", UIUtils.getIconFromResources("copy.png"));
        copyPathButton.addActionListener(e -> {
            StringSelection stringSelection = new StringSelection(getFilesystemPath().toString());
            Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
            clipboard.setContents(stringSelection, null);
        });

    }

    protected AbstractButton createButton(String text, Icon icon) {
        if(firstButton == null) {
            firstButton = new JButton(text, icon);
            add(firstButton);
            return firstButton;
        }
        else {
            if(additionalActionsMenu == null) {
                JButton additionalActions = new JButton("...");
                additionalActions.setMaximumSize(new Dimension(1,(int)firstButton.getPreferredSize().getHeight()));
                additionalActionsMenu = UIUtils.addPopupMenuToComponent(additionalActions);
                add(additionalActions);
            }

            JMenuItem item = new JMenuItem(text, icon);
            additionalActionsMenu.add(item);
            return item;
        }
    }

}

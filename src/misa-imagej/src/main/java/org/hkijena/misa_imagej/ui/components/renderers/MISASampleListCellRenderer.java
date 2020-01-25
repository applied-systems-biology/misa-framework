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

package org.hkijena.misa_imagej.ui.components.renderers;

import org.hkijena.misa_imagej.api.MISASample;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.MonochromeColorIcon;

import javax.swing.*;
import java.awt.*;

public class MISASampleListCellRenderer extends JLabel implements ListCellRenderer<MISASample> {

    private MonochromeColorIcon icon = new MonochromeColorIcon(UIUtils.getIconFromResources("sample-template.png"));

    public MISASampleListCellRenderer() {
        this.setIcon(icon);
        setBorder(BorderFactory.createEmptyBorder(4, 4, 4, 4));
    }

    @Override
    public Component getListCellRendererComponent(JList<? extends MISASample> list, MISASample value, int index, boolean isSelected, boolean cellHasFocus) {

        if(value == null) {
            setText("Nothing selected");
            return this;
        }

        setText(value.getName());
        icon.setColor(value.toColor());

        if(isSelected || cellHasFocus) {
            setBackground(new Color(184, 207, 229));
        }
        else {
            setBackground(new Color(255,255,255));
        }

        return this;
    }
}

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
import javax.swing.table.TableCellRenderer;
import java.awt.*;

public class MISASampleTableCellRender extends JLabel implements TableCellRenderer {

    private MonochromeColorIcon icon = new MonochromeColorIcon(UIUtils.getIconFromResources("sample-template.png"));

    @Override
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        if(value instanceof MISASample) {
            setText(((MISASample) value).getName());
            icon.setColor(((MISASample) value).toColor());
            setIcon(icon);

            if(isSelected || hasFocus) {
                setBackground(new Color(184, 207, 229));
            }
            else {
                setBackground(new Color(255,255,255));
            }
        }
        else {
            setText(null);
            setIcon(null);
        }
        return this;
    }
}

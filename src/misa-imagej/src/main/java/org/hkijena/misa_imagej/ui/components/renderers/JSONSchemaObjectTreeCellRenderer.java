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

import org.hkijena.misa_imagej.api.json.JSONSchemaObject;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeCellRenderer;
import java.awt.*;

public class JSONSchemaObjectTreeCellRenderer extends JLabel implements TreeCellRenderer {
    public JSONSchemaObjectTreeCellRenderer() {
        setOpaque(true);
        setBorder(BorderFactory.createEmptyBorder(4, 4, 4, 4));
    }

    @Override
    public Component getTreeCellRendererComponent(JTree jTree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {

        if(jTree.getFont() != null) {
            setFont(jTree.getFont());
        }

        Object o = ((DefaultMutableTreeNode)value).getUserObject();
        if(o instanceof JSONSchemaObject) {
            setText(((JSONSchemaObject)o).getDocumentationTitle());
            JSONSchemaObject entry = (JSONSchemaObject)o;
            setIcon(entry.getType().getIcon());
            setToolTipText(((JSONSchemaObject) o).getTooltip());
        }
        else {
            setText(o.toString());
            setIcon(null);
        }

        // Update status
        // Update status
        if(selected) {
            setBackground(new Color(184, 207, 229));
        }
        else {
            setBackground(new Color(255,255,255));
        }

        return this;
    }
}

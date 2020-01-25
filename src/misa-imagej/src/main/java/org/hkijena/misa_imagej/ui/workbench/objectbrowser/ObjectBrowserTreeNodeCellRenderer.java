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

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISASample;
import org.hkijena.misa_imagej.api.json.JSONSchemaObject;
import org.hkijena.misa_imagej.api.workbench.MISAAttachmentDatabase;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.ColorIcon;
import org.hkijena.misa_imagej.utils.ui.MonochromeColorIcon;

import javax.swing.*;
import javax.swing.tree.TreeCellRenderer;
import java.awt.*;

public class ObjectBrowserTreeNodeCellRenderer extends JPanel implements TreeCellRenderer {

    private MISAAttachmentDatabase database;
    private JLabel iconLabel = new JLabel();
    private JLabel mainLabel = new JLabel();
    private JLabel additionalInfoLabel = new JLabel();

    private MonochromeColorIcon sampleIcon = new MonochromeColorIcon(UIUtils.getIconFromResources("sample-template.png"), Color.WHITE);
    private MonochromeColorIcon objectIcon = new MonochromeColorIcon(UIUtils.getIconFromResources("object-template.png"), Color.WHITE);
    private ColorIcon cacheIcon = new ColorIcon(16,16);

    public ObjectBrowserTreeNodeCellRenderer(MISAAttachmentDatabase database) {
        this.database = database;
        setOpaque(true);
        setBorder(BorderFactory.createEmptyBorder(4, 4, 4, 4));

        setLayout(new GridBagLayout());
        add(iconLabel, new GridBagConstraints() {
            {
                gridx = 0;
                gridy = 0;
                insets = new Insets(0, 0, 0, 4);
            }
        });
        add(mainLabel, new GridBagConstraints() {
            {
                gridx = 1;
                gridy = 0;
                weightx = 1;
                fill = GridBagConstraints.HORIZONTAL;
//                insets = new Insets(4, 4, 0, 4);
            }
        });
        add(additionalInfoLabel, new GridBagConstraints() {
            {
                gridx = 1;
                gridy = 1;
                weightx = 1;
                fill = GridBagConstraints.HORIZONTAL;
//                insets = new Insets(0, 4, 4, 4);
            }
        });
        additionalInfoLabel.setFont(additionalInfoLabel.getFont().deriveFont(11.0f));
    }

    @Override
    public Component getTreeCellRendererComponent(JTree tree, Object treeNode, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {

        if (treeNode instanceof ObjectBrowserTreeNode) {
            ObjectBrowserTreeNode.Role role = ((ObjectBrowserTreeNode) treeNode).getRole();
            String value = (String)((ObjectBrowserTreeNode) treeNode).getUserObject();

            switch (role) {
                case Root:
                    mainLabel.setText("Objects");
                    additionalInfoLabel.setText(null);
                    iconLabel.setIcon(UIUtils.getIconFromResources("object.png"));
                    break;
                case Sample:
                    mainLabel.setText(value);
                    additionalInfoLabel.setText(null);
                    sampleIcon.setColor(MISASample.nameToColor(value));
                    iconLabel.setIcon(sampleIcon);
                    break;
                case SerializationId: {
                    JSONSchemaObject schema = database.getMisaOutput().getAttachmentSchemas().getOrDefault(value, null);
                    if (schema != null && schema.getDocumentationTypeTitle() != null && !schema.getDocumentationTypeTitle().isEmpty()) {
                        mainLabel.setText(schema.getDocumentationTypeTitle());
                        additionalInfoLabel.setText(value);
                        objectIcon.setColor(schema.toColor());
                        iconLabel.setIcon(objectIcon);
                    } else {
                        mainLabel.setText(value);
                        additionalInfoLabel.setText(null);
                        objectIcon.setColor(UIUtils.stringToColor(value, 0.8f, 0.8f));
                        iconLabel.setIcon(objectIcon);
                    }
                }
                break;
                case Property:
                    mainLabel.setText(value);
                    additionalInfoLabel.setText(null);
                    iconLabel.setIcon(UIUtils.getIconFromResources("label.png"));
                    break;
                case SerializationNamespace:
                    mainLabel.setText(value);
                    additionalInfoLabel.setText(null);
                    iconLabel.setIcon(UIUtils.getIconFromResources("module.png"));
                    break;
                case CacheAndSubCache: {
                    MISASample sample = database.getMisaOutput().getModuleInstance().getOrCreateAnySample();
                    MISACache matchingCache = sample.findMatchingCache(value);

                    if (matchingCache != null) {
                        mainLabel.setText(matchingCache.getFullRelativePath());
                        additionalInfoLabel.setText(value);
                        cacheIcon.setColor(matchingCache.toColor());
                        iconLabel.setIcon(cacheIcon);
                    }
                    else {
                        mainLabel.setText(value);
                        additionalInfoLabel.setText(null);
                        iconLabel.setIcon(UIUtils.getIconFromResources("database.png"));
                    }

                }
                break;
                case Cache: {
                    MISASample sample = database.getMisaOutput().getModuleInstance().getOrCreateAnySample();
                    MISACache matchingCache = sample.findMatchingCache(value);

                    if (matchingCache != null) {
                        mainLabel.setText(matchingCache.getFullRelativePath());
                        additionalInfoLabel.setText(matchingCache.getCacheTypeName());
                        cacheIcon.setColor(matchingCache.toColor());
                        iconLabel.setIcon(cacheIcon);
                    }
                    else {
                        mainLabel.setText(value);
                        additionalInfoLabel.setText(null);
                        iconLabel.setIcon(UIUtils.getIconFromResources("database.png"));
                    }
                }
                break;
                case SubCache: {
                    mainLabel.setText(value);
                    additionalInfoLabel.setText(null);
                    iconLabel.setIcon(UIUtils.getIconFromResources("database.png"));
                }
                break;
                default:
                    mainLabel.setText("" + value);
                    additionalInfoLabel.setText(null);
                    iconLabel.setIcon(null);
                    break;
            }
        } else {
            mainLabel.setText("" + treeNode);
            additionalInfoLabel.setText(null);
            iconLabel.setIcon(null);
        }

        if (selected) {
            setBackground(new Color(184, 207, 229));
        } else {
            setBackground(new Color(255, 255, 255));
        }

        return this;
    }
}

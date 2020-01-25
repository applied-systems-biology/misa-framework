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

package org.hkijena.misa_imagej.ui.json;

import com.google.common.eventbus.Subscribe;
import org.hkijena.misa_imagej.api.json.JSONSchemaObject;
import org.hkijena.misa_imagej.ui.components.renderers.JSONSchemaObjectTreeCellRenderer;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.jdesktop.swingx.JXTextField;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeSelectionModel;
import java.awt.*;

import static org.hkijena.misa_imagej.utils.UIUtils.UI_PADDING;

/**
 * Editor widget for a JSONSchema. Has a node tree on the left side.
 */
public class JSONSchemaEditorUI extends JPanel {

    private JTree jsonTree;
    private JPanel objectEditor;
    private JComponent topPanel;


    private int objectEditorRows = 0;
    private JSONSchemaObject lastObjectEditorSchemaParent = null;

    private JToggleButton enableObjectsButton;
    private JToggleButton showAllObjects;
    private JXTextField objectFilter;

    private JSONSchemaObject displayedSchema = null;
    private JSONSchemaObject schema = null;

    /**
     * Creates a JSON schema editor with a panel on top of the tree
     * @param topPanel if null, no panel will be created
     */
    public JSONSchemaEditorUI(JComponent topPanel) {
        this.topPanel = topPanel;
        initialize();
        setSchema(null);
    }

    public JSONSchemaEditorUI() {
        this(null);
    }

    private void setDisplayedSchema(JSONSchemaObject obj) {
        this.displayedSchema = obj;
        updateEditor();
    }

    private void updateEditor() {
        objectEditor.removeAll();
        objectEditorRows = 0;
        lastObjectEditorSchemaParent = null;
        objectEditor.revalidate();
        objectEditor.repaint();

        if(displayedSchema != null) {
            JSONSchemaEditorRegistry.getEditorFor(displayedSchema).populate(this);
            objectEditor.add(new JPanel(), new GridBagConstraints() {
                {
                    anchor = GridBagConstraints.PAGE_START;
                    gridx = 2;
                    gridy = objectEditorRows++;
                    fill = GridBagConstraints.HORIZONTAL | GridBagConstraints.VERTICAL;
                    weightx = 0;
                    weighty = 1;
                }
            });
        }
    }

    /**
     * Inserts an object editor UI into this schema editor
     * @param ui
     */
    public void insertObjectEditorUI(JSONSchemaObjectEditorUI ui, boolean withLabel) {
        // Filtering
        if(!enableObjectsButton.isSelected() && ui.getJsonSchemaObject().getParent() != displayedSchema)
            return;
        if(objectFilter.getText() != null && !objectFilter.getText().isEmpty()) {
            String searchText = ui.getJsonSchemaObject().getName().toLowerCase();
            if(!searchText.contains(objectFilter.getText().toLowerCase())) {
                return;
            }
        }

        JSONSchemaObject parent = ui.getJsonSchemaObject().getParent();

        if(parent != null && parent != lastObjectEditorSchemaParent && parent.getDepth() >= displayedSchema.getDepth()) {
            String parentPath = displayedSchema.getId() + parent.getValuePath().substring(displayedSchema.getValuePath().length());

            // Announce the object
            final boolean first = lastObjectEditorSchemaParent == null;
            lastObjectEditorSchemaParent = parent;
            JLabel description = new JLabel(parentPath);
            description.setIcon(parent.getType().getIcon());
            description.setFont(description.getFont().deriveFont(14.0f));
            objectEditor.add(description, new GridBagConstraints() {
                {
                    anchor = GridBagConstraints.WEST;
                    gridx = 0;
                    gridy = objectEditorRows++;
                    gridwidth = 2;
                    weightx = 0;
                    insets = new Insets(first ? 8 : 24,4,8,4);
                }
            });
        }
        if(withLabel) {
            JLabel description = new JLabel(ui.getJsonSchemaObject().getDocumentationTitle());
            description.setIcon(ui.getJsonSchemaObject().getType().getIcon());
            description.setToolTipText(ui.getJsonSchemaObject().getTooltip());
            objectEditor.add(description, new GridBagConstraints() {
                {
                    anchor = GridBagConstraints.WEST;
                    gridx = 0;
                    gridy = objectEditorRows;
                    weightx = 0;
                    insets = UI_PADDING;
                }
            });
        }
        objectEditor.add(ui, new GridBagConstraints() {
            {
                anchor = GridBagConstraints.WEST;
                gridx = 1;
                gridy = objectEditorRows;
                insets = UI_PADDING;
                weightx = 1;
                fill = GridBagConstraints.HORIZONTAL;
            }
        });
        ++objectEditorRows;
    }

    private void initialize() {
        setLayout(new BorderLayout());

        JPanel treePanel = new JPanel(new BorderLayout());
        {
            // If enabled, add panel
            if(topPanel != null) {
                treePanel.add(topPanel, BorderLayout.NORTH);
            }

            // Create tree
            jsonTree = new JTree();
            jsonTree.setCellRenderer(new JSONSchemaObjectTreeCellRenderer());
            jsonTree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
            treePanel.add(new JScrollPane(jsonTree) {
                {
                    setMinimumSize(new Dimension(128, 0));
                }
            }, BorderLayout.CENTER);
        }

        JPanel editPanel = new JPanel(new BorderLayout());
        {
            // Create a toolbar with view options
            JToolBar toolBar = new JToolBar();

            enableObjectsButton = new JToggleButton("Objects", UIUtils.getIconFromResources("object.png"), true);
            enableObjectsButton.setToolTipText("If enabled, object parameters are shown in the editor.");
            enableObjectsButton.addActionListener(actionEvent -> updateEditor());
            toolBar.add(enableObjectsButton);

            showAllObjects = new JToggleButton("Whole tree", UIUtils.getIconFromResources("tree.png"), false);
            showAllObjects.setToolTipText("If enabled, the whole subtree of settings is shown.");
            showAllObjects.addActionListener(actionEvent -> updateEditor());
            toolBar.add(showAllObjects);

            toolBar.add(Box.createHorizontalGlue());

            objectFilter = new JXTextField("Filter ...");
            objectFilter.getDocument().addDocumentListener(new DocumentListener() {
                @Override
                public void insertUpdate(DocumentEvent documentEvent) {
                    setDisplayedSchema(displayedSchema);
                }

                @Override
                public void removeUpdate(DocumentEvent documentEvent) {
                    setDisplayedSchema(displayedSchema);
                }

                @Override
                public void changedUpdate(DocumentEvent documentEvent) {
                    setDisplayedSchema(displayedSchema);
                }
            });
            toolBar.add(objectFilter);

            JButton clearFilterButton = new JButton(UIUtils.getIconFromResources("clear.png"));
            clearFilterButton.addActionListener(actionEvent -> objectFilter.setText(""));
            toolBar.add(clearFilterButton);

            editPanel.add(toolBar, BorderLayout.NORTH);

            // Add the scroll layout here
            objectEditor = new JPanel(new GridBagLayout());
            editPanel.add(new JScrollPane(objectEditor), BorderLayout.CENTER);
        }

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, treePanel, editPanel);
        splitPane.setResizeWeight(0);
        add(splitPane, BorderLayout.CENTER);

        jsonTree.addTreeSelectionListener(e -> {
            if(jsonTree.getLastSelectedPathComponent() != null) {
                DefaultMutableTreeNode nd = (DefaultMutableTreeNode)jsonTree.getLastSelectedPathComponent();
                setDisplayedSchema((JSONSchemaObject)nd.getUserObject());
            }
        });

    }

    public void setSchema(JSONSchemaObject jsonSchema) {
        this.schema = jsonSchema;
        if(jsonSchema != null && jsonSchema.getProperties() != null && jsonSchema.getProperties().size() > 0) {
            jsonTree.setModel(new DefaultTreeModel(jsonSchema.toTreeNode()));
            setDisplayedSchema(jsonSchema);
            jsonSchema.getEventBus().register(this);

            jsonTree.setEnabled(true);
            objectEditor.setEnabled(true);
        }
        else {
            jsonTree.setModel(new DefaultTreeModel(new DefaultMutableTreeNode("No properties to edit")));
            setDisplayedSchema(null);
            jsonTree.setEnabled(false);
            objectEditor.setEnabled(false);
        }
    }

    @Subscribe
    public void handleSchemaEvent(JSONSchemaObject.AddedAdditionalPropertyEvent event) {
        if(event.getProperty().getParent() == displayedSchema) {
            jsonTree.setModel(new DefaultTreeModel(event.getProperty().getParent().toTreeNode()));
            refreshEditor();
        }
    }

    @Subscribe
    public void handleSchemaEvent(JSONSchemaObject.RemovedAdditionalPropertyEvent event) {
        if(event.getProperty().getParent() == displayedSchema) {
            jsonTree.setModel(new DefaultTreeModel(event.getProperty().getParent().toTreeNode()));
            refreshEditor();
        }
    }

    public void refreshEditor() {
        if(schema != null) {
            setSchema(schema);
        }
    }

    /**
     * If true, the populated objects should be limited
     * @return
     */
    public boolean getObjectLimitEnabled() {
        return !showAllObjects.isSelected();
    }

}

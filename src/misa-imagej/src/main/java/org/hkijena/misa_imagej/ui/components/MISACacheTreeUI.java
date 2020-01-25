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

package org.hkijena.misa_imagej.ui.components;

import com.google.common.eventbus.EventBus;
import org.apache.commons.collections.ListUtils;
import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISASample;
import org.hkijena.misa_imagej.utils.ui.ColorIcon;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeCellRenderer;
import javax.swing.tree.TreeSelectionModel;
import java.awt.*;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

public class MISACacheTreeUI extends JTree {

    private MISASample sample;
    private Entry currentEntry;
    private EventBus eventBus = new EventBus();

    public MISACacheTreeUI() {
        initialize();
    }

    private void initialize() {
        this.setCellRenderer(new CellRenderer());
        this.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        addTreeSelectionListener(treeSelectionEvent -> {
            if(this.getLastSelectedPathComponent() != null) {
                DefaultMutableTreeNode node = (DefaultMutableTreeNode)this.getLastSelectedPathComponent();
                setCurrentCacheList((Entry)node.getUserObject());
            }
        });
        refresh();
    }

    public MISASample getSample() {
        return this.sample;
    }

    public void setSample(MISASample sample) {
        this.sample = sample;
        refresh();
    }

    public Entry getRootCacheList() {
        return (Entry)((DefaultMutableTreeNode)getModel().getRoot()).getUserObject();
    }

    public Entry getCurrentCacheList() {
        return currentEntry;
    }

    public void setCurrentCacheList(Entry entry) {
        this.currentEntry = entry;
        eventBus.post(new ChangedCurrentCacheListEvent(this));
    }

    protected Entry createRootEntry() {
        return new MISACacheTreeUI.Entry("'" + sample.getName() + "' data",
                ListUtils.union(sample.getImportedCaches(), sample.getExportedCaches()));
    }

    protected Entry createEntry(MISACache cache) {
        return new MISACacheTreeUI.Entry(cache.getRelativePathName(), Arrays.asList(cache));
    }

    private void refresh() {
        if(sample != null) {
            // Create tree nodes
            currentEntry = createRootEntry();
            DefaultMutableTreeNode rootNode = new DefaultMutableTreeNode(currentEntry);

            // Imported data
            DefaultMutableTreeNode importedNode = new DefaultMutableTreeNode(new MISACacheTreeUI.Entry("Input", sample.getImportedCaches()));
            for(MISACache cache : sample.getImportedCaches()) {
                importedNode.add(new DefaultMutableTreeNode(createEntry(cache)));
            }
            rootNode.add(importedNode);

            // Exported data
            DefaultMutableTreeNode exportedNode = new DefaultMutableTreeNode(new MISACacheTreeUI.Entry("Output", sample.getExportedCaches()));
            for(MISACache cache : sample.getExportedCaches()) {
                exportedNode.add(new DefaultMutableTreeNode(createEntry(cache)));
            }
            rootNode.add(exportedNode);

            this.setModel(new DefaultTreeModel(rootNode));
            setCurrentCacheList(currentEntry);
        }
        else {
            this.setModel(new DefaultTreeModel(new DefaultMutableTreeNode("No properties to edit")));
            setCurrentCacheList(null);
        }
    }

    public EventBus getEventBus() {
        return eventBus;
    }

    public static class CellRenderer extends JLabel implements TreeCellRenderer {

        private ColorIcon icon;

        public CellRenderer() {
            setOpaque(true);
            setBorder(BorderFactory.createEmptyBorder(4, 4, 4, 4));
            icon = new ColorIcon(16, 16, Color.BLACK);
        }

        @Override
        public Component getTreeCellRendererComponent(JTree jTree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {

            if(jTree.getFont() != null) {
                setFont(jTree.getFont());
            }

            Object o = ((DefaultMutableTreeNode)value).getUserObject();
            if(o instanceof Entry) {
                setText(o.toString());
                Entry entry = (Entry)o;
                if(entry.caches.size() == 1) {
                    icon.setColor(entry.caches.get(0).toColor());
                    setIcon(icon);
                }
                else {
                    setIcon(null);
                }
            }
            else {
                setText(o.toString());
                setIcon(null);
            }

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

    public static class Entry {
        public java.util.List<MISACache> caches;
        public String name;

        public Entry() {
        }

        public Entry(String name, List<MISACache> caches) {
            this.caches = caches;
            this.name = name;

            // Sort, so imported items are on top
            this.caches.sort(Comparator.comparing(MISACache::getIOType).thenComparing(MISACache::getRelativePath).thenComparing(MISACache::getCacheTypeName));
        }

        @Override
        public String toString() {
            return name;
        }
    }

    public static class ChangedCurrentCacheListEvent {
        private MISACacheTreeUI tree;

        public ChangedCurrentCacheListEvent(MISACacheTreeUI tree) {
            this.tree = tree;
        }

        public MISACacheTreeUI getTree() {
            return tree;
        }
    }
}

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

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import java.util.ArrayList;
import java.util.List;

/**
 * Remembers expanded tree nodes
 */
public class ObjectBrowserTreeSnapshot {
    private JTree tree;
    private List<List<String>> paths = new ArrayList<>();

    public ObjectBrowserTreeSnapshot(JTree tree) {
        this.tree = tree;
    }

    public void createSnapshot() {
        paths.clear();
        for(int i = 0; i < tree.getRowCount(); ++i) {
            if(tree.isExpanded(i)) {
                TreePath path = tree.getPathForRow(i);
                List<String> namePath = new ArrayList<>();
                for(Object segment : path.getPath()) {
                    if(segment instanceof DefaultMutableTreeNode) {
                        namePath.add("" + ((DefaultMutableTreeNode) segment).getUserObject());
                    }
                    else {
                        namePath = null;
                        break;
                    }
                }
                if(namePath != null)
                    paths.add(namePath);
            }
        }
    }

    private void expandPath(List<String> path) {
        if(tree.getModel() != null && tree.getModel().getRoot() instanceof TreeNode) {
            TreeNode current = (TreeNode) tree.getModel().getRoot();
            ArrayList<Object> currentPath = new ArrayList<>();
            currentPath.add(current);

            for(int i = 1; i < path.size(); ++i) {

                // Expand the current path
                tree.expandPath(new TreePath(currentPath.toArray()));

                String segment = path.get(i);

                boolean found = false;
                for(int j = 0; j < current.getChildCount(); ++j) {
                    TreeNode child = current.getChildAt(j);
                    if(child instanceof DefaultMutableTreeNode) {
                        if(segment.equals(((DefaultMutableTreeNode) child).getUserObject())) {
                            current = child;
                            found = true;
                            break;
                        }
                    }
                }

                if(found) {
                    currentPath.add(current);
                }
                else {
                    return;
                }
            }

            // Expand the current path
            tree.expandPath(new TreePath(currentPath.toArray()));
        }
    }

    public void restoreSnapshot() {
        for(List<String> path : paths) {
            expandPath(path);
        }
    }
}

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
import org.hkijena.misa_imagej.api.workbench.MISAAttachmentDatabase;
import org.hkijena.misa_imagej.utils.SQLUtils;

import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class ObjectBrowserTreeNode extends DefaultMutableTreeNode {

    public Role getRole() {
        return role;
    }

    public enum Role {
        Root,
        SerializationNamespace,
        SerializationId,
        CacheAndSubCache,
        Cache,
        SubCache,
        Sample,
        Property
    }

    private MISAAttachmentDatabase database;
    private Role[] roleAssignment;
    private String[] knownValues;
    private boolean loaded = false;
    private Role role = Role.Root;

    public ObjectBrowserTreeNode(MISAAttachmentDatabase database, Role[] roleAssignment, String[] knownValues) {
        this.database = database;
        this.roleAssignment = roleAssignment;
        this.knownValues = knownValues;
        setAllowsChildren(true);
        add(new DefaultMutableTreeNode("Loading ..."));
        for(int i = knownValues.length - 1; i >= 0; --i) {
            if(knownValues[i] != null) {
                setUserObject(knownValues[i]);
                role = roleAssignment[i];
                break;
            }
        }
    }

    public void loadDatabaseEntries(DefaultTreeModel model) {
        if(loaded)
            return;

        int childrenRoleIndex = getFirstUnknownValue();
        removeAllChildren();
        try (ResultSet resultSet = getChildDatabaseEntries()){
            Set<String > knownChildren = new HashSet<>();
            while(resultSet.next()) {
                String childValue = resultSet.getString(1);

                if(roleAssignment[childrenRoleIndex] == Role.SerializationNamespace) {
                    // Modify
                    childValue = childValue.substring(0, childValue.indexOf(":"));
                }
                else if(roleAssignment[childrenRoleIndex] == Role.Cache) {
                    MISASample sample = database.getMisaOutput().getModuleInstance().getOrCreateAnySample();
                    MISACache cache = sample.findMatchingCache(childValue);
                    if(cache != null) {
                        childValue = cache.getFullRelativePath();
                    }
                }
                else if(roleAssignment[childrenRoleIndex] == Role.SubCache) {
                    MISASample sample = database.getMisaOutput().getModuleInstance().getOrCreateAnySample();
                    MISACache cache = sample.findMatchingCache(childValue);
                    if(cache != null) {
                        childValue = childValue.substring(cache.getFullRelativePath().length());
                    }
                }

                if(knownChildren.contains(childValue))
                    continue;
                knownChildren.add(childValue);
                String[] childKnownValues = knownValues.clone();
                childKnownValues[childrenRoleIndex] = childValue;

                ObjectBrowserTreeNode childNode = new ObjectBrowserTreeNode(database, roleAssignment, childKnownValues);
                add(childNode);
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }

        loaded = true;
        model.nodeStructureChanged(this);
    }

    public List<String> getFilters() {
        List<String> filters = new ArrayList<>();
        for(int i = 0; i < roleAssignment.length; ++i) {
            if(knownValues[i] != null) {
                switch (roleAssignment[i]) {
                    case SerializationNamespace:
                        filters.add("\"serialization-id\" like '" + SQLUtils.escapeWildcardsForSQLite(knownValues[i]) + ":%' escape '\\'" );
                        break;
                    case SerializationId:
                        filters.add("\"serialization-id\" is " + SQLUtils.value(knownValues[i]) );
                        break;
                    case Cache:
                        filters.add("cache like '" + SQLUtils.escapeWildcardsForSQLite(knownValues[i]) + "%' escape '\\'" );
                        break;
                    case SubCache:
                        filters.add("cache like '%" + SQLUtils.escapeWildcardsForSQLite(knownValues[i]) + "' escape '\\'" );
                        break;
                    case CacheAndSubCache:
                        filters.add("cache is " + SQLUtils.value(knownValues[i]) );
                        break;
                    case Sample:
                        filters.add("sample is " + SQLUtils.value(knownValues[i]) );
                        break;
                    case Property:
                        filters.add("property is " + SQLUtils.value(knownValues[i]) );
                        break;
                }
            }
        }
        return filters;
    }

    public ResultSet getChildDatabaseEntries() {
        StringBuilder sql = new StringBuilder();

        int childrenRoleIndex = getFirstUnknownValue();

        sql.append("distinct ");

        switch (roleAssignment[childrenRoleIndex]) {
            case SerializationNamespace:
                sql.append("\"serialization-id\"");
                break;
            case SerializationId:
                sql.append("\"serialization-id\"");
                break;
            case CacheAndSubCache:
                sql.append("cache");
                break;
            case Cache:
                sql.append("cache");
                break;
            case SubCache:
                sql.append("cache");
                break;
            case Sample:
                sql.append("sample");
                break;
            case Property:
                sql.append("property");
                break;
        }

        return database.query(sql.toString(), getFilters(), "");
    }

    private boolean hasUnknownValue() {
        return getFirstUnknownValue() != knownValues.length;
    }

    private int getFirstUnknownValue() {
        for(int i = 0; i < knownValues.length; ++i) {
            if(knownValues[i] == null)
                return i;
        }
        return knownValues.length;
    }

    @Override
    public boolean isLeaf() {
       return !hasUnknownValue();
    }
}

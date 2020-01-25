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

package org.hkijena.misa_imagej.ui.registries;

import org.hkijena.misa_imagej.ui.workbench.tableanalyzer.MISATableToTableOperation;
import org.hkijena.misa_imagej.ui.workbench.tableanalyzer.MISATableToTableOperationUI;
import org.hkijena.misa_imagej.ui.workbench.tableanalyzer.MISATableVectorOperation;
import org.hkijena.misa_imagej.ui.workbench.tableanalyzer.MISATableVectorOperationUI;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

public class MISATableAnalyzerUIOperationRegistry {
    private Map<Class<? extends MISATableVectorOperation>, VectorOperationEntry> vectorOperationEntries = new HashMap<>();
    private Map<Class<? extends MISATableToTableOperation>, TableToTableOperationEntry> tableToTableOperationEntries = new HashMap<>();

    public void register(Class<? extends MISATableVectorOperation> operationClass,
                         Class<? extends MISATableVectorOperationUI> uiClass,
                         String name,
                         String shortcut,
                         String description,
                         Icon icon) {
        vectorOperationEntries.put(operationClass, new VectorOperationEntry(operationClass,
                uiClass,
                name,
                shortcut,
                description,
                icon));
    }

    public MISATableVectorOperationUI createUIForVectorOperation(MISATableVectorOperation operation) {
        try {
            if(vectorOperationEntries.get(operation.getClass()).getUiClass() == null)
                return null;
            return vectorOperationEntries.get(operation.getClass()).getUiClass().getConstructor(MISATableVectorOperation.class).newInstance(operation);
        } catch (InstantiationException | IllegalAccessException | InvocationTargetException | NoSuchMethodException e) {
            throw new RuntimeException(e);
        }
    }

    public MISATableToTableOperationUI createUIForTableToTableOperation(MISATableToTableOperation operation) {
        try {
            if(tableToTableOperationEntries.get(operation.getClass()).getUiClass() == null)
                return null;
            return tableToTableOperationEntries.get(operation.getClass()).getUiClass().getConstructor(MISATableToTableOperation.class).newInstance(operation);
        } catch (InstantiationException | IllegalAccessException | InvocationTargetException | NoSuchMethodException e) {
            throw new RuntimeException(e);
        }
    }

    public Collection<VectorOperationEntry> getVectorOperationEntries() {
        return vectorOperationEntries.values();
    }

    public Collection<TableToTableOperationEntry> getTableToTableOperationEntries() { return tableToTableOperationEntries.values(); }

    public String getNameOf(MISATableVectorOperation operation) {
        return vectorOperationEntries.get(operation.getClass()).getName();
    }

    public String getShortcutOf(MISATableVectorOperation operation) {
        return vectorOperationEntries.get(operation.getClass()).getShortcut();
    }

    public Icon getIconOf(MISATableVectorOperation operation) {
        return vectorOperationEntries.get(operation.getClass()).getIcon();
    }

    public String getNameOf(MISATableToTableOperation operation) {
        return tableToTableOperationEntries.get(operation.getClass()).getName();
    }

    public Icon getIconOf(MISATableToTableOperation operation) {
        return tableToTableOperationEntries.get(operation.getClass()).getIcon();
    }

    public static class VectorOperationEntry {
        private Class<? extends MISATableVectorOperation> operationClass;
        private Class<? extends MISATableVectorOperationUI> uiClass;
        private String name;
        private String shortcut;
        private String description;
        private Icon icon;

        public VectorOperationEntry(Class<? extends MISATableVectorOperation> operationClass, Class<? extends MISATableVectorOperationUI> uiClass, String name, String shortcut, String description, Icon icon) {
            this.operationClass = operationClass;
            this.uiClass = uiClass;
            this.name = name;
            this.shortcut = shortcut;
            this.description = description;
            this.icon = icon;
        }

        public Class<? extends MISATableVectorOperationUI> getUiClass() {
            return uiClass;
        }

        public String getName() {
            return name;
        }

        public String getDescription() {
            return description;
        }

        public Icon getIcon() {
            return icon;
        }

        public Class<? extends MISATableVectorOperation> getOperationClass() {
            return operationClass;
        }

        public MISATableVectorOperation instantiateOperation() {
            try {
                return operationClass.getConstructor().newInstance();
            } catch (InstantiationException | IllegalAccessException | InvocationTargetException | NoSuchMethodException e) {
                throw new RuntimeException(e);
            }
        }

        public String getShortcut() {
            return shortcut;
        }
    }

    public static class TableToTableOperationEntry {
        private Class<? extends MISATableToTableOperation> operationClass;
        private Class<? extends MISATableToTableOperationUI> uiClass;
        private String name;
        private String description;
        private Icon icon;

        public TableToTableOperationEntry(Class<? extends MISATableToTableOperation> operationClass, Class<? extends MISATableToTableOperationUI> uiClass, String name, String description, Icon icon) {
            this.operationClass = operationClass;
            this.uiClass = uiClass;
            this.name = name;
            this.description = description;
            this.icon = icon;
        }

        public Class<? extends MISATableToTableOperationUI> getUiClass() {
            return uiClass;
        }

        public String getName() {
            return name;
        }

        public String getDescription() {
            return description;
        }

        public Icon getIcon() {
            return icon;
        }

        public Class<? extends MISATableToTableOperation> getOperationClass() {
            return operationClass;
        }

        public MISATableToTableOperation instantiateOperation() {
            try {
                return operationClass.getConstructor().newInstance();
            } catch (InstantiationException | IllegalAccessException | InvocationTargetException | NoSuchMethodException e) {
                throw new RuntimeException(e);
            }
        }
    }
}

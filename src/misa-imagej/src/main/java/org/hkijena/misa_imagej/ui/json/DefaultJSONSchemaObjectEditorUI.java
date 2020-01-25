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

import org.hkijena.misa_imagej.api.json.JSONSchemaObject;
import org.hkijena.misa_imagej.api.json.JSONSchemaObjectType;
import org.hkijena.misa_imagej.utils.ui.DocumentChangeListener;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import java.awt.*;
import java.util.ArrayList;
import java.util.Comparator;

/**
 * Editor that can edit any JSON schema
 * This is used as fallback if the parameter is not serializeable or has no editor assigned to it
 */
public class DefaultJSONSchemaObjectEditorUI extends JSONSchemaObjectEditorUI {

    public DefaultJSONSchemaObjectEditorUI(JSONSchemaObject object) {
        super(object);
    }

    @Override
    public void populate(JSONSchemaEditorUI schemaEditorUI) {
        if(getJsonSchemaObject().getType() == JSONSchemaObjectType.jsonObject) {
            // Do not do anything. Instead create editors for the contained objects
            ArrayList<JSONSchemaObject> objects = new ArrayList<>(getJsonSchemaObject().getProperties().values());
            objects.sort(Comparator.comparingInt(JSONSchemaObject::getMaxDepth).thenComparing(JSONSchemaObject::getName));

            for(JSONSchemaObject obj : objects) {
                if(!schemaEditorUI.getObjectLimitEnabled() || obj.getMaxDepth() <= 1) {
                    JSONSchemaEditorRegistry.getEditorFor(obj).populate(schemaEditorUI);
                }
            }
        }
        else if(getJsonSchemaObject().getEnumValues() != null) {
            // Create a combo box within this panel
            setLayout(new BorderLayout());
            JComboBox<Object> comboBox = new JComboBox<>();
            comboBox.setToolTipText(getJsonSchemaObject().getTooltip());
            DefaultComboBoxModel<Object> model = new DefaultComboBoxModel<>();

            for(Object d : getJsonSchemaObject().getEnumValues()) {
                model.addElement(d);
                if(d.equals(getJsonSchemaObject().getValue())) {
                    model.setSelectedItem(d);
                }
            }
            comboBox.setModel(model);
            add(comboBox, BorderLayout.CENTER);

            comboBox.addActionListener(actionEvent -> {
                getJsonSchemaObject().setValue(model.getSelectedItem());
            });

            schemaEditorUI.insertObjectEditorUI(this, true);
        }
        else if(getJsonSchemaObject().getType() == JSONSchemaObjectType.jsonString) {
            // Create a string editor
            setLayout(new BorderLayout());
            JTextField edit = new JTextField(getJsonSchemaObject().getValue() != null ? (String) getJsonSchemaObject().getValue() : "");
            edit.setToolTipText(getJsonSchemaObject().getTooltip());
            if(getJsonSchemaObject().getValue() != null) {
                edit.setText((String) getJsonSchemaObject().getValue());
            }
            add(edit, BorderLayout.CENTER);

            edit.getDocument().addDocumentListener(new DocumentChangeListener() {
                @Override
                public void changed(DocumentEvent documentEvent) {
                    getJsonSchemaObject().setValue(edit.getText());
                }
            });

            schemaEditorUI.insertObjectEditorUI(this, true);
        }
        else if(getJsonSchemaObject().getType() == JSONSchemaObjectType.jsonNumber) {
            // Create a spinner where the user can edit the value
            setLayout(new BorderLayout());
            JSpinner edit = new JSpinner();
            Dimension dim = edit.getPreferredSize();
            edit.setModel(new SpinnerNumberModel(0.0, -Double.MAX_VALUE, Double.MAX_VALUE, 1));
            edit.setPreferredSize(dim);
            edit.setToolTipText(getJsonSchemaObject().getTooltip());

            if(getJsonSchemaObject().getValue() != null) {
                edit.setValue(getJsonSchemaObject().getValue());
            }
            add(edit, BorderLayout.CENTER);

            edit.addChangeListener(changeEvent -> {
                getJsonSchemaObject().setValue(edit.getValue());
            });
            schemaEditorUI.insertObjectEditorUI(this, true);
        }
        else if(getJsonSchemaObject().getType() == JSONSchemaObjectType.jsonBoolean) {
            setLayout(new BorderLayout());
            // Create a checkbox
            JCheckBox checkBox = new JCheckBox();
            checkBox.setToolTipText(getJsonSchemaObject().getTooltip());
            checkBox.setText(getJsonSchemaObject().getDocumentationTitle());

            if(getJsonSchemaObject().getValue() != null) {
                checkBox.setSelected((boolean) getJsonSchemaObject().getValue());
            }
            add(checkBox, BorderLayout.CENTER);
            checkBox.addActionListener(actionEvent -> {
                getJsonSchemaObject().setValue(checkBox.isSelected());
            });

            schemaEditorUI.insertObjectEditorUI(this, false);
        }
        else {
            throw new UnsupportedOperationException("Unknown schema object type " + getJsonSchemaObject().getType());
        }

    }

}

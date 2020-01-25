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

package org.hkijena.misa_imagej.api.workbench.table;

import com.google.gson.JsonPrimitive;
import org.hkijena.misa_imagej.api.MISAAttachment;
import org.hkijena.misa_imagej.api.json.JSONSchemaObject;
import org.hkijena.misa_imagej.api.json.JSONSchemaObjectType;

import java.sql.SQLException;
import java.util.Map;
import java.util.Stack;

public class MISAAttachmentTableJsonValueColumn implements MISAAttachmentTableColumn {

    private String propertyName;

    public MISAAttachmentTableJsonValueColumn(String propertyName) {
        this.propertyName = propertyName;
    }

    @Override
    public Object getValue(MISAAttachmentTable table, int id, String sample, String cache, String property, String serializationId, MISAAttachment attachment) throws SQLException {
        attachment.load();
        MISAAttachment.Property propertyInstance = attachment.getProperty(propertyName);
        if(propertyInstance instanceof MISAAttachment.MemoryProperty) {
            JsonPrimitive primitive =((MISAAttachment.MemoryProperty) propertyInstance).getValue();
            if(primitive.isNumber())
                return primitive.getAsNumber();
            else if(primitive.isString())
                return primitive.getAsString();
            else if (primitive.isBoolean())
                return primitive.getAsBoolean();
            else
                throw new UnsupportedOperationException();
        }
        else {
            return null;
        }
    }

    @Override
    public String getName() {
        return propertyName.substring(1);
    }

    /**
     * The propertyName that is queried from the attachment
     *
     * @return
     */
    public String getPropertyName() {
        return propertyName;
    }

    public static void addColumnsToTable(MISAAttachmentTable table, boolean addSubObjects) {
        JSONSchemaObject schema = table.getDatabase().getMisaOutput().getAttachmentSchemas().getOrDefault(
                table.getSerializationId(), null);
        if (schema != null) {
            Stack<JSONSchemaObject> stack = new Stack<>();
            Stack<String> paths = new Stack<>();
            stack.push(schema);
            paths.push("");

            while (!stack.isEmpty()) {
                JSONSchemaObject object = stack.pop();
                String path = paths.pop();
                switch (object.getType()) {
                    case jsonString:
                    case jsonNumber:
                    case jsonBoolean:
                        table.addColumn(new MISAAttachmentTableJsonValueColumn(path));
                        break;
                    case jsonObject:
                        for (Map.Entry<String, JSONSchemaObject> entry : object.getProperties().entrySet()) {
                            if (entry.getValue().getType() == JSONSchemaObjectType.jsonObject &&
                                    entry.getValue().getSerializationId() != null && !entry.getValue().getSerializationId().isEmpty()) {
                                if (!addSubObjects)
                                    continue;
                            }

                            stack.push(entry.getValue());
                            paths.push(path + "/" + entry.getKey());
                        }
                        break;
                }
            }
        }
    }
}

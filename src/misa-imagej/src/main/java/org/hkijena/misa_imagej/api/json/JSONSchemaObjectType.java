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

package org.hkijena.misa_imagej.api.json;

import com.google.gson.*;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.lang.reflect.Type;

public enum JSONSchemaObjectType {
    jsonObject,
    jsonString,
    jsonNumber,
    jsonBoolean,
    jsonArray;

    public Icon getIcon() {
        switch (this) {
            case jsonObject:
                return UIUtils.getIconFromResources("object.png");
            case jsonString:
                return UIUtils.getIconFromResources("text.png");
            case jsonBoolean:
                return UIUtils.getIconFromResources("checkbox.png");
            case jsonNumber:
                return UIUtils.getIconFromResources("number.png");
            default:
                return null;
        }
    }

    public static class JSONAdapter implements JsonDeserializer<JSONSchemaObjectType>, JsonSerializer<JSONSchemaObjectType> {
        @Override
        public JSONSchemaObjectType deserialize(JsonElement jsonElement, Type type, JsonDeserializationContext jsonDeserializationContext) throws JsonParseException {
            switch (jsonElement.getAsString()) {
                case "object":
                    return jsonObject;
                case "number":
                    return jsonNumber;
                case "boolean":
                    return jsonBoolean;
                case "string":
                    return jsonString;
                case "array":
                    return jsonArray;
                default:
                    throw new JsonParseException("Unknown type " + jsonElement.getAsString());
            }
        }

        @Override
        public JsonElement serialize(JSONSchemaObjectType jsonSchemaObjectType, Type type, JsonSerializationContext jsonSerializationContext) {
            switch(jsonSchemaObjectType) {
                case jsonObject:
                    return new JsonPrimitive("object");
                case jsonBoolean:
                    return new JsonPrimitive("boolean");
                case jsonString:
                    return new JsonPrimitive("string");
                case jsonNumber:
                    return new JsonPrimitive("number");
                case jsonArray:
                    return new JsonPrimitive("array");
                default:
                    throw new UnsupportedOperationException("Unknown schema object type " + jsonSchemaObjectType);
            }
        }
    }
}

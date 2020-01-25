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

import java.util.HashMap;
import java.util.Map;

/**
 * Manages available JSON schema object editors
 */
public class JSONSchemaEditorRegistry {

    private static Map<String, Class<? extends JSONSchemaObjectEditorUI>> registeredEditors = new HashMap<>();
    private static boolean isInitialized = false;

    private JSONSchemaEditorRegistry() {

    }

    /**
     * Registers an editor class for a serialization Id
     * @param serializationId
     * @param editorClass
     */
    public static void register(String serializationId, Class<JSONSchemaObjectEditorUI> editorClass) {
        registeredEditors.put(serializationId, editorClass);
    }

    /**
     * Creates an editor for the provided object
     * @param schemaObject
     * @return
     */
    public static JSONSchemaObjectEditorUI getEditorFor(JSONSchemaObject schemaObject) {
        if(isInitialized)
            initialize();
        if(schemaObject.getSerializationId() != null) {
            Class<? extends JSONSchemaObjectEditorUI> result = registeredEditors.getOrDefault(schemaObject.getSerializationId(), null);
            if(result != null) {
                try {
                    return result.getConstructor(JSONSchemaObject.class).newInstance(schemaObject);
                } catch (Exception e) {
                    e.printStackTrace();
                    throw new RuntimeException(e);
                }
            }
        }
        return new DefaultJSONSchemaObjectEditorUI(schemaObject);
    }

    /**
     * Initializes default editors
     */
    public static void initialize() {
        isInitialized = true;
    }
}

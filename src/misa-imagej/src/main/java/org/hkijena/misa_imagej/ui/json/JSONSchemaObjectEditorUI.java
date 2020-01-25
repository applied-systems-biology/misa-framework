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

import javax.swing.*;

/**
 * Base class of all JSON schema object editors
 */
public abstract class JSONSchemaObjectEditorUI extends JPanel {

    private JSONSchemaObject jsonSchemaObject;

    public JSONSchemaObjectEditorUI(JSONSchemaObject jsonSchemaObject) {
        this.jsonSchemaObject = jsonSchemaObject;
    }

    public JSONSchemaObject getJsonSchemaObject() {
        return jsonSchemaObject;
    }

    public abstract void populate(JSONSchemaEditorUI schemaEditorUI);
}

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

import com.google.gson.JsonElement;

public class JSONPropertyPathSegment implements JSONPathSegment {

    private String property;

    public JSONPropertyPathSegment(String property) {
        this.property = property;
    }

    @Override
    public JsonElement getElement(JsonElement jsonElement) {
        return jsonElement.getAsJsonObject().get(property);
    }

    @Override
    public String toString() {
        return property;
    }
}

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

package org.hkijena.misa_imagej.api;

import com.google.gson.JsonObject;
import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;
import java.util.List;

public class MISASerializable {
    @SerializedName("misa:serialization-id")
    public String serializationId = "misa:serializable";

    @SerializedName("misa:serialization-hierarchy")
    public List<String> serializationHierarchy = new ArrayList<>();

    /**
     * Full JSON data that is deserialized
     * This might contain additional properties that are not deserialized
     */
    public transient JsonObject rawData;

    @Override
    public String toString() {
        return serializationId;
    }
}

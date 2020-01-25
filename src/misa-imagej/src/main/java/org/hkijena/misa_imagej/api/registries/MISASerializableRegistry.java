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

package org.hkijena.misa_imagej.api.registries;

import com.google.common.collect.Lists;
import com.google.gson.Gson;
import com.google.gson.JsonElement;
import org.hkijena.misa_imagej.api.MISASerializable;
import org.hkijena.misa_imagej.utils.GsonUtils;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MISASerializableRegistry {
    private Map<String, Class<? extends MISASerializable>> registeredCaches = new HashMap<>();

    public MISASerializableRegistry() {

    }

    /**
     * Registers an editor class for a serialization Id
     * @param serializationId
     * @param cacheClass
     */
    public void register(String serializationId, Class<? extends MISASerializable> cacheClass) {
        registeredCaches.put(serializationId, cacheClass);
    }

    /**
     * Deserializes a JSON element into a serializable
     * @param element
     * @return
     */
    public MISASerializable deserialize(JsonElement element) {
        if(element.isJsonObject()) {
            if(element.getAsJsonObject().has("misa:serialization-id")) {
                String id = element.getAsJsonObject().get("misa:serialization-id").getAsString();
                if(registeredCaches.containsKey(id)) {
                    Gson gson = GsonUtils.getGson();
                    MISASerializable result = gson.fromJson(element, registeredCaches.get(id));
                    result.rawData = element.getAsJsonObject();
                    return result;
                }
                else {
                    Gson gson = GsonUtils.getGson();
                    MISASerializable result = null;
                    // Deserialize to the best matching class
                    List<String> hierarchy = gson.fromJson(element.getAsJsonObject().get("misa:serialization-hierarchy"), List.class);
                    for(String hid : Lists.reverse(hierarchy)) {
                        if(registeredCaches.containsKey(hid)) {
                            result = gson.fromJson(element, registeredCaches.get(hid));
                            break;
                        }
                    }
                    if(result == null) {
                        result = gson.fromJson(element, MISASerializable.class);
                    }
                    result.rawData = element.getAsJsonObject();
                    return result;
                }
            }
            return null;
        }
        else {
            return null;
        }
    }
}

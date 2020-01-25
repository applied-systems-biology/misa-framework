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

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISAFilesystemEntry;
import org.hkijena.misa_imagej.api.MISASample;

import java.util.HashMap;
import java.util.Map;

public class MISACacheRegistry {
    private Map<String, Class<? extends MISACache>> registeredCaches = new HashMap<>();

    public MISACacheRegistry() {

    }

    /**
     * Registers an editor class for a serialization Id
     * @param serializationId
     * @param cacheClass
     */
    public void register(String serializationId, Class<? extends MISACache> cacheClass) {
        registeredCaches.put(serializationId, cacheClass);
    }

    /**
     * Creates a cache for a filesystem entry
     * @param filesystemEntry
     * @return
     */
    public MISACache getCacheFor(MISASample sample, MISAFilesystemEntry filesystemEntry) {

        MISACache tmp = new MISACache(sample, filesystemEntry);
        String patternId = tmp.getPatternSerializationID();
        String descriptionId = tmp.getDescriptionSerializationID();

        Class<? extends MISACache> result = null;

        if(descriptionId != null) {
            // The preferred way: Decide via description id
            result = registeredCaches.getOrDefault(descriptionId, null);
        }
        if(result == null && patternId != null) {
            // Also possible: A (possibly) more generic way via pattern
            result = registeredCaches.getOrDefault(patternId, null);
        }
        if(result == null) {
            result = MISACache.class;
        }

        try {
            return result.getConstructor(MISASample.class, MISAFilesystemEntry.class).newInstance(sample, filesystemEntry);
        } catch (Exception e) {
            e.printStackTrace();
            throw new RuntimeException(e);
        }
    }
}

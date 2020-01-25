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

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.workbench.MISAOutput;
import org.hkijena.misa_imagej.extension.outputcaches.DefaultMISAOutputCacheUI;
import org.hkijena.misa_imagej.ui.workbench.MISAOutputCacheUI;

import java.util.HashMap;
import java.util.Map;

public class MISAOutputCacheUIRegistry {

    private Map<Class<? extends MISACache>, Class<? extends MISAOutputCacheUI>> registeredCacheEditors = new HashMap<>();

    public MISAOutputCacheUIRegistry() {

    }

    /**
     * Registers an editor class for a serialization Id
     * @param cacheClass
     */
    public void register(Class<? extends MISACache> cacheClass, Class<? extends MISAOutputCacheUI> editorClass) {
        registeredCacheEditors.put(cacheClass, editorClass);
    }

    /**
     * Creates an UI editor for the cache
     * @return
     */
    public MISAOutputCacheUI getEditorFor(MISAOutput misaOutput, MISACache cache) {
        Class<? extends MISAOutputCacheUI> result = registeredCacheEditors.getOrDefault(cache.getClass(), null);
        if(result == null)
            return new DefaultMISAOutputCacheUI(misaOutput, cache);
        else {
            try {
                return result.getConstructor(MISAOutput.class, MISACache.class).newInstance(misaOutput, cache);
            } catch (Exception e) {
                e.printStackTrace();
                throw new RuntimeException(e);
            }
        }
    }
}

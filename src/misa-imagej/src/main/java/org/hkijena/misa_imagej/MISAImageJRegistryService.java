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

package org.hkijena.misa_imagej;

import org.hkijena.misa_imagej.api.registries.MISACacheRegistry;
import org.hkijena.misa_imagej.api.registries.MISASerializableRegistry;
import org.hkijena.misa_imagej.ui.registries.*;
import org.scijava.InstantiableException;
import org.scijava.plugin.Plugin;
import org.scijava.plugin.PluginInfo;
import org.scijava.plugin.PluginService;
import org.scijava.service.AbstractService;

@Plugin(type = MISAImageJService.class)
public class MISAImageJRegistryService extends AbstractService implements MISAImageJService {

    private static MISAImageJRegistryService instance;

    public static MISAImageJRegistryService getInstance() {
        return instance;
    }

    public static void instantiate(PluginService pluginService) {
        try {
            instance = (MISAImageJRegistryService) pluginService.getPlugin(MISAImageJRegistryService.class).createInstance();
            instance.discover(pluginService);
        } catch (InstantiableException e) {
            throw new RuntimeException(e);
        }
    }

    private MISADataSourceUIRegistry dataSourceUIRegistry;
    private MISAOutputCacheUIRegistry outputCacheUIRegistry;
    private MISACacheRegistry cacheRegistry;
    private MISASerializableRegistry serializableRegistry;
    private MISAAttachmentFilterUIRegistry attachmentFilterUIRegistry;
    private MISATableAnalyzerUIOperationRegistry tableAnalyzerUIOperationRegistry;
    private MISAPlotBuilderRegistry plotBuilderRegistry;

    public MISAImageJRegistryService() {
        dataSourceUIRegistry = new MISADataSourceUIRegistry();
        outputCacheUIRegistry = new MISAOutputCacheUIRegistry();
        cacheRegistry = new MISACacheRegistry();
        serializableRegistry = new MISASerializableRegistry();
        attachmentFilterUIRegistry = new MISAAttachmentFilterUIRegistry();
        tableAnalyzerUIOperationRegistry = new MISATableAnalyzerUIOperationRegistry();
        plotBuilderRegistry = new MISAPlotBuilderRegistry();
    }

    @Override
    public MISADataSourceUIRegistry getDataSourceUIRegistry() {
        return dataSourceUIRegistry;
    }

    @Override
    public MISAOutputCacheUIRegistry getOutputCacheUIRegistry() {
        return outputCacheUIRegistry;
    }

    @Override
    public MISAAttachmentFilterUIRegistry getAttachmentFilterUIRegistry() {
        return attachmentFilterUIRegistry;
    }

    @Override
    public MISACacheRegistry getCacheRegistry() {
        return cacheRegistry;
    }

    @Override
    public MISASerializableRegistry getSerializableRegistry() {
        return serializableRegistry;
    }

    @Override
    public MISATableAnalyzerUIOperationRegistry getTableAnalyzerUIOperationRegistry() {
        return tableAnalyzerUIOperationRegistry;
    }

    @Override
    public MISAPlotBuilderRegistry getPlotBuilderRegistry() {
        return plotBuilderRegistry;
    }

    private void discover(PluginService pluginService) {
        for(PluginInfo<MISAImageJExtensionService> info : pluginService.getPluginsOfType(MISAImageJExtensionService.class)) {
            try {
                MISAImageJExtensionService service = (MISAImageJExtensionService)info.createInstance();
                service.register(this);
            } catch (InstantiableException e) {
                throw new RuntimeException(e);
            }
        }
    }
}

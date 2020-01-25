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

package org.hkijena.misa_imagej.extension.datasources;

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISADataSource;
import org.hkijena.misa_imagej.api.MISAValidityReport;
import org.hkijena.misa_imagej.api.pipelining.MISAPipelineNode;

import java.nio.file.Path;

/**
 * Data source that is used by the pipelining tool
 */
public class MISAPipelineNodeDataSource implements MISADataSource {

    private MISAPipelineNode sourceNode;
    private MISACache sourceCache;
    private MISACache cache;

    public MISAPipelineNodeDataSource(MISACache cache, MISAPipelineNode sourceNode) {
        this.cache = cache;
        this.sourceNode = sourceNode;
    }


    @Override
    public void install(Path installFolder, boolean forceCopy) {
        // Install operation will be done by pipeline
    }

    @Override
    public String getName() {
        return "Pipeline: " + getSourceNode().getName();
    }

    @Override
    public boolean isEditable() {
        return true;
    }

    @Override
    public MISACache getCache() {
        return cache;
    }

    @Override
    public MISAValidityReport getValidityReport() {
        return new MISAValidityReport();
    }

    public MISAPipelineNode getSourceNode() {
        return sourceNode;
    }

    public MISACache getSourceCache() {
        return sourceCache;
    }

    public void setSourceCache(MISACache sourceCache) {
        this.sourceCache = sourceCache;
    }
}

/**
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

package org.hkijena.segment_glomeruli.tasks;

import org.hkijena.segment_glomeruli.DataInterface;
import org.hkijena.segment_glomeruli.data.TissueQuantificationResult;

public class QuantifyTissue extends DAGTask {

    public QuantifyTissue(Integer tid, DataInterface dataInterface) {
        super(tid, dataInterface);
    }

    @Override
    public void work() {
        System.out.println("Running QuantifyTissue on " + getDataInterface().getInputData().toString());

        TissueQuantificationResult result = getDataInterface().getTissueQuantificationResult();
        result.numPixels = getDataInterface().getTissuePixelCount();
        result.volumeMicrons3 = result.numPixels * getDataInterface().getVoxelSizeXY() * getDataInterface().getVoxelSizeXY() * getDataInterface().getVoxelSizeZ();
    }
}

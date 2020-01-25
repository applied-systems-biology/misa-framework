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

import net.imglib2.Cursor;
import net.imglib2.img.Img;
import net.imglib2.type.numeric.integer.UnsignedIntType;
import org.hkijena.segment_glomeruli.DataInterface;

public class ApplyGlomeruliFiltering extends DAGTask {

    public ApplyGlomeruliFiltering(Integer tid, DataInterface dataInterface) {
        super(tid, dataInterface);
    }

    @Override
    public void work() {
        System.out.println("Running ApplyGlomeruliFiltering on " + getDataInterface().getInputData().toString());

        for(int z = 0; z < getDataInterface().getInputData().getZSize(); ++z) {
            Img<UnsignedIntType> label = getDataInterface().getGlomeruli3DOutputData().getOrCreatePlane(z);
            Cursor<UnsignedIntType> cursor = label.cursor();
            while(cursor.hasNext()) {
                cursor.fwd();
                int l = cursor.get().getInteger();
                if(l > 0 && !getDataInterface().getGlomeruliQuantificationResult().data.get(l).valid) {
                    cursor.get().set(0);
                }
            }
            getDataInterface().getGlomeruli3DOutputData().setPlane(z, label);
        }
    }
}

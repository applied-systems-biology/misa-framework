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
import org.hkijena.segment_glomeruli.data.GlomeruliQuantificationResult;
import org.hkijena.segment_glomeruli.data.Glomerulus;

public class QuantifyGlomeruli extends DAGTask {

    private double glomerulusMinRadius = 15;
    private double glomerulusMaxRadius = 65;

    public QuantifyGlomeruli(Integer tid, DataInterface dataInterface) {
        super(tid, dataInterface);
    }

    @Override
    public void work() {
        System.out.println("Running QuantifyGlomeruli on " + getDataInterface().getInputData().toString());

        GlomeruliQuantificationResult result = getDataInterface().getGlomeruliQuantificationResult();

        for(int z = 0; z < getDataInterface().getInputData().getZSize(); ++z) {
            Img<UnsignedIntType> label = getDataInterface().getGlomeruli3DOutputData().getOrCreatePlane(z);
            Cursor<UnsignedIntType> cursor = label.cursor();
            while(cursor.hasNext()) {
                cursor.fwd();
                int l = cursor.get().getInteger();
                if(l > 0) {
                    result.data.putIfAbsent(l, new Glomerulus());
                    Glomerulus glom = result.data.get(l);
                    glom.label = l;
                    glom.pixels += 1;
                }
            }
        }

        // Calculate the properties of the glomeruli
        double glomerulus_min_volume = 4.0 / 3.0 * Math.PI * Math.pow(glomerulusMinRadius, 3);
        double glomerulus_max_volume = 4.0 / 3.0 * Math.PI * Math.pow(glomerulusMaxRadius, 3);

        double diameter_sum = 0;
        double diameter_sum_sq = 0;

        for(Glomerulus glomerulus : result.data.values()) {
            glomerulus.volume = glomerulus.pixels * getDataInterface().getVoxelSizeXY() * getDataInterface().getVoxelSizeXY() * getDataInterface().getVoxelSizeZ();
            glomerulus.diameter = 2.0 * Math.pow(3.0 / 4.0 * glomerulus.volume / Math.PI, 1.0 / 3.0);
            glomerulus.valid = glomerulus.volume >= glomerulus_min_volume && glomerulus.volume <= glomerulus_max_volume;
            if(glomerulus.valid) {
                ++result.validGlomeruliNumber;
                diameter_sum += glomerulus.diameter;
                diameter_sum_sq += glomerulus.diameter * glomerulus.diameter;
            }
            else {
                ++result.invalidGlomeruliNumber;
            }
        }

        result.validGlomeruliDiameterAverage = diameter_sum / result.validGlomeruliNumber;
        result.validGlomeruliDiameterVariance = (diameter_sum_sq / result.validGlomeruliNumber) - Math.pow(result.validGlomeruliDiameterAverage, 2);

    }
}

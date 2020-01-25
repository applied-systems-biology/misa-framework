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

package org.hkijena.segment_glomeruli.data;

import com.google.gson.annotations.SerializedName;

import java.util.HashMap;
import java.util.Map;

public class GlomeruliQuantificationResult {
    public Map<Integer, Glomerulus> data = new HashMap<>();

    @SerializedName("valid-glomeruli-number")
    public int validGlomeruliNumber = 0;

    @SerializedName("invalid-glomeruli-number")
    public int invalidGlomeruliNumber = 0;

    @SerializedName("valid-glomeruli-diameter-average")
    public double validGlomeruliDiameterAverage = 0;

    @SerializedName("valid-glomeruli-diameter-variance")
    public double validGlomeruliDiameterVariance = 0;
}

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

import com.google.gson.annotations.SerializedName;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MISARuntimeLog extends MISASerializable {

    @SerializedName("entries")
    public Map<String, List<Entry>> entries = new HashMap<>();

    public static class Entry {

        @SerializedName("name")
        public String name;

        @SerializedName("end-time")
        public double endTime;

        @SerializedName("start-time")
        public double startTime;
    }

    /**
     * Returns the total runtime
     * @return
     */
    public double getTotalRuntime() {
        double result = 0;
        for(List<Entry> list : entries.values()) {
            for(Entry entry : list) {
                result = Math.max(entry.endTime, result);
            }
        }
        return result;
    }

    /**
     * Estimates the runtime if no parallelization would be used
     * @return
     */
    public double getUnparallelizedRuntime() {
        if(entries.size() <= 1)
            return getTotalRuntime();
        double result = 0;
        for(List<Entry> list : entries.values()) {
            for(Entry entry : list) {
                result += entry.endTime - entry.startTime;
            }
        }
        return result;
    }

    /**
     * Returns the speedup by parallelization
     * @return
     */
    public double getParallelizationSpeedup() {
        return getUnparallelizedRuntime() / getTotalRuntime();
    }


}

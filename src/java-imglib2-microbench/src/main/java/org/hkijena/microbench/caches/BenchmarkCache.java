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

package org.hkijena.microbench.caches;

import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Path;
import java.util.Map;

public class BenchmarkCache {
    private Path file;

    public BenchmarkCache(Path file) {
        this.file = file;
    }

    public void put(Map<String, Double> times) {
        try(FileWriter writer = new FileWriter(file.toFile())) {
            writer.append(String.join(",", "Benchmark", "Runtime (s)"));
            writer.append('\n');
            for(Map.Entry<String, Double> kv : times.entrySet()) {
                writer.append(String.join(",", kv.getKey(), "" + kv.getValue()));
                writer.append('\n');
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}

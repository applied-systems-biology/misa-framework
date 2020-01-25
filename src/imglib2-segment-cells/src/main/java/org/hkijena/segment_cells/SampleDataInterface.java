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

package org.hkijena.segment_cells;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

public class SampleDataInterface {

    private Path outputDirectory;
    private Map<String, ExperimentDataInterface> experiments = new HashMap<>();
    private Map<String, Integer> quantificationResults = new HashMap<>();

    public SampleDataInterface(Path inputDirectory, Path outputDirectory) {
        this.outputDirectory = outputDirectory;
        try {
            for(Path experiment : Files.list(inputDirectory).collect(Collectors.toList())) {
                if(Files.isDirectory(experiment)) {
                    ExperimentDataInterface dataInterface = new ExperimentDataInterface(experiment, this);
                    experiments.put(experiment.getFileName().toString(), dataInterface);
                }
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public Map<String, ExperimentDataInterface> getExperiments() {
        return experiments;
    }

    public void saveQuantificationResults() {
        try(FileWriter writer = new FileWriter(outputDirectory.resolve("conidia.json").toFile())) {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            String json = gson.toJson(quantificationResults);
            writer.write(json);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public String toString() {
        return outputDirectory.toString();
    }

    public Path getOutputDirectory() {
        return outputDirectory;
    }

    public Map<String, Integer> getQuantificationResults() {
        return quantificationResults;
    }
}

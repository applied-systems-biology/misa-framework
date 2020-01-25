# /**
# * Copyright by Ruman Gerst
# * Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge
# * https://www.leibniz-hki.de/en/applied-systems-biology.html
# * HKI-Center for Systems Biology of Infection
# * Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)
# * Adolf-Reichwein-Straße 23, 07745 Jena, Germany
# *
# * This code is licensed under BSD 2-Clause
# * See the LICENSE file provided with this code for the full license.
# */

import os

import algorithms

print("Input read from " + config["input"])
print("Output written to " + config["output"])

if not config["input"].endswith("/"):
    config["input"] += "/"
if not config["output"].endswith("/"):
    config["output"] += "/"

if not os.path.exists(config["output"]):
    os.makedirs(config["output"])

# Collect sample information
samples = {}

for sample_name in os.listdir(config["input"]):
    sample_path = config["input"] + "/" + sample_name
    output_path = config["output"] + "/" + sample_name
    if not os.path.isdir(sample_path):
        continue

    experiments = sorted([x for x in os.listdir(sample_path) if os.path.isdir(sample_path + "/" + x)])

    samples[sample_name] = {
        "experiments": experiments
    }

    print("Sample " + sample_name + " of size " + str(len(experiments)))

rule all:
    input: expand(config["output"] + "{sample}/results.json", sample=list(samples.keys()))

def aggr_experiments_q(wildcards):
    return [ config["output"] + wildcards["sample"] + "/" + x + ".tif" for x in samples[wildcards["sample"]]["experiments"]]

rule experiments_q:
    input: aggr_experiments_q
    output: config["output"] + "{sample}/results.json"
    run:
        sample=samples[wildcards["sample"]]
        conidia=config["output"] + wildcards["sample"] + "/"
        print("Quantifying ...")
        algorithms.quantify_conidia(label_dir=conidia,
                                    output_file=output[0],
                                    experiments=sample["experiments"])

rule segment_conidia:
    input: config["input"] + "{sample}/{experiment}/channel1.tif"
    output: config["output"] + "{sample}/{experiment}.tif"
    run:
        sample=samples[wildcards["sample"]]
        algorithms.segment_conidia(input_file=input[0], output_file=output[0],)

rule input_files:
    output: config["input"] + "{sample}/{experiment}/channel1.tif"

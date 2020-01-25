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
import glob
import json
import algorithms

print("Input read from " + config["input"])
print("Output written to " + config["output"])

if not config["input"].endswith("/"):
    config["input"] += "/"
if not config["output"].endswith("/"):
    config["output"] += "/"

if not os.path.exists(config["output"]):
    os.makedirs(config["output"])

# Collect voxel sizes
voxel_sizes = None
with open(config["input"] + "/voxel_sizes.json", "r") as f:
    voxel_sizes = json.load(f)

# Collect sample information
samples = {}

for sample_name in os.listdir(config["input"]):
    sample_path = config["input"] + "/" + sample_name
    output_path = config["output"] + "/" + sample_name
    if not os.path.isdir(sample_path):
        continue

    slice_names = sorted([os.path.basename(x) for x in glob.glob(sample_path + "/*.tif")])

    samples[sample_name] = {
        "slice-names": slice_names,
        "voxel-xy": voxel_sizes[sample_name]["xy"],
        "voxel-z": voxel_sizes[sample_name]["z"]
    }

    print("Sample " + sample_name + " of size " + str(len(slice_names)))

rule all:
    input: expand(config["output"] + "{sample}/glomeruli.json", sample=list(samples.keys()))

def aggr_glomeruli_q(wildcards):
    return [ config["output"] + wildcards["sample"] + "/glomeruli2d/" + x for x in samples[wildcards["sample"]]["slice-names"]]

rule glomeruli_q:
    input: aggr_glomeruli_q
    output: config["output"] + "{sample}/glomeruli.json"
    run:
        sample=samples[wildcards["sample"]]
        glomeruli2d=config["output"] + wildcards["sample"] + "/glomeruli2d/"
        glomeruli3d=config["output"] + wildcards["sample"] + "/glomeruli3d/"
        print("Segmenting glomeruli in 3D ...")
        algorithms.segment_glomeruli3d(input_dir=glomeruli2d,
                                            output_dir=glomeruli3d,
                                            slice_names=sample["slice-names"])
        print("Filtering and quantifying glomeruli in 3D ...")
        algorithms.quantify_and_filter_glomeruli3d(label_dir=glomeruli3d,
                                                   output_file=output[0],
                                                   slice_names=sample["slice-names"],
                                                   voxel_xy=sample["voxel-xy"],
                                                   voxel_z=sample["voxel-z"])

rule glomeruli_2d:
    input:
        config["input"] + "{sample}/{slice}",
        config["output"] + "{sample}/tissue/{slice}",
        config["output"] + "{sample}/tissue.json"
    output: config["output"] + "{sample}/glomeruli2d/{slice}"
    run:
        sample=samples[wildcards["sample"]]
        algorithms.segment_glomeruli2d(input_file=input[0],
                                       tissue_mask_file=input[1],
                                       output_file=output[0],
                                       voxel_xy=sample["voxel-xy"])

def aggr_tissue_q(wildcards):
    return [ config["output"] + wildcards["sample"] + "/tissue/" + x for x in samples[wildcards["sample"]]["slice-names"]]

rule tissue_q:
    input: aggr_tissue_q
    output: config["output"] + "{sample}/tissue.json"
    run:
        sample=samples[wildcards["sample"]]
        algorithms.quantify_tissue_2d(input_dir=config["output"] + wildcards["sample"] + "/tissue",
                                      output_file=output[0],
                                      voxel_xy=sample["voxel-xy"],
                                      voxel_z=sample["voxel-z"],
                                      slice_names=sample["slice-names"])

rule tissue_2d:
    input: config["input"] + "{sample}/{slice}"
    output: config["output"] + "{sample}/tissue/{slice}"
    run:
        sample=samples[wildcards["sample"]]
        algorithms.segment_tissue2d(input_file=input[0],
                                    output_file=output[0],
                                    voxel_xy=sample["voxel-xy"])

rule input_files:
    output: config["input"] + "{sample}/{slice}"

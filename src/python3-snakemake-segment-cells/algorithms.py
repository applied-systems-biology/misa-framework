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

import json

import mahotas
import numpy as np
import skimage.external.tifffile as tifffile
import skimage.morphology as morph
from scipy import ndimage as ndi
from skimage import filters
from skimage import (img_as_ubyte, img_as_float, img_as_int)
from skimage import io
from skimage.morphology import watershed


def segment_conidia(input_file, output_file):
    img = io.imread(input_file)
    img = img_as_float(img)
    img = filters.gaussian(img, 1)

    thresholded = img_as_ubyte(img > filters.threshold_otsu(img))
    thresholded = mahotas.close_holes(thresholded)

    distance = ndi.distance_transform_edt(thresholded)
    local_maxi = distance == morph.dilation(distance, morph.square(5))
    local_maxi[thresholded == 0] = 0
    markers = ndi.label(local_maxi)[0]

    labels = watershed(-distance, markers, mask=thresholded)
    tifffile.imsave(output_file, img_as_int(labels), compress=5)
    # tifffile.imsave(output_file + ".vis.tif", img_as_ubyte(color.label2rgb(labels, bg_label=0)), compress=5)


def quantify_conidia(label_dir, output_file, experiments):
    data = {}
    for experiment in experiments:
        img = io.imread(label_dir + "/" + experiment + ".tif")
        data[experiment] = len(np.unique(img)) - 1
    with open(output_file, "w") as f:
        json.dump(data, f, indent=4)

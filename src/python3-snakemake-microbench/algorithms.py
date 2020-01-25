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

import sys
from collections import defaultdict
from datetime import datetime

import numpy as np
import pandas as pd
import skimage.external.tifffile as tifffile
import skimage.morphology as morph
from scipy import ndimage as ndi
from skimage import feature
from skimage import filters
from skimage import img_as_float, img_as_float32, img_as_ubyte


def wiener2(img, neighborship=3, noise_variance=None):
    box_kernel = np.ones((neighborship,neighborship)) / 9
    img_mean = ndi.convolve(img, box_kernel)
    img2_mean = ndi.convolve(img * img, box_kernel)
    img_var = img2_mean - img_mean * img_mean
    img_var += sys.float_info.epsilon
    if noise_variance is None:
        noise_variance = np.mean(img_var)
    result = img_mean + ((img_var - noise_variance) / img_var) * (img - img_mean)
    return result


def microbench(input_file, output_folder):

    times = []
    times.append(datetime.now())

    img = img_as_float(tifffile.imread(input_file))
    times.append(datetime.now())

    # Median filter
    img_median_filtered = filters.median(img, morph.square(21))
    times.append(datetime.now())

    # Morphology benchmark
    img_dilated = morph.dilation(img, morph.disk(15))
    times.append(datetime.now())

    # FFT-IFFT benchmark
    img_fft = np.fft.fft2(img)
    img_ifft = np.fft.ifft2(img_fft).real
    times.append(datetime.now())

    # Otsu benchmark
    img_otsu = img > filters.threshold_otsu(img)
    times.append(datetime.now())

    # Percentile benchmark
    img_percentile = img > np.percentile(img, 65)
    times.append(datetime.now())

    # Canny benchmark
    img_edges = img_as_float32(feature.canny(img_as_ubyte(img), 1, 0.1 * 255, 0.2 * 255))
    times.append(datetime.now())

    # Wiener2 benchmark
    img_wiener2 = wiener2(img)
    times.append(datetime.now())

    # IO
    tifffile.imsave(output_folder + "/median_filtered.tif", img_as_float32(img_median_filtered))
    tifffile.imsave(output_folder + "/dilated.tif", img_as_float32(img_dilated))
    tifffile.imsave(output_folder + "/fft_ifft.tif", img_as_float32(img_ifft))
    tifffile.imsave(output_folder + "/otsu.tif", img_as_float32(img_otsu))
    tifffile.imsave(output_folder + "/percentile.tif", img_as_float32(img_percentile))
    tifffile.imsave(output_folder + "/canny_edges.tif", img_as_float32(img_edges))
    tifffile.imsave(output_folder + "/wiener2.tif", img_as_float32(img_wiener2))
    times.append(datetime.now())

    # Save benchmark results
    runtimes = defaultdict(int)
    time_points = [None, "io", "median", "morphology", "fft-ifft", "otsu", "percentile", "canny", "wiener2", "io"]
    for i in range(1, len(time_points)):
        runtimes[time_points[i]] += (times[i] - times[i - 1]).total_seconds()

    df = pd.DataFrame({ "Runtime (s)": [ runtimes[x] for x in runtimes.keys() ] }, index=runtimes.keys())
    df.to_csv(output_folder + "/runtime.csv")

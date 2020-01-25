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

import numpy as np
import scipy.ndimage as ndi
from skimage import img_as_float, img_as_float32
from skimage.external import tifffile


def clamp(img):
    img[img < 0] = 0
    img[img > 1] = 1
    return img


def convolve(input_data_file, input_psf_file, output_file):
    psf = tifffile.imread(input_psf_file)
    img = tifffile.imread(input_data_file)
    convolved = ndi.convolve(img, psf)
    tifffile.imsave(output_file, img_as_float32(convolved), compress=5)


def fftpad(img, target_size,shift=False):

    # Additional padding for even image dimensions
    ap = [0, 0]
    if img.shape[0] % 2 == 0:
        ap[0] = 1
    if img.shape[1] % 2 == 0:
        ap[1] = 1

    c = (target_size - np.array(img.shape) - ap) // 2
    p = ((c[0], c[0] + ap[0]), (c[1], c[1] + ap[1]))
    pd = np.pad(img, p, "constant")
    if shift:
        pd = np.roll(pd, -(np.array(pd.shape) // 2), (0, 1))
    return pd


def fftunpad(img, source_size):
    ap = [0, 0]
    if source_size[0] % 2 == 0:
        ap[0] = 1
    if source_size[1] % 2 == 0:
        ap[1] = 1
    pad = (np.array(img.shape) - np.array(source_size) - ap) // 2
    return img[pad[0]:source_size[0]+pad[0],pad[1]:source_size[1]+pad[1]]


def get_laplacian(target_size):
    sz = np.array(target_size)
    sz2 = sz // 2 + 1
    psz = sz - sz2
    u = np.tile(np.arange(0, sz2[1], dtype=np.float), sz2[0])
    u = np.reshape(u, sz2)
    v = np.repeat(np.arange(0, sz2[0], dtype=np.float), sz2[1])
    v = np.reshape(v, sz2)

    u = np.pi * u / (sz[1] / 2)
    v = np.pi * v / (sz[0] / 2)

    h = u ** 2 + v ** 2
    h = np.pad(h, ((0, psz[0]), (0, psz[1])), mode="reflect")
    return h + 1j * np.zeros(target_size, dtype=np.float)


def deconvolve(input_data_file, input_psf_file, output_file):

    # Parameter for regularized inverse filter
    rif_lambda = 0.001

    psf = tifffile.imread(input_psf_file)
    convolved = tifffile.imread(input_data_file)

    target_size = np.array(convolved.shape) + np.array(psf.shape) - 1
    H = np.fft.fft2(fftpad(psf, target_size, True))
    Y = np.fft.fft2(fftpad(convolved, target_size))
    # L = np.fft.fft2(fftpad(np.reshape([1, 1, 1, 1, -8, 1, 1, 1, 1], (3, 3)) / 8, target_size, True))
    L = get_laplacian(Y.shape)

    X = Y * H / ((H * H) + (L * rif_lambda * L))
    x = np.fft.ifft2(X).real
    x = fftunpad(x, convolved.shape)

    tifffile.imsave(output_file, img_as_float32(x), compress=5)

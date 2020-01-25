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

#pragma once

#include <opencv2/opencv.hpp>
#include <boost/filesystem/path.hpp>

namespace misaxx::imaging::utils {

    enum class tiff_compression : unsigned short {
        none = 1,
        lzw = 5
    };

    /**
     * Reads a cv::Mat from TIFF. Supports all types supported by OpenCV
     * @param t_path
     * @return
     */
    extern cv::Mat tiffread(const boost::filesystem::path &t_path);

    /**
     * Writes a cv::Mat to TIFF. Supports all types supported by OpenCV
     * @param t_img
     * @param t_path
     */
    extern void tiffwrite(const cv::Mat &t_img, const boost::filesystem::path &t_path, tiff_compression t_compression = tiff_compression::none);
}





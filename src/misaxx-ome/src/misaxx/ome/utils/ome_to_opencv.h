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

#include <ome/files/FormatReader.h>
#include <opencv2/opencv.hpp>
#include <misaxx/ome/descriptions/misa_ome_plane_description.h>

namespace misaxx::ome {
    
    /**
     * Converts a OME variant pixel buffer into a cv::Mat
     * @param ome
     * @return
     */
    extern cv::Mat ome_to_opencv(const ::ome::files::FormatReader &ome_reader, const misa_ome_plane_description &index);

}
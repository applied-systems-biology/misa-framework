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

#include "segmentation2d_base.h"

namespace misaxx_tissue {

    /**
     * Main algorithm used to segment the tissue in the publication by Klingberg et. al.
     */
    struct segmentation2d_klingberg_1 : public segmentation2d_base {

        parameter<int> m_median_filter_size;
        parameter<int> m_downscale_factor;
        parameter<double> m_thresholding_percentile;
        parameter<double> m_thresholding_percentile_factor;
        parameter<int> m_morph_disk_radius;
        parameter<double> m_label_min_percentile;
        parameter<std::string> m_resize_interpolation;

        using segmentation2d_base::segmentation2d_base;

        void create_parameters(misaxx::misa_parameter_builder &t_parameters) override;

        void work() override;
    };
}
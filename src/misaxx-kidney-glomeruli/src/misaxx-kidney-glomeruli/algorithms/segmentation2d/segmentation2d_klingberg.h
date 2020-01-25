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

namespace misaxx_kidney_glomeruli {
    struct segmentation2d_klingberg : public segmentation2d_base {

        using segmentation2d_base::segmentation2d_base;

        parameter<int> m_median_filter_size;
        parameter<double>  m_glomeruli_min_rad;
        parameter<double>  m_glomeruli_max_rad;
        parameter<double> m_threshold_percentile;
        parameter<double> m_threshold_factor;

        void work() override;

        void create_parameters(misaxx::misa_parameter_builder &t_parameters) override;
    };
}
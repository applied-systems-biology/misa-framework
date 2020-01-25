//
// Created by rgerst on 08.04.19.
//

#pragma once

#include "segmentation3d_base.h"

namespace misaxx_kidney_glomeruli {
    struct segmentation3d_klingberg : public segmentation3d_base {

        parameter<double> m_max_glomerulus_radius;

        using segmentation3d_base::segmentation3d_base;

        void work() override;

        void create_parameters(parameter_list &t_parameters) override;
    };
}




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

#include <misaxx-kidney-glomeruli/module_interface.h>
#include <misaxx/core/misa_task.h>

namespace misaxx_kidney_glomeruli {
    struct segmentation3d_base : public misaxx::misa_task {

        misaxx::ome::misa_ome_tiff m_input_segmented2d;
        misaxx::ome::misa_ome_tiff m_output_segmented3d;

        using misaxx::misa_task::misa_task;

    };
}
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

#include <misaxx/core/misa_task.h>
#include <misaxx/core/misa_cached_data.h>
#include <misaxx-tissue/module_interface.h>

namespace misaxx_tissue {
    struct segmentation2d_base : public misaxx::misa_task {

        misaxx::ome::misa_ome_plane m_input_autofluoresence;
        misaxx::ome::misa_ome_plane m_output_segmented2d;

        using misa_task::misa_task;

    };
}
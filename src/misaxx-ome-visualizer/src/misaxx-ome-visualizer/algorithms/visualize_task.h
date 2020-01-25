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
#include <misaxx/ome/accessors/misa_ome_plane.h>

namespace misaxx_ome_visualizer {
    struct visualize_task : public misaxx::misa_task {
        using misaxx::misa_task::misa_task;

        misaxx::ome::misa_ome_plane m_input;
        misaxx::ome::misa_ome_plane m_output;

        void work() override;

        void create_parameters(misaxx::misa_parameter_builder &t_parameters) override;
    };
}





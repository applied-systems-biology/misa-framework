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

#include <misaxx/core/misa_module.h>
#include <misaxx-tissue/module_interface.h>
#include <misaxx/core/misa_parameter.h>

namespace misaxx_tissue {

    struct module : public misaxx::misa_module<module_interface> {

        misaxx::misa_parameter<std::string> m_segmentation2d_algorithm;
        misaxx::misa_parameter<std::string> m_segmentation3d_algorithm;

        using misaxx::misa_module<module_interface>::misa_module;

        void create_blueprints(blueprint_list &t_blueprints, parameter_list &t_parameters) override;

        void build(const blueprint_builder &t_builder) override;
    };
}

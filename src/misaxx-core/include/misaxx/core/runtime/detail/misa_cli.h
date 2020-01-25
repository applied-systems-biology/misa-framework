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

#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/misa_root_module.h>

namespace misaxx {
    template<class Module>
    void misa_cli::set_root_module(const std::string &t_name) {
        
        using root_module_type = misaxx::misa_root_module<Module>;

        auto instantiator = [](const std::shared_ptr<misaxx::misa_work_node> &node) {
            return misaxx::misa_work_node::instance_ptr_type(
                    new root_module_type(node));
        };
        std::shared_ptr<misa_work_node> root_node = misa_work_node::create_instance(t_name,
                                                                                    std::shared_ptr<misa_work_node>(),
                                                                                    instantiator);
        std::shared_ptr<misa_work_node> schema_root_node = misa_work_node::create_instance(t_name,
                                                                                    std::shared_ptr<misa_work_node>(),
                                                                                    instantiator);

        this->set_root_node(std::move(root_node));
        this->set_schema_root_node(std::move(schema_root_node));
    }
}
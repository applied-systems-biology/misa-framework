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

#include <misaxx/core/misa_module_interface.h>
#include <misaxx/core/misa_module.h>
#include <misaxx/core/misa_root_module_base.h>
#include <misaxx/core/runtime/misa_runtime_properties.h>
#include <misaxx/core/runtime/misa_parameter_registry.h>
#include <iostream>

namespace misaxx {



    /**
     * Wrap the root module around this module to interpret subfolders inside the import filesystem
     * as objects. The SubModule is called on each of this objects.
     * @tparam SubModule
     */
    template<class SubModule>
    struct misa_root_module : public misa_root_module_base {

        using misa_root_module_base::misa_root_module_base;

    protected:

        blueprint create_rootmodule_blueprint(const std::string &t_name) override {
            return create_submodule_blueprint<SubModule>(t_name);
        }

        misa_worker &build_rootmodule(const blueprint_builder &t_builder, const std::string &t_name) override {
            return t_builder.build<SubModule>(t_name);
        }
    };



}

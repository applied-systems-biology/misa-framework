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
#include <iostream>

namespace misaxx {

    /**
    * Empty module definition for misa_multiobject_root
    */
    struct misa_multiobject_root_interface : public misa_module_interface {
        void setup() override;
    };

    /**
     * Tags a module as root module used by node paths
     */
    struct misa_root_module_base : public misa_module<misa_multiobject_root_interface> {
        using misa_module<misa_multiobject_root_interface>::misa_module;

        void create_parameters(misa_parameter_builder &t_parameters) override;

        void create_blueprints(blueprint_list &t_blueprints, parameter_list &t_parameters) override;

        void build(const blueprint_builder &t_builder) override;

        const std::vector<std::string> &get_objects() const;

    protected:

        /**
         * Creates the blueprint for the submodules
         * @return
         */
        virtual misa_dispatcher_blueprint_list::blueprint create_rootmodule_blueprint(const std::string &t_name) = 0;

        /**
         * Instantiates the submodule
         * @param t_name
         * @return
         */
        virtual misa_worker &build_rootmodule(const blueprint_builder &t_builder, const std::string &t_name) = 0;

    private:
        std::vector<std::string> m_objects;
    };
}

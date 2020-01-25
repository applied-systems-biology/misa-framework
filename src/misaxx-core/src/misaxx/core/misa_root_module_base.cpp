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

#include <misaxx/core/misa_root_module_base.h>

void misaxx::misa_multiobject_root_interface::setup() {
}

void misaxx::misa_root_module_base::create_parameters(misaxx::misa_parameter_builder &) {

}

void misaxx::misa_root_module_base::create_blueprints(misaxx::misa_dispatcher::blueprint_list &t_blueprints,
                                                      misaxx::misa_dispatcher::parameter_list &) {
    if (misaxx::runtime_properties::is_simulating()) {
        t_blueprints.add(create_rootmodule_blueprint("__OBJECT__"));
        m_objects.emplace_back("__OBJECT__");
    } else {
        std::cout << "[multiobject_root] Dispatching root module for all input objects ..." << "\n";
        const nlohmann::json &object_json = misaxx::parameter_registry::get_parameter_json()["samples"];
        for (nlohmann::json::const_iterator it = object_json.begin(); it != object_json.end(); ++it) {
            const std::string &name = it.key();

            if(name == "__OBJECT__") {
                throw std::runtime_error("The sample name '__OBJECT__' is reserved for internal usage.");
            }

            filesystem::entry e = filesystem.imported->resolve(name);
            if (e->has_external_path()) {
                if (boost::filesystem::is_directory(e->external_path())) {
                    std::cout << "[multiobject_root] Found object " << name << ". External path "
                              << e->external_path().string() << " is valid." << "\n";

                } else {
                    std::cout << "[multiobject_root] Warning: Found object " << name << ", but external path "
                              << e->external_path().string() << " does not exist." << "\n";
                }

                t_blueprints.add(create_rootmodule_blueprint(name));
                m_objects.push_back(name);
            } else {
                std::cout << "[multiobject_root] Found object " << name
                          << ", but it has no external path. Skipping." << "\n";
            }
        }
    }
}

void misaxx::misa_root_module_base::build(const misaxx::misa_dispatcher::blueprint_builder &t_builder) {
    for(const std::string &key : m_objects) {
        build_rootmodule(t_builder, key);
    }
}

const std::vector<std::string> &misaxx::misa_root_module_base::get_objects() const {
    return m_objects;
}

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

#include <misaxx/core/runtime/misa_parameter_registry.h>
#include <misaxx/core/runtime/misa_runtime.h>

using namespace misaxx;

nlohmann::json &misaxx::parameter_registry::get_parameter_json() {
    return misa_runtime::instance().get_parameters();
}

nlohmann::json misaxx::parameter_registry::get_json_raw(const std::vector<std::string> &t_path) {
    return misa_runtime::instance().get_parameter_value(t_path);
}

std::shared_ptr<misa_json_schema_property> misaxx::parameter_registry::get_schema_builder() {
    return misa_runtime::instance().get_schema_builder();
}

std::shared_ptr<misa_json_schema_property>
parameter_registry::register_parameter(const std::vector<std::string> &t_path) {
    std::shared_ptr<misa_json_schema_property> current = get_schema_builder();
    for(const std::string &segment : t_path) {
        current = current->resolve(segment);
    }
    return current;
}

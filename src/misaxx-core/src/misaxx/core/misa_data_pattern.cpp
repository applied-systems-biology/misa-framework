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

#include <misaxx/core/misa_data_pattern.h>

using namespace misaxx;

void misa_data_pattern::build_serialization_id_hierarchy(
        std::vector<misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/base"));
}

void misa_data_pattern::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
}

void misa_data_pattern::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
}

void misa_data_pattern::from_json(const nlohmann::json &) {
}

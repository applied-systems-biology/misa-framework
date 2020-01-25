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

#include <misaxx/core/attachments/misa_unit_numeric.h>

using namespace misaxx;

void misa_unit_numeric::from_json(const nlohmann::json &) {
}

void misa_unit_numeric::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
}

void misa_unit_numeric::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_unit<1>::to_json_schema(t_schema);
}

std::string misa_unit_numeric::get_literal() const {
    return ""; // No literal
}

bool misa_unit_numeric::operator==(const misa_unit_numeric &) const {
    return true;
}

bool misa_unit_numeric::operator!=(const misa_unit_numeric &) const {
    return false;
}

void misa_unit_numeric::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "attachments/quantities/numeric"));
}

std::string misa_unit_numeric::get_documentation_name() const {
    return "Numeric unit";
}

std::string misa_unit_numeric::get_documentation_description() const {
    return "Unit of order 0";
}

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

#include <misaxx/core/patterns/misa_dummy_pattern.h>

std::string misaxx::misa_dummy_pattern::get_documentation_name() const {
    return "No pattern";
}

std::string misaxx::misa_dummy_pattern::get_documentation_description() const {
    return "Unused pattern";
}

void
misaxx::misa_dummy_pattern::build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const {
    misa_data_pattern::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/dummy"));
}

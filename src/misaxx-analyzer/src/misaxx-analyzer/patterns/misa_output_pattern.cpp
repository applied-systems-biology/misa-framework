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

#include <misaxx-analyzer/patterns/misa_output_pattern.h>

#include "misaxx-analyzer/patterns/misa_output_pattern.h"

void misaxx_analyzer::misa_output_pattern::from_json(const nlohmann::json &t_json) {
    misa_folder_pattern::from_json(t_json);
}

void misaxx_analyzer::misa_output_pattern::to_json(nlohmann::json &t_json) const {
    misa_folder_pattern::to_json(t_json);
}

void misaxx_analyzer::misa_output_pattern::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_folder_pattern::to_json_schema(t_schema);
}

void misaxx_analyzer::misa_output_pattern::build_serialization_id_hierarchy(
        std::vector<misaxx::misa_serialization_id> &result) const {
    misa_folder_pattern::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misa-indexer", "patterns/misa-output"));
}

std::string misaxx_analyzer::misa_output_pattern::get_documentation_name() const {
    return "MISA Output pattern";
}

std::string misaxx_analyzer::misa_output_pattern::get_documentation_description() const {
    return "Detects a folder that contains a MISA++ output";
}

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

#include <misaxx-kidney-glomeruli/attachments/glomeruli.h>
#include <misaxx/core/utils/string.h>

using namespace misaxx;
using namespace misaxx_kidney_glomeruli;

void glomeruli::from_json(const nlohmann::json &t_json) {
    misaxx::misa_locatable::from_json(t_json);
    data = t_json["data"].get<std::unordered_map<int, glomerulus>>();
    valid_glomeruli_number = t_json["valid-glomeruli-number"].get<size_t>();
    invalid_glomeruli_number = t_json["invalid-glomeruli-number"].get<size_t>();
    valid_glomeruli_diameter_average = t_json["valid-glomeruli-diameter-average"].get<size_t>();
    valid_glomeruli_diameter_variance = t_json["valid-glomeruli-diameter-variance"].get<size_t>();
}

void glomeruli::to_json(nlohmann::json &t_json) const {
    misaxx::misa_locatable::to_json(t_json);
    for (const auto &kv : data) {
        kv.second.to_json(t_json["data"][misaxx::utils::to_string(kv.first)]);
    }
    t_json["valid-glomeruli-number"] = valid_glomeruli_number;
    t_json["invalid-glomeruli-number"] = invalid_glomeruli_number;
    t_json["valid-glomeruli-diameter-average"] = valid_glomeruli_diameter_average;
    t_json["valid-glomeruli-diameter-variance"] = valid_glomeruli_diameter_variance;
}

void glomeruli::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misaxx::misa_locatable::to_json_schema(t_schema);
    t_schema.resolve("data")->declare_required<std::unordered_map<std::string, glomerulus>>()
            .document_title("Data")
            .document_description("The map of glomerulus label to glomerulus");
    t_schema.resolve("valid-glomeruli-number")->declare_required<size_t>()
            .document_title("Number of valid glomeruli")
            .document_description("The number of valid glomeruli according to the filter criteria");
    t_schema.resolve("invalid-glomeruli-number")->declare_required<size_t>()
            .document_title("Number of invalid glomeruli")
            .document_description("The number of invalid glomeruli according to the filter criteria");
    t_schema.resolve("valid-glomeruli-diameter-average")->declare_required<double>()
            .document_title("Average diameter of valid glomeruli");
    t_schema.resolve("valid-glomeruli-diameter-variance")->declare_required<double>()
            .document_title("Variance of the diameter of valid glomeruli");
}

void glomeruli::build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const {
    misaxx::misa_locatable::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misa-kidney-glomeruli", "attachments/glomeruli"));
}

std::string glomeruli::get_documentation_name() const {
    return "Glomeruli";
}

std::string glomeruli::get_documentation_description() const {
    return "List of all glomeruli detected by the segmentation algorithm";
}

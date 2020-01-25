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

#include "misaxx-microbench/attachments/microbench_runtimes.h"

using namespace misaxx_microbench;

void misaxx_microbench::microbench_runtimes::from_json(const nlohmann::json &t_json) {
    misa_locatable::from_json(t_json);
    data = t_json["data"].get<std::unordered_map<std::string, double>>();
}

void misaxx_microbench::microbench_runtimes::to_json(nlohmann::json &t_json) const {
    misa_locatable::to_json(t_json);
    t_json["data"] = data;
}

void misaxx_microbench::microbench_runtimes::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_locatable::to_json_schema(t_schema);
    t_schema.resolve("data")->declare_required<std::unordered_map<std::string, double>>();
}

void misaxx_microbench::microbench_runtimes::build_serialization_id_hierarchy(
        std::vector<misaxx::misa_serialization_id> &result) const {
    misa_locatable::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misaxx-microbenchmark", "attachments/microbench-runtimes"));
}

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

#include <misaxx/core/attachments/misa_locatable.h>
#include <misaxx-kidney-glomeruli/attachments/glomerulus.h>

namespace misaxx_kidney_glomeruli {

    struct glomeruli : public misaxx::misa_locatable {
        std::unordered_map<int, glomerulus> data;
        size_t valid_glomeruli_number = 0;
        size_t invalid_glomeruli_number = 0;
        double valid_glomeruli_diameter_average = 0;
        double valid_glomeruli_diameter_variance = 0;

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misaxx::misa_json_schema_property &t_schema) const override;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const glomeruli& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, glomeruli& p) {
        p.from_json(j);
    }
}

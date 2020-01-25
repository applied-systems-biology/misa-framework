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

#include <misaxx/core/misa_serializable.h>
#include <misaxx/core/descriptions/misa_file_description.h>
#include <misaxx/core/misa_data_description.h>

namespace misaxx {

    /**
     * General-purpose description for a stack of files
     */
    struct misa_file_stack_description : public misa_data_description {

        using files_type = std::unordered_map<std::string, misa_file_description>;

        files_type files;

        misa_file_stack_description() = default;

        explicit misa_file_stack_description(files_type t_files);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;

    };

    inline void to_json(nlohmann::json& j, const misa_file_stack_description& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_file_stack_description& p) {
        p.from_json(j);
    }
}

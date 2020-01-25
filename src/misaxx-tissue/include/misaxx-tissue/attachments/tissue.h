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
#include <misaxx/ome/attachments/misa_ome_pixel_count.h>

namespace misaxx_tissue {
    struct tissue : public misaxx::misa_serializable {

        using volume_type = misaxx::misa_quantity<double, misaxx::ome::misa_ome_unit_length<3>>;

        misaxx::ome::misa_ome_pixel_count pixels;
        volume_type volume;

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misaxx::misa_json_schema_property &schema) const override;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const override;

    };

    inline void to_json(nlohmann::json& j, const tissue& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, tissue& p) {
        p.from_json(j);
    }
}





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
#include <misaxx/core/attachments/misa_quantity.h>
#include <misaxx/ome/attachments/misa_ome_unit.h>
#include <misaxx/ome/attachments/misa_ome_voxel_size.h>

namespace misaxx::ome {
    /**
     * Higher-order wrapper around a number that is modeling the number of pixels
     * Can interact with misa_ome_voxel_size to calculate a misa_ome_volume
     */
    struct misa_ome_pixel_count : public misaxx::misa_locatable, public boost::equality_comparable<misa_ome_pixel_count> {
        long count = 0;

        misa_ome_pixel_count() = default;

        explicit misa_ome_pixel_count(long t_count);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misaxx::misa_json_schema_property &t_schema) const override;

        misaxx::misa_quantity<double, misa_ome_unit_length<3>> get_volume(const misa_ome_voxel_size &voxel_size) const;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_ome_pixel_count& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_ome_pixel_count& p) {
        p.from_json(j);
    }
}





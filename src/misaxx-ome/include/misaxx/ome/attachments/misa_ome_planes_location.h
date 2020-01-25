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

#include <misaxx/core/attachments/misa_location.h>
#include <misaxx/ome/descriptions/misa_ome_plane_description.h>

namespace misaxx::ome {
    /**
     * Attachment allows finding an object via its plane location
     */
    struct misa_ome_planes_location : public misaxx::misa_location {

        /**
         * The planes within the referenced OME TIFF that contains the
         * referenced object
         */
        std::vector<misa_ome_plane_description> planes;

        using misaxx::misa_location::misa_location;

        explicit misa_ome_planes_location(misaxx::misa_cached_data_base &t_cache, std::vector<misa_ome_plane_description> t_planes);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misaxx::misa_json_schema_property &t_schema) const override;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_ome_planes_location& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_ome_planes_location& p) {
        p.from_json(j);
    }

}




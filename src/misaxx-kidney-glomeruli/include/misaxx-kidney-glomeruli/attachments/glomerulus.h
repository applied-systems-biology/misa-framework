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

#include <misaxx/ome/attachments/misa_ome_voxel.h>
#include <misaxx/ome/attachments/misa_ome_pixel_count.h>
#include <misaxx/ome/attachments/misa_ome_quantity.h>
#include <misaxx/core/attachments/misa_locatable.h>
#include <misaxx/ome/utils/units_length.h>

namespace misaxx_kidney_glomeruli {

    struct glomerulus : public misaxx::misa_locatable {
        /**
         * Number of pixels
         */
        misaxx::ome::misa_ome_pixel_count pixels;
        /**
         * Volume of the glomerulus
         */
        misaxx::ome::misa_ome_volume<double> volume;
        /**
         * Diameter of the glomerulus
         */
        misaxx::ome::misa_ome_length<double> diameter;
        /**
         * Bounding box of the glomerulus
         */
        misaxx::ome::misa_ome_voxel bounds;
        /**
         * Label in the labeling output
         */
        int label = 0;
        /**
         * True if the glomerulus is detected as valid during quantification
         */
        bool valid = false;

        glomerulus();

        void from_json(const nlohmann::json &j) override;

        void to_json(nlohmann::json &j) const override;

        void to_json_schema(misaxx::misa_json_schema_property &t_schema) const override;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json &j, const glomerulus &p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json &j, glomerulus &p) {
        p.from_json(j);
    }
}

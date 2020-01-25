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

#include <misaxx/core/attachments/misa_matrix.h>
#include <ome/xml/meta/OMEXMLMetadata.h>
#include <misaxx/ome/attachments/misa_ome_unit.h>

namespace misaxx::ome {
    /**
     * Higher-order wrapper around a vector of 3 length-unit values, modelling the size of a voxel
     */
    struct misa_ome_voxel_size : public misaxx::misa_serializable, public boost::equality_comparable<misa_ome_voxel_size> {

        using matrix_type = misaxx::misa_vector<double, misaxx::ome::misa_ome_unit_length <1>, 3>;
        using unit_type = misaxx::ome::misa_ome_unit_length<1>;
        using ome_unit_type = typename unit_type::ome_unit_type;

        /**
         * Matrix that contains
         */
        matrix_type values;

        misa_ome_voxel_size() = default;

        /**
         * Initializes this voxel size from a matrix
         * @param v
         */
        explicit misa_ome_voxel_size(matrix_type v);

        explicit misa_ome_voxel_size(double x, double y, double z, unit_type u);

        /**
         * Initializes the voxel size from metadata of given image series
         * @param t_meta
         * @param t_series
         */
        explicit misa_ome_voxel_size(const ::ome::xml::meta::OMEXMLMetadata &t_meta, size_t t_series = 0, std::optional<unit_type> t_unit = std::nullopt);

        /**
         * Returns the element product of the matrix as quantity
         * @return
         */
        misaxx::misa_quantity<double, misa_ome_unit_length<3>> get_volume() const;

        /**
         * Gets the area of the xy plane
         */
        misaxx::misa_quantity<double, misa_ome_unit_length<2>> get_xy_area() const;

        /**
         * Returns the unit of the voxel size
         * @return
         */
        misa_ome_unit_length<1> get_unit() const;

        /**
         * Gets the width
         * @return
         */
        misaxx::misa_quantity<double, misa_ome_unit_length<1>> get_size_x() const;

        /**
         * Gets the height
         * @return
         */
        misaxx::misa_quantity<double, misa_ome_unit_length<1>> get_size_y() const;

        /**
         * Gets the depth
         * @return
         */
        misaxx::misa_quantity<double, misa_ome_unit_length<1>> get_size_z() const;

        /**
         * Gets width = height if they are the same, otherwise throws an error
         * @return
         */
        misaxx::misa_quantity<double, misa_ome_unit_length<1>> get_size_xy() const;

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misaxx::misa_json_schema_property &t_schema) const override;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const override;

    public:

        bool operator==(const misa_ome_voxel_size &rhs) const;
    };

    inline void to_json(nlohmann::json& j, const misa_ome_voxel_size& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_ome_voxel_size& p) {
        p.from_json(j);
    }
}
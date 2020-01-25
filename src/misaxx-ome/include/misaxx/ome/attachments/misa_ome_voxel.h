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
#include <misaxx/ome/attachments/misa_ome_unit.h>
#include <misaxx/ome/attachments/misa_ome_voxel_size.h>
#include <misaxx/core/attachments/misa_quantity_range.h>

namespace misaxx::ome {

    /**
     * Models a cuboid 3D voxel
     * The 'from' point is inclusive, while the 'to' point is exclusive
     */
    struct misa_ome_voxel : public misaxx::misa_serializable {
        using range_type = misaxx::misa_quantity_range<double, misaxx::ome::misa_ome_unit_length <1>>;
        using unit_type = misaxx::ome::misa_ome_unit_length<1>;
        using ome_unit_type = typename unit_type::ome_unit_type;

        range_type x_range;
        range_type y_range;
        range_type z_range;

        /**
         * Initializes an invalid voxel (MAX, MIN) ranges
         */
        misa_ome_voxel();

        /**
         * Initializes an invalid voxel (MAX, MIN) ranges
         * and a unit
         * @param t_unit
         */
        explicit misa_ome_voxel(const unit_type &t_unit);

        /**
         * Initializes a voxel from an input matrix
         * @param t_matrix
         */
        explicit misa_ome_voxel(range_type t_x, range_type t_y, range_type t_z);

        /**
         * Returns the voxel size
         * @return
         */
        explicit operator misa_ome_voxel_size() const;

        /**
         * Converts the unit of the ranges
         * @param t_unit
         */
        misa_ome_voxel cast_unit(const unit_type &t_unit) const;

        /**
         * Returns the voxel size
         * @return
         */
        misa_ome_voxel_size get_size() const;

        /**
         * Returns the size in X direction
         * @return
         */
        misaxx::misa_quantity<double, unit_type> get_size_x() const;

        /**
         * Returns the size in Y direction
         * @return
         */
        misaxx::misa_quantity<double, unit_type> get_size_y() const;

        /**
         * Return the size in Z direction
         * @return
         */
        misaxx::misa_quantity<double, unit_type> get_size_z() const;

        misaxx::misa_quantity<double, unit_type> get_from_x() const;

        misaxx::misa_quantity<double, unit_type> get_to_x() const;

        misaxx::misa_quantity<double, unit_type> get_from_y() const;

        misaxx::misa_quantity<double, unit_type> get_to_y() const;

        misaxx::misa_quantity<double, unit_type> get_from_z() const;

        misaxx::misa_quantity<double, unit_type> get_to_z() const;

        void set_from_x(const misaxx::misa_quantity<double, unit_type> &value);

        void set_to_x(const misaxx::misa_quantity<double, unit_type> &value);

        void set_from_y(const misaxx::misa_quantity<double, unit_type> &value);

        void set_to_y(const misaxx::misa_quantity<double, unit_type> &value);

        void set_from_z(const misaxx::misa_quantity<double, unit_type> &value);

        void set_to_z(const misaxx::misa_quantity<double, unit_type> &value);

        /**
         * Ensures that the point is included in the voxel
         * @param value
         */
        void include_x(const misaxx::misa_quantity<double, unit_type> &value);

        /**
         * Ensures that the point is included in the voxel
         * @param value
         */
        void include_y(const misaxx::misa_quantity<double, unit_type> &value);

        /**
         * Ensures that the point is included in the voxel
         * @param value
         */
        void include_z(const misaxx::misa_quantity<double, unit_type> &value);

        /**
         * Ensures that the point is included in the voxel
         * @param x
         * @param y
         * @param z
         */
        void include(const misaxx::misa_quantity<double, unit_type> &x, const misaxx::misa_quantity<double, unit_type> &y, const misaxx::misa_quantity<double, unit_type> &z);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misaxx::misa_json_schema_property &t_schema) const override;

        /**
         * Returns true if from < to for all coordinates
         * @return
         */
        bool is_valid() const;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_ome_voxel& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_ome_voxel& p) {
        p.from_json(j);
    }
}





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

#include <misaxx/ome/attachments/misa_ome_voxel.h>
#include <cmath>

using namespace misaxx;
using namespace misaxx::ome;

misa_ome_voxel::misa_ome_voxel() : x_range(range_type(INFINITY, -INFINITY)),
                                   y_range(range_type(INFINITY, -INFINITY)),
                                   z_range(range_type(INFINITY, -INFINITY)) {

}

misa_ome_voxel::misa_ome_voxel(const misa_ome_voxel::unit_type &t_unit) : x_range(
        range_type(INFINITY, -INFINITY, t_unit)),
                                                                          y_range(range_type(INFINITY, -INFINITY,
                                                                                             t_unit)),
                                                                          z_range(range_type(INFINITY, -INFINITY,
                                                                                             t_unit)) {

}

misa_ome_voxel::misa_ome_voxel(misa_ome_voxel::range_type t_x,
                               misa_ome_voxel::range_type t_y,
                               misa_ome_voxel::range_type t_z) :
        x_range(std::move(t_x)),
        y_range(std::move(t_y)),
        z_range(std::move(t_z)) {

}

misa_ome_voxel::operator misa_ome_voxel_size() const {
    return get_size();
}

misa_ome_voxel_size misa_ome_voxel::get_size() const {
    auto same_unit = misaxx::misa_quantities_to_same_unit<double, misa_ome_voxel::unit_type >(
            {x_range.get_length(), y_range.get_length(), z_range.get_length()});
    return misa_ome_voxel_size(same_unit[0].get_value(),
                               same_unit[1].get_value(),
                               same_unit[2].get_value(),
                               same_unit[0].get_unit());
}

misa_quantity<double, misa_ome_voxel::unit_type> misa_ome_voxel::get_size_x() const {
    return x_range.get_length();
}

misa_quantity<double, misa_ome_voxel::unit_type> misa_ome_voxel::get_size_y() const {
    return y_range.get_length();
}

misa_quantity<double, misa_ome_voxel::unit_type> misa_ome_voxel::get_size_z() const {
    return z_range.get_length();
}

misa_quantity<double, misa_ome_voxel::unit_type> misa_ome_voxel::get_from_x() const {
    return x_range.get_from();
}

misa_quantity<double, misa_ome_voxel::unit_type> misa_ome_voxel::get_to_x() const {
    return x_range.get_to();
}

misa_quantity<double, misa_ome_voxel::unit_type> misa_ome_voxel::get_from_y() const {
    return y_range.get_from();
}

misa_quantity<double, misa_ome_voxel::unit_type> misa_ome_voxel::get_to_y() const {
    return y_range.get_to();
}

misa_quantity<double, misa_ome_voxel::unit_type> misa_ome_voxel::get_from_z() const {
    return z_range.get_from();
}

misa_quantity<double, misa_ome_voxel::unit_type> misa_ome_voxel::get_to_z() const {
    return z_range.get_to();
}

void misa_ome_voxel::set_from_x(const misa_quantity<double, misa_ome_voxel::unit_type> &value) {
    x_range.set_from(value);
}

void misa_ome_voxel::set_to_x(const misa_quantity<double, misa_ome_voxel::unit_type> &value) {
    x_range.set_to(value);
}

void misa_ome_voxel::set_from_y(const misa_quantity<double, misa_ome_voxel::unit_type> &value) {
    y_range.set_from(value);
}

void misa_ome_voxel::set_to_y(const misa_quantity<double, misa_ome_voxel::unit_type> &value) {
    y_range.set_to(value);
}

void misa_ome_voxel::set_from_z(const misa_quantity<double, misa_ome_voxel::unit_type> &value) {
    z_range.set_from(value);
}

void misa_ome_voxel::set_to_z(const misa_quantity<double, misa_ome_voxel::unit_type> &value) {
    z_range.set_to(value);
}

void misa_ome_voxel::include_x(const misa_quantity<double, misa_ome_voxel::unit_type> &value) {
    x_range.include(value);
}

void misa_ome_voxel::include_y(const misa_quantity<double, misa_ome_voxel::unit_type> &value) {
    y_range.include(value);
}

void misa_ome_voxel::include_z(const misa_quantity<double, misa_ome_voxel::unit_type> &value) {
    z_range.include(value);
}

void misa_ome_voxel::include(const misa_quantity<double, misa_ome_voxel::unit_type> &x,
                             const misa_quantity<double, misa_ome_voxel::unit_type> &y,
                             const misa_quantity<double, misa_ome_voxel::unit_type> &z) {
    include_x(x);
    include_y(y);
    include_z(z);
}

void misa_ome_voxel::from_json(const nlohmann::json &t_json) {
    x_range.from_json(t_json["x"]);
    y_range.from_json(t_json["y"]);
    z_range.from_json(t_json["z"]);
}

void misa_ome_voxel::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
    x_range.to_json(t_json["x"]);
    y_range.to_json(t_json["y"]);
    z_range.to_json(t_json["z"]);
}

void misa_ome_voxel::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
    x_range.to_json_schema(*t_schema.resolve("x"));
    y_range.to_json_schema(*t_schema.resolve("y"));
    z_range.to_json_schema(*t_schema.resolve("z"));
}

void misa_ome_voxel::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misa-ome", "attachments/voxel"));
}

bool misa_ome_voxel::is_valid() const {
    return get_from_x() < get_to_x() && get_from_y() < get_to_y() && get_from_z() < get_to_z();
}

std::string misa_ome_voxel::get_documentation_name() const {
    return "OME Voxel";
}

std::string misa_ome_voxel::get_documentation_description() const {
    return "Voxel of OME length types";
}

misa_ome_voxel misa_ome_voxel::cast_unit(const misa_ome_voxel::unit_type &t_unit) const {
    misa_ome_voxel result;
    result.x_range = x_range.cast_unit(t_unit);
    result.y_range = y_range.cast_unit(t_unit);
    result.z_range = z_range.cast_unit(t_unit);
    return result;
}






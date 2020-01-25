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

#include <misaxx/ome/attachments/misa_ome_planes_location.h>

using namespace misaxx;
using namespace misaxx::ome;

misa_ome_planes_location::misa_ome_planes_location(misa_cached_data_base &t_cache,
                                                 std::vector<misa_ome_plane_description> t_planes) : misa_location(t_cache),
                                                 planes(std::move(t_planes)){

}

void misa_ome_planes_location::from_json(const nlohmann::json &t_json) {
    misa_location::from_json(t_json);
    planes = t_json["ome-planes"].get<std::vector<misa_ome_plane_description>>();
}

void misa_ome_planes_location::to_json(nlohmann::json &t_json) const {
    misa_location::to_json(t_json);
    t_json["ome-planes"] = planes;
}

void misa_ome_planes_location::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_location::to_json_schema(t_schema);
    t_schema.resolve("ome-planes")->declare_required<std::vector<misa_ome_plane_description>>();
}

void misa_ome_planes_location::build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const {
    misa_location::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misa-ome", "attachments/planes-location"));
}

std::string misa_ome_planes_location::get_documentation_name() const {
    return "OME TIFF planes location";
}

std::string misa_ome_planes_location::get_documentation_description() const {
    return "Location within an OME TIFF";
}

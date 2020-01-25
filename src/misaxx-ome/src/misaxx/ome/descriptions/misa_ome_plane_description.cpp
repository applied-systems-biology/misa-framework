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

#include <misaxx/ome/descriptions/misa_ome_plane_description.h>

using namespace misaxx;
using namespace misaxx::ome;

misa_ome_plane_description::misa_ome_plane_description(::ome::files::dimension_size_type t_series) :
        series(t_series), z(0), c(0), t(0) {

}

misa_ome_plane_description::misa_ome_plane_description(::ome::files::dimension_size_type t_series,
                                                       ::ome::files::dimension_size_type t_z,
                                                       ::ome::files::dimension_size_type t_c,
                                                       ::ome::files::dimension_size_type t_t) :
        series(t_series), z(t_z), c(t_c), t(t_t) {

}

void misa_ome_plane_description::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_data_description::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misa_ome", "descriptions/plane"));
}

std::array<::ome::files::dimension_size_type, 3> misa_ome_plane_description::as_zct() const {
    return { z, c, t };
}

::ome::files::dimension_size_type misa_ome_plane_description::index_within(const ::ome::files::FormatReader &reader) const {
    if(reader.getSeries() != series)
        throw std::runtime_error("The reader must be located at the same series as the location description!");
    return reader.getIndex(z, c, t);
}

::ome::files::dimension_size_type
misa_ome_plane_description::index_within(const ::ome::files::out::OMETIFFWriter &writer) const {
    if(writer.getSeries() != series)
        throw std::runtime_error("The reader must be located at the same series as the location description!");
    return writer.getIndex(z, c, t);
}

void misa_ome_plane_description::from_json(const nlohmann::json &t_json) {
    series = t_json["series"];
    z = t_json["z"];
    c = t_json["c"];
    t = t_json["t"];
}

void misa_ome_plane_description::to_json(nlohmann::json &t_json) const {
    t_json["series"] = series;
    t_json["z"] = z;
    t_json["c"] = c;
    t_json["t"] = t;
}

void misa_ome_plane_description::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_data_description::to_json_schema(t_schema);
    t_schema.resolve("series")->declare_required<::ome::files::dimension_size_type>()
            .document_title("Series")
            .document_description("The series that this plane is located in");
    t_schema.resolve("z")->declare_required<::ome::files::dimension_size_type>()
            .document_title("Depth")
            .document_description("The Z location the plane is located in");
    t_schema.resolve("c")->declare_required<::ome::files::dimension_size_type>()
            .document_title("Channel")
            .document_description("The channel that this plane is located in");
    t_schema.resolve("t")->declare_required<::ome::files::dimension_size_type>()
            .document_title("Time")
            .document_description("The time that this plane is located in");
}

bool misa_ome_plane_description::operator==(const misa_ome_plane_description &rhs) const {
    return series == rhs.series &&
           z == rhs.z &&
           c == rhs.c &&
           t == rhs.t;
}

bool misa_ome_plane_description::operator<(const misa_ome_plane_description &rhs) const {
    return series < rhs.series || z < rhs.z || c < rhs.c || t < rhs.t;
}

std::string misa_ome_plane_description::get_documentation_name() const {
    return "OME TIFF Plane";
}

std::string misa_ome_plane_description::get_documentation_description() const {
    return "A plane within an OME TIFF";
}


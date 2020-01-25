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

#include <misaxx/ome/descriptions/misa_ome_tiff_description.h>
#include <misaxx/core/runtime/misa_runtime_properties.h>

using namespace misaxx;
using namespace misaxx::ome;

void misa_ome_tiff_description::from_json(const nlohmann::json &t_json) {
    misa_file_description::from_json(t_json);
    metadata = ::ome::files::createOMEXMLMetadata(t_json["ome-xml-metadata"].get<std::string>());
}

void misa_ome_tiff_description::to_json(nlohmann::json &t_json) const {
    misa_file_description::to_json(t_json);
    if(!misaxx::runtime_properties::is_simulating()) {
        t_json["ome-xml-metadata"] = metadata->dumpXML();
    }
}

void misa_ome_tiff_description::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_file_description::to_json_schema(t_schema);
    t_schema.resolve("ome-xml-metadata")->declare_optional<std::string>()
            .document_title("OME XML Metadata")
            .document_description("OME XML metadata that describes this TIFF");
}

void
misa_ome_tiff_description::build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const {
    misa_file_description::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misa-ome", "descriptions/ome-tiff"));
}

std::string misa_ome_tiff_description::get_documentation_name() const {
    return "OME TIFF";
}

std::string misa_ome_tiff_description::get_documentation_description() const {
    return "A *.tiff/*.tif file that ist compatile with the OME TIFF standard";
}

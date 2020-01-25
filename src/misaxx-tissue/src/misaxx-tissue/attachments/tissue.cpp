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

#include "misaxx-tissue/attachments/tissue.h"

using namespace misaxx;
using namespace misaxx_tissue;

void tissue::from_json(const nlohmann::json &t_json) {
    misa_serializable::from_json(t_json);
    pixels.from_json(t_json["pixels"]);
    volume.from_json(t_json["volume"]);
}

void tissue::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
    pixels.to_json(t_json["pixels"]);
    volume.to_json(t_json["volume"]);
}

void tissue::to_json_schema(misa_json_schema_property &schema) const {
    misa_serializable::to_json_schema(schema);
    schema.resolve("pixels")->declare_required<misaxx::ome::misa_ome_pixel_count>()
            .document_title("Pixels")
            .document_description("The number of detected tissue pixels");
    schema.resolve("volume")->declare_required<volume_type>()
            .document_title("Volume")
            .document_description("The volume of the tissue");
}

std::string tissue::get_documentation_name() const {
    return "Tissue";
}

std::string tissue::get_documentation_description() const {
    return "Quantified information about the tissue";
}

void tissue::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa-tissue", "attachments/tissue"));
}

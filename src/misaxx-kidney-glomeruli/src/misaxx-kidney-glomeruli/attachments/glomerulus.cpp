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

#include <misaxx-kidney-glomeruli/attachments/glomerulus.h>

using namespace misaxx;
using namespace misaxx_kidney_glomeruli;

glomerulus::glomerulus() : volume(misaxx::ome::misa_ome_volume<double> { misaxx::ome::units::micrometer<3>() }),
                           diameter(misaxx::ome::misa_ome_length<double> { misaxx::ome::units::micrometer<1>() }),
                           bounds(misaxx::ome::misa_ome_voxel { misaxx::ome::units::micrometer<1>() }) {
}


void glomerulus::from_json(const nlohmann::json &j) {
    misa_locatable::from_json(j);
    pixels.from_json(j["pixels"]);
    volume.from_json(j["volume"]);
    diameter.from_json(j["diameter"]);
    bounds.from_json(j["bounds"]);
    label = j["label"];
    valid = j["valid"];
}

void glomerulus::to_json(nlohmann::json &j) const {
    misa_locatable::to_json(j);
    pixels.to_json(j["pixels"]);
    volume.to_json(j["volume"]);
    diameter.to_json(j["diameter"]);
    bounds.to_json(j["bounds"]);
    j["label"] = label;
    j["valid"] = valid;
}

void glomerulus::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_locatable::to_json_schema(t_schema);
    t_schema.resolve("pixels")->declare_required<misaxx::ome::misa_ome_pixel_count>()
            .document_title("Pixels")
            .document_description("The number of detected pixels of this glomerulus");
    t_schema.resolve("volume")->declare_required<misaxx::ome::misa_ome_volume<double>>()
            .document_title("Volume")
            .document_description("The volume of this glomerulus");
    t_schema.resolve("diameter")->declare_required<misaxx::ome::misa_ome_length<double>>()
            .document_title("diameter")
            .document_description("The diameter of this glomerulus");
    t_schema.resolve("bounds")->declare_required<misaxx::ome::misa_ome_voxel>()
            .document_title("bounds")
            .document_description("The bounds of this glomerulus (if available)");
    t_schema.resolve("label")->declare_required<int>()
            .document_title("Label")
            .document_description("The label of this glomerulus within the 3D segmented image stack");
    t_schema.resolve("valid")->declare_required<bool>()
            .document_title("Valid")
            .document_description("True if this glomerulus is valid");
}

void glomerulus::build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const {
    misa_locatable::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misa-kidney-glomeruli", "attachments/glomerulus"));
}

std::string glomerulus::get_documentation_name() const {
    return "Glomerulus";
}

std::string glomerulus::get_documentation_description() const {
    return "Glomerulus detected by the segmentation algorithm";
}


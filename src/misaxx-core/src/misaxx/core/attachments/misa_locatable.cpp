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

#include <misaxx/core/attachments/misa_locatable.h>
#include <misaxx/core/misa_json_schema_property.h>

using namespace misaxx;

misa_locatable::misa_locatable(std::shared_ptr<const misa_location> t_location) : m_location(std::move(t_location)) {

}

void misa_locatable::from_json(const nlohmann::json &t_json) {
    m_location_json = t_json["location"];
}

void misa_locatable::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
    if(static_cast<bool>(m_location))
        t_json["location"] = *m_location;
    else
        t_json["location"] = nlohmann::json {};
}

void misa_locatable::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
    t_schema.resolve("location")->declare_required<misa_location>()
            .document_title("Location")
            .document_description("Location of this object");
}

void misa_locatable::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "attachments/locatable"));
}

void misa_locatable::set_location(std::shared_ptr<const misa_location> location) {
    m_location = std::move(location);
    m_location_json = nlohmann::json {};
}

std::string misa_locatable::get_documentation_name() const {
    return "MISA Locatable";
}

std::string misa_locatable::get_documentation_description() const {
    return "Serializable object that contains information about its location";
}



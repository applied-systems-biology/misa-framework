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

#include <misaxx/core/misa_description_storage.h>

using namespace misaxx;

misa_description_storage::misa_description_storage(std::shared_ptr<misa_data_pattern> t_pattern,
                                                   std::shared_ptr<misa_data_description> t_description) : m_pattern(
        std::move(t_pattern)), m_description(std::move(t_description)) {

}

misa_description_storage::misa_description_storage(const misa_description_storage &t_source) : misa_locatable() {
    nlohmann::json json;
    t_source.to_json(json);
    from_json(json);
}

std::shared_ptr<misa_description_storage> misa_description_storage::clone() const {
    return std::make_shared<misa_description_storage>(*this);
}

void misa_description_storage::from_imported_json(const nlohmann::json &t_json) {
    if (t_json.find("pattern") != t_json.end() || t_json.find("description") != t_json.end()) {
        from_json(t_json);
    } else {
        m_raw_pattern_json = t_json;
    }
}

void misa_description_storage::from_json(const nlohmann::json &t_json) {
    {
        auto it = t_json.find("pattern");
        if (it != t_json.end()) {
            m_raw_pattern_json = it.value();
        }
    }
    {
        auto it = t_json.find("description");
        if (it != t_json.end()) {
            m_raw_description_json = it.value();
        }
    }
    {
        auto it = t_json.find("misa:documentation-title");
        if (it != t_json.end()) {
            documentation_title = it.value();
        }
    }
    {
        auto it = t_json.find("misa:documentation-description");
        if (it != t_json.end()) {
            documentation_description = it.value();
        }
    }
}

void misa_description_storage::to_json(nlohmann::json &t_json) const {
    misa_locatable::to_json(t_json);
    t_json["pattern"] = m_raw_pattern_json; // Pass along the raw metadata. This is very important!
    t_json["description"] = m_raw_description_json;
    if(!documentation_title.empty())
        t_json["misa:documentation-title"] = documentation_title;
    if(!documentation_description.empty())
        t_json["misa:documentation-description"] = documentation_description;

    if (static_cast<bool>(m_pattern)) {
        auto &j = t_json["pattern"];
        m_pattern->to_json(j);
    }
    if (static_cast<bool>(m_description)) {
        auto &j = t_json["description"];
        m_description->to_json(j);
    }
}

void misa_description_storage::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_locatable::to_json_schema(t_schema);
    if (has_pattern()) {
//        t_schema["pattern"]["pattern-type"].define(get<misa_data_pattern>().get_serialization_id());
        get<misa_data_pattern>().to_json_schema(t_schema["pattern"]);
    }
    if (has_description()) {
//        t_schema["description"]["description-type"].define(get<misa_data_description>().get_serialization_id());
        get<misa_data_description>().to_json_schema(t_schema["description"]);
    }
    if(!documentation_title.empty())
        t_schema.document_title(documentation_title);
    if(!documentation_description.empty())
        t_schema.document_description(documentation_description);
}

void misa_description_storage::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_locatable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "description-storage"));
}

bool misa_description_storage::has_pattern() const {
    return static_cast<bool>(m_pattern) ||
           misa_serializable::type_is_deserializable_from_json<misa_data_pattern>(m_raw_pattern_json);
}

bool misa_description_storage::has_description() const {
    return static_cast<bool>(m_description) ||
           misa_serializable::type_is_deserializable_from_json<misa_data_description>(m_raw_description_json);
}

misa_description_storage &misa_description_storage::operator=(const misa_description_storage &t_source) {
    nlohmann::json json;
    t_source.to_json(json);
    from_json(json);
    return *this;
}

std::string misa_description_storage::get_documentation_name() const {
    return "Data description";
}

std::string misa_description_storage::get_documentation_description() const {
    return "Description of a data cache, including an optional pattern used to detect input files";
}


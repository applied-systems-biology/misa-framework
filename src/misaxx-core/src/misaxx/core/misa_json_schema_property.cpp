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

#include <misaxx/core/misa_json_schema_property.h>

using namespace misaxx;

std::shared_ptr<misaxx::misa_json_schema_property> misaxx::misa_json_schema_property::resolve(const std::string &key) {
    if(property_type != property_type::subtree)
        throw std::runtime_error("Cannot access schema subproperty of leaf property!");
    if(value_type != nlohmann::json::value_t::object)
        throw std::runtime_error("Cannot access schema subproperty of non-object property!");
    auto it = properties.find(key);
    if(it != properties.end())
        return it->second;
    else {
        auto prop = std::make_shared<misa_json_schema_property>();
        properties[key] = prop;
        return prop;
    }
}

std::shared_ptr<misa_json_schema_property> misa_json_schema_property::self() {
    return shared_from_this();
}

std::shared_ptr<const misa_json_schema_property> misa_json_schema_property::self() const {
    return shared_from_this();
}

void misa_json_schema_property::to_json(nlohmann::json &json) const {
    if(!title.empty())
        json["misa:documentation-title"] = title;
    if(!description.empty())
        json["misa:documentation-description"] = description;
    if(!type_title.empty())
        json["misa:documentation-type-title"] = type_title;
    if(!type_description.empty())
        json["misa:documentation-type-description"] = type_description;
    if(serialization_id)
        json["misa:serialization-id"] = serialization_id.value();
    if(serialization_hierarchy)
        json["misa:serialization-hierarchy"] = serialization_hierarchy.value();
    switch(value_type) {
        case nlohmann::json::value_t::string:
            json["type"] = "string";
            break;
        case nlohmann::json::value_t::object:
        case nlohmann::json::value_t::null:
            json["type"] = "object";
            break;
        case nlohmann::json::value_t::array:
            json["type"] = "array";
            break;
        case nlohmann::json::value_t::boolean:
            json["type"] = "boolean";
            break;
        case nlohmann::json::value_t::number_float:
        case nlohmann::json::value_t::number_integer:
        case nlohmann::json::value_t::number_unsigned:
            json["type"] = "number";
            break;
        case nlohmann::json::value_t::discarded:
        default:
            throw std::runtime_error("Unsupported JSON type");
    }

    if(property_type == property_type::leaf) {
        if(default_value)
            json["default"] = default_value.value();
        if(!allowed_values.empty())
            json["enum"] = allowed_values;
        if(children_template) {
            switch(value_type) {
                case nlohmann::json::value_t::object:
                    children_template->to_json(json["additionalProperties"]);
                    break;
                case nlohmann::json::value_t::array:
                    children_template->to_json(json["items"]);
                    break;
                case nlohmann::json::value_t::null:
                case nlohmann::json::value_t::boolean:
                case nlohmann::json::value_t::string:
                case nlohmann::json::value_t::number_float:
                case nlohmann::json::value_t::number_integer:
                case nlohmann::json::value_t::number_unsigned:
                case nlohmann::json::value_t::discarded:
                default:
                    throw std::runtime_error("Unsupported JSON type for template");
            }
        }
    } else {
        std::vector<std::string> required_children;
        for(const auto &kv : properties) {
            if(kv.second->required)
                required_children.push_back(kv.first);
            kv.second->to_json(json["properties"][kv.first]);
        }
        if(!required_children.empty())
            json["required"] = required_children;
    }
}

std::shared_ptr<misa_json_schema_property> misa_json_schema_property::resolve(const std::vector<std::string> &keys) {
    std::shared_ptr<misa_json_schema_property> result = self();
    for(const std::string &key : keys) {
        result = result->resolve(key);
    }
    return result;
}

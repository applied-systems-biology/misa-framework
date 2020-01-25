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

#include <misaxx/core/misa_module_info.h>
#include <misaxx/core/misa_json_schema_property.h>

using namespace misaxx;

std::string misa_module_info::get_id() const {
    return m_id;
}

std::string misa_module_info::get_version() const {
    return m_version;
}

std::string misa_module_info::get_name() const {
    if(m_name.empty())
        return get_id();
    else
        return m_name;
}

std::vector<misa_module_info> misa_module_info::get_dependencies() const {
    return m_dependencies;
}

void misa_module_info::from_json(const nlohmann::json &t_json) {
    m_id = t_json["id"];
    m_version = t_json["version"];
    if(t_json.find("name") != t_json.end())
        m_name = t_json["name"];
    if(t_json.find("description") != t_json.end())
        m_description = t_json["description"];
    if(t_json.find("dependencies") != t_json.end())
        m_dependencies = t_json["dependencies"].get<std::vector<misa_module_info>>();

    if(t_json.find("authors") != t_json.end())
        m_authors = t_json["authors"].get<std::vector<std::string>>();
    if(t_json.find("citation") != t_json.end())
        m_citation = t_json["citation"];
    if(t_json.find("url") != t_json.end())
        m_url = t_json["url"];
    if(t_json.find("license") != t_json.end())
        m_license = t_json["license"];
    if(t_json.find("organization") != t_json.end())
        m_organization = t_json["organization"];
    if(t_json.find("is-external") != t_json.end())
        m_is_external = t_json["is-external"];
}

void misa_module_info::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
    t_json["id"] = m_id;
    t_json["version"] = m_version;
    t_json["name"] = get_name();
    t_json["description"] = m_description;
    t_json["dependencies"] = m_dependencies;
    t_json["authors"] = m_authors;
    t_json["citation"] = m_citation;
    t_json["url"] = m_url;
    t_json["license"] = m_license;
    t_json["organization"] = m_organization;
    t_json["is-external"] = m_is_external;
}

void misa_module_info::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
    t_schema.resolve("id")->declare_required<std::string>();
    t_schema.resolve("version")->declare_required<std::string>();
    t_schema.resolve("name")->declare_optional<std::string>();
    t_schema.resolve("description")->declare_optional<std::string>();
    t_schema.resolve("dependencies")->declare_optional<std::vector<misa_module_info>>();
    t_schema.resolve("authors")->declare_optional<std::vector<std::string>>();
    t_schema.resolve("citation")->declare_optional<std::string>();
    t_schema.resolve("url")->declare_optional<std::string>();
    t_schema.resolve("license")->declare_optional<std::string>();
    t_schema.resolve("organization")->declare_optional<std::string>();
    t_schema.resolve("is-external")->declare_optional<bool>()
            .document_description("If true, this info does not describe a MISA++ module");
}

void misa_module_info::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "module-info"));
}

std::string misa_module_info::get_description() const {
    return m_description;
}

std::string misa_module_info::get_url() const {
    return m_url;
}

std::vector<std::string> misa_module_info::get_authors() const {
    return m_authors;
}

std::string misa_module_info::get_organization() const {
    return m_organization;
}

std::string misa_module_info::get_license() const {
    return m_license;
}

std::string misa_module_info::get_citation() const {
    return m_citation;
}

bool misa_module_info::is_external() const {
    return m_is_external;
}

void misa_module_info::set_id(std::string t_id) {
    m_id = std::move(t_id);
}

void misa_module_info::set_name(std::string t_description) {
    m_name = std::move(t_description);
}

void misa_module_info::set_version(std::string t_version) {
    m_version = std::move(t_version);
}

void misa_module_info::add_dependency(misa_module_info t_dependency) {
    m_dependencies.emplace_back(std::move(t_dependency));
}

void misa_module_info::set_description(std::string t_description) {
    m_description = std::move(t_description);
}

void misa_module_info::set_url(std::string t_url) {
    m_url = std::move(t_url);
}

void misa_module_info::add_author(std::string t_author) {
    m_authors.emplace_back(std::move(t_author));
}

void misa_module_info::set_authors(std::vector<std::string> t_authors) {
    m_authors = std::move(t_authors);
}

void misa_module_info::set_organization(std::string t_organization) {
    m_organization = std::move(t_organization);
}

void misa_module_info::set_license(std::string t_license) {
    m_license = std::move(t_license);
}

void misa_module_info::set_citation(std::string t_citation) {
    m_citation = std::move(t_citation);
}

void misa_module_info::set_is_external(bool t_flag) {
    m_is_external = t_flag;
}


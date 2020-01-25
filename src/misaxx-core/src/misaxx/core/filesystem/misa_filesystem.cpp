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

#include <misaxx/core/filesystem/misa_filesystem.h>

using namespace misaxx;

bool misa_filesystem::is_valid() const {
    return imported && exported;
}

misa_filesystem misa_filesystem::create_subsystem(const std::string &t_name) {
    if(!is_valid())
        throw std::runtime_error("Cannot create sub-filesystem from invalid filesystem!");
    misa_filesystem result;
    result.imported = imported->resolve(t_name);
    result.exported = exported->resolve(t_name);
    return result;
}

void misa_filesystem::from_json(const nlohmann::json &) {
    throw std::runtime_error("Not implemented");
}

void misa_filesystem::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
    throw std::runtime_error("Not implemented");
}

void misa_filesystem::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
    imported->to_json_schema(t_schema["imported"]);
    exported->to_json_schema(t_schema["exported"]);
}

void misa_filesystem::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "filesystem"));
}

std::shared_ptr<misa_filesystem_entry>
misa_filesystem::find_external_path(const boost::filesystem::path &t_path) const {

//    std::cout << "SEARCHING_FOR "<< t_path << "\n";

    auto in_imported = imported->find_external_path(t_path);
    auto in_exported = exported->find_external_path(t_path);

    if(!static_cast<bool>(in_imported) && !static_cast<bool>(in_exported)) {
        return std::shared_ptr<misa_filesystem_entry>();
    }
    else if(static_cast<bool>(in_imported) && !static_cast<bool>(in_exported)) {
        return in_imported;
    }
    else if(!static_cast<bool>(in_imported) && static_cast<bool>(in_exported)) {
        return in_exported;
    }
    else if(in_imported->get_depth() > in_exported->get_depth()) {
        return in_imported;
    }
    else {
        return in_exported;
    }
}

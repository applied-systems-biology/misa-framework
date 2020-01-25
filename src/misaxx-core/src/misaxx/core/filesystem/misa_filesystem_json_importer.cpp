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

#include <misaxx/core/filesystem/misa_filesystem_json_importer.h>

using namespace misaxx;

void misa_filesystem_json_importer::import_entry(const nlohmann::json &t_json, const filesystem::entry &t_entry) {

    if(t_json.find("external-path") != t_json.end()) {
        t_entry->custom_external = t_json["external-path"].get<std::string>();
        std::cout << "[Filesystem][json-importer] Importing entry " << t_entry->custom_external.string() << " into " << t_entry->internal_path().string() << "\n";
    }
    else {
        std::cout << "[Filesystem][json-importer] Importing entry " << t_entry->internal_path().string() << "\n";
    }

    // Load the metadata from JSON or file if applicable
    // File metadata is preferred
    if(t_entry->has_external_path() && boost::filesystem::is_regular_file(t_entry->external_path() / "misa-data.json")) {
        std::cout << "[Filesystem][json-importer] Importing metadata from file " << (t_entry->external_path() / "misa-data.json").string() << "\n";
        nlohmann::json json;
        std::ifstream stream;
        stream.open((t_entry->external_path() / "misa-data.json").string());
        stream >> json;
        t_entry->metadata->from_json(json);
    }
    else if(t_json.find("data-metadata") != t_json.end()) {
        std::cout << "[Filesystem][json-importer] Importing metadata from JSON" << "\n";
        t_entry->metadata->from_json(t_json["data-metadata"]);
    }

    if(t_json.find("children") != t_json.end()) {
        const nlohmann::json &children = t_json["children"];
        for(nlohmann::json::const_iterator kv = children.begin(); kv != children.end(); ++kv) {
            const nlohmann::json &json_entry = kv.value();
            filesystem::entry f = t_entry->create(kv.key());
            import_entry(json_entry, f);
        }
    }
}

misa_filesystem misa_filesystem_json_importer::import() {
    misa_filesystem vfs;
    vfs.imported = std::make_shared<misa_filesystem_entry>("imported", misa_filesystem_entry_type::imported);
    vfs.exported = std::make_shared<misa_filesystem_entry>("exported", misa_filesystem_entry_type::exported);

    nlohmann::json json;

    if(input_json.empty()) {
        std::ifstream stream;
        stream.open(json_path.string());
        stream >> json;
    }
    else {
        json = input_json;
    }

    if(json.find("imported") != json.end()) {
        import_entry(json["imported"], vfs.imported);
    }
    if(json.find("exported") != json.end()) {
        import_entry(json["exported"], vfs.exported);
    }

    return vfs;
}

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

#include <misaxx-analyzer/module_interface.h>
#include <sstream>
#include "attachment_indexer_task.h"
#include <misaxx/core/utils/filesystem.h>

using namespace misaxx_analyzer;
using namespace misaxx;


attachment_indexer_discover_result attachment_indexer_task::discover(nlohmann::json &json,
        const std::vector<std::string> &path,
        misaxx::readwrite_access<attachment_index_database> &db,
         const std::string &sample,
         const std::string &cache) {
    if(json.is_object()) {

        std::unordered_map<std::string, attachment_indexer_discover_result> discovered_properties;

        for(auto it = json.begin(); it != json.end(); ++it) {
            auto p = path;
            p.emplace_back(it.key());
            attachment_indexer_discover_result id = discover(it.value(), p, db, sample, cache);
            if(id.database_id > 0) {
                discovered_properties[it.key()] = id;
            }
        }

        // Erase away discovered propertes
        for(const auto &kv : discovered_properties) {
            json[kv.first] = nlohmann::json {
                    { "misa-analyzer:database-index", kv.second.database_id }
            };
//            if(!kv.second.title.empty())
//                json[kv.first]["misa:documentation-title"] = kv.second.title;
//            if(!kv.second.description.empty())
//                json[kv.first]["misa:documentation-description"] = kv.second.description;
        }

        auto serialization_id = json.find("misa:serialization-id");
        if(serialization_id != json.end()) {
            // Add to database
            attachment_index_row row {};
            row.id = -1;
            row.sample = sample;
            row.cache = cache;
            row.serialization_id = serialization_id->get<std::string>();

            for(size_t i = 0; i < path.size(); ++i) {
                if(i > 0)
                    row.property += "/";
                std::string segment = path.at(i);
                boost::replace_all(segment, "\"", "\\\"");
                if(boost::contains(segment, "/")) {
                    row.property += "\"";
                    row.property += segment;
                    row.property += "\"";
                }
                else {
                    row.property += segment;
                }
            }

            // Create a copy of the JSON data without already referenced properties
            std::stringstream db_json_stream;
            db_json_stream << json;
            row.json_data = db_json_stream.str();

            attachment_indexer_discover_result result;
            result.database_id = db.get().insert(row);

//            if(json.find("misa:documentation-title") != json.end())
//                result.title = json.at("misa:documentation-title").get<std::string>();
//            if(json.find("misa:documentation-description") != json.end())
//                result.description = json.at("misa:documentation-description").get<std::string>();

            return result;
        }

        return attachment_indexer_discover_result {};
    }
    else if(json.is_array()) {
        for(size_t i = 0; i < json.size(); ++i) {
            auto p = path;
            p.emplace_back("[" + std::to_string(i) + "]");
            discover(json[i], p, db, sample, cache);
        }
        return attachment_indexer_discover_result {};
    }
    else {
        return attachment_indexer_discover_result {};
    }
}

void attachment_indexer_task::work() {
    // Open access only once
    auto db_access = get_module_as<module_interface>()->data.get_attachment_index().access_readwrite();

    const auto attachments_vector =  get_module_as<module_interface>()->data.get_attachments();
    for(size_t i = 0; i < attachments_vector.size(); ++i) {

        const misa_json &attachments =  attachments_vector.at(i);
        // Send progress manually, so MISA++ for ImageJ can pick it up
        std::cout << "<#> <" << i << " / " << attachments_vector.size() << ">" << " Indexing attachment " << attachments.get_unique_location() << "\n";

        // Extract the sample and cache from the attachment
        boost::filesystem::path attachment_root_path =
                get_module_as<module_interface>()->data.get_location() / "attachments";
        boost::filesystem::path attachment_relative_path = boost::filesystem::relative(
                misaxx::utils::make_preferred(attachments.get_unique_location()), misaxx::utils::make_preferred(attachment_root_path));
        std::vector<std::string> segments;
        for (const boost::filesystem::path &segment: attachment_relative_path) {
            segments.emplace_back(segment.filename().string());
        }
        std::string sample = segments[1];
        std::string cache = segments[0];
        for (size_t j = 2; j < segments.size(); ++j) {
            cache += "/" + segments[j];
        }
        auto json_access = attachments.access_readonly();

        nlohmann::json json = json_access.get();
        discover(json, {}, db_access, sample, cache);
    }
}

void attachment_indexer_task::create_parameters(misaxx::misa_parameter_builder &t_parameters) {

}



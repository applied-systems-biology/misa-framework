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

#include "misaxx-analyzer/caches/misa_output_cache.h"

void misaxx_analyzer::misa_output_cache::postprocess() {
    misa_cache::postprocess();
}

void misaxx_analyzer::misa_output_cache::do_link(const misaxx_analyzer::misa_output_description &t_description) {
    set(t_description.folder);
    set_unique_location(t_description.folder);

    // Link attachment schemas
    m_attachment_schemas.force_link(get_internal_location(),
                                    get_location() / "attachments",
                                    misaxx::misa_description_storage::with(misaxx::misa_json_description(
                                            get_location() / "attachments" / "serialization-schemas.json")));
    if (!m_attachment_schemas.exists())
        throw std::runtime_error("Serialization schemata for attachments do not exist!");

    // Link full attachment schemas
    m_full_attachment_schemas.force_link(get_internal_location(),
                                         get_location() / "attachments",
                                         misaxx::misa_description_storage::with(misaxx::misa_json_description(
                                                 get_location() / "attachments" / "serialization-schemas-full.json")));

    // Link attachments
    if(boost::filesystem::exists(get_location() / "attachments" / "exported")) {
        for(const boost::filesystem::path& entry : boost::make_iterator_range(
                boost::filesystem::recursive_directory_iterator(get_location() / "attachments" / "exported"))) {
            if(entry.extension() == ".json") {
                misaxx::misa_json cache;
                cache.force_link(get_internal_location(), entry.parent_path(), std::make_shared<misaxx::misa_json_description>(entry));
                m_attachments.emplace_back(std::move(cache));
            }
        }
    }

    if(boost::filesystem::exists(get_location() / "attachments" / "imported")) {
        for(const boost::filesystem::path& entry : boost::make_iterator_range(
                boost::filesystem::recursive_directory_iterator(get_location() / "attachments" / "imported"))) {
            if(entry.extension() == ".json") {
                misaxx::misa_json cache;
                cache.force_link(get_internal_location(), entry.parent_path(), std::make_shared<misaxx::misa_json_description>(entry));
                m_attachments.emplace_back(std::move(cache));
            }
        }
    }

    // Link attachment index database
    m_attachment_index.force_link(get_internal_location(), get_location(), std::make_shared<attachment_index_description>("attachment-index.sqlite"));
}

misaxx_analyzer::misa_output_description
misaxx_analyzer::misa_output_cache::produce_description(const boost::filesystem::path &t_location,
                                                       const misaxx_analyzer::misa_output_pattern &t_pattern) {
    misa_output_description result;
    t_pattern.apply(result, t_location);
    return result;
}

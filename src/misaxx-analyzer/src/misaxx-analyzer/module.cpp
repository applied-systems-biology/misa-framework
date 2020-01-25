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

#include <misaxx-analyzer/module.h>
#include "schema_traversion_task.h"
#include "attachment_indexer_task.h"

using namespace misaxx_analyzer;

void module::create_blueprints(blueprint_list &t_blueprints, parameter_list &t_parameters) {
    m_enable_schema_traversion = t_parameters.create_runtime_parameter<bool>("enable-schema-traversion", true);
    m_enable_schema_traversion.schema->document_title("Enable schema traversion")
    .document_description("If enabled, create a full schema of attached data");

    m_enable_attachment_indexing = t_parameters.create_runtime_parameter<bool>("enable-attachment-indexing", true);
    m_enable_attachment_indexing.schema->document_title("Enable attachment indexing")
    .document_description("If enabled, index the location of attached objects");

    t_blueprints.add(create_blueprint<schema_traversion_task>("schema-traversion"));
    t_blueprints.add(create_blueprint<attachment_indexer_task>("attachment-indexer"));
}

void module::build(const misaxx::misa_dispatcher::blueprint_builder &t_builder) {

    const bool unskippable = !misaxx::runtime_properties::requested_skipping();

    if(m_enable_attachment_indexing.query()) {
        if(!boost::filesystem::exists(data.get_location() / "attachments" / "serialization-schemas-full.json") || unskippable)
            t_builder.build<schema_traversion_task>("schema-traversion");
        else
            std::cout << "Skipping serialization schema traversion ..." << "\n";
    }
    if(m_enable_schema_traversion.query()) {
        if(!boost::filesystem::exists(data.get_location() / "attachment-index.sqlite") || unskippable) {
            t_builder.build<attachment_indexer_task>("attachment-indexer");
        }
        else {
            std::cout << "Skipping attachment indexing ..." << "\n";
        }
    }
}


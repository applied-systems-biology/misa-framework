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

#include "misaxx/core/descriptions/misa_exported_attachments_description.h"

void misaxx::misa_exported_attachments_description::build_serialization_id_hierarchy(
        std::vector<misaxx::misa_serialization_id> &result) const {
    misa_file_description::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misa", "descriptions/exported-attachments"));
}

std::string misaxx::misa_exported_attachments_description::get_documentation_name() const {
    return "Exported attachments";
}

std::string misaxx::misa_exported_attachments_description::get_documentation_description() const {
    return "Contains a copy of all data attached to a cache";
}

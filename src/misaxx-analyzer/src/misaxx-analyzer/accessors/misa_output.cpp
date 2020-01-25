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

#include <misaxx-analyzer/accessors/misa_output.h>

#include "misaxx-analyzer/accessors/misa_output.h"

const misaxx::misa_json misaxx_analyzer::misa_output::get_attachment_schemas() const {
    return data->m_attachment_schemas;
}

misaxx::misa_json misaxx_analyzer::misa_output::get_full_attachment_schemas() {
    return data->m_full_attachment_schemas;
}

std::vector<misaxx::misa_json> misaxx_analyzer::misa_output::get_attachments() const {
   return data->m_attachments;
}

misaxx_analyzer::attachment_index misaxx_analyzer::misa_output::get_attachment_index() {
    return data->m_attachment_index;
}

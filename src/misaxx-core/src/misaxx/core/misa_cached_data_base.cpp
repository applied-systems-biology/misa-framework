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

#include <misaxx/core/misa_cached_data_base.h>

void misaxx::misa_cached_data_base::suggest_document_title(std::string title) {
    if(describe()->documentation_title.empty())
        describe()->documentation_title = std::move(title);
}

void misaxx::misa_cached_data_base::suggest_document_description(std::string description) {
    if(describe()->documentation_description.empty())
        describe()->documentation_description = std::move(description);
}

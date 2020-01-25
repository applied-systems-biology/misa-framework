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

#include <misaxx/core/patterns/misa_json_pattern.h>

#include "misaxx/core/patterns/misa_json_pattern.h"

std::string misaxx::misa_json_pattern::get_documentation_name() const {
    return "JSON file pattern";
}

std::string misaxx::misa_json_pattern::get_documentation_description() const {
    return "Finds a JSON file";
}

misaxx::misa_json_pattern::misa_json_pattern() : misa_file_pattern({ ".json" }) {

}

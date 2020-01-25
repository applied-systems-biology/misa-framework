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

#include <misaxx/core/misa_cache.h>
#include <misaxx/core/utils/filesystem.h>

boost::filesystem::path misaxx::misa_cache::get_internal_unique_location() const {
    boost::filesystem::path relative = boost::filesystem::relative(misaxx::utils::make_preferred(get_unique_location()),
            misaxx::utils::make_preferred(get_location()));
    return misaxx::utils::make_preferred(get_internal_location()) / relative;
}

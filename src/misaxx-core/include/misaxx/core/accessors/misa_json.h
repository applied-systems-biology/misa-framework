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

#pragma once

#include <misaxx/core/caches/misa_json_cache.h>
#include <misaxx/core/misa_cached_data.h>
#include <misaxx/core/misa_default_description_accessors.h>

namespace misaxx {
    /**
     * Cache that allows access to JSON data
     */
    struct misa_json : public misa_cached_data<misa_json_cache>,
            public misa_description_accessors_from_cache<misa_json_cache, misa_json> {
        using  misa_cached_data<misa_json_cache>::misa_cached_data;

        /**
         * Returns true if the JSON file already exists
         * @return
         */
        bool exists() const;
    };
}





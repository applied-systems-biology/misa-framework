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

#include <misaxx/core/misa_cached_data.h>
#include <misaxx-analyzer/caches/misa_output_cache.h>

namespace misaxx_analyzer {
    struct misa_output : public misaxx::misa_cached_data<misa_output_cache> {
        using misaxx::misa_cached_data<misa_output_cache>::misa_cached_data;

        /**
         * Returns a cache that gives access to attachment schemas
         * @return
         */
        const misaxx::misa_json get_attachment_schemas() const;

        /**
         * Returns the attachment caches
         * @return
         */
        std::vector<misaxx::misa_json> get_attachments() const;

        /**
         * Returns a cache that contains the full attachment schemas
         * @return
         */
        misaxx::misa_json get_full_attachment_schemas();

        /**
         * Returns the attachment index database
         * @return
         */
        attachment_index get_attachment_index();
    };
}





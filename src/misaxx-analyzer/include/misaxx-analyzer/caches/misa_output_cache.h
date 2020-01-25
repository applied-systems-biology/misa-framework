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

#include <misaxx/core/misa_default_cache.h>
#include <misaxx/core/accessors/misa_json.h>
#include <misaxx-analyzer/patterns/misa_output_pattern.h>
#include <misaxx-analyzer/accessors/attachment_index.h>

namespace misaxx_analyzer {
    struct misa_output_cache : public misaxx::misa_default_cache<misaxx::utils::memory_cache<boost::filesystem::path>,
        misa_output_pattern, misa_output_description> {

        /**
         * Contains the schemas of attachments
         */
        misaxx::misa_json m_attachment_schemas;

        /**
         * Contains the full set of attachment schemas
         */
        misaxx::misa_json m_full_attachment_schemas;

        /**
         * The attachments
         */
        std::vector<misaxx::misa_json> m_attachments;

        /**
         * Database that indexes all attachments
         */
        attachment_index m_attachment_index;

        void postprocess() override;

        void do_link(const misa_output_description &t_description) override;

    protected:
        misa_output_description
        produce_description(const boost::filesystem::path &t_location, const misa_output_pattern &t_pattern) override;
    };
}





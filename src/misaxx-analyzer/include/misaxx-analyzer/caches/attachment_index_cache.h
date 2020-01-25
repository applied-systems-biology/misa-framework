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

#include <misaxx/core/misa_cache.h>
#include <misaxx-analyzer/patterns/attachment_index_pattern.h>
#include <misaxx-analyzer/descriptions/attachment_index_description.h>
#include <misaxx/core/misa_default_cache.h>

namespace misaxx_analyzer {

    struct attachment_index_row {
        int id;
        std::string sample;
        std::string cache;
        std::string property;
        std::string serialization_id;
        std::string json_data;
    };

    struct attachment_index_database {
        virtual int insert(const attachment_index_row &row) { return 0; };
    };

    struct attachment_index_cache
            : public misaxx::misa_default_cache<misaxx::utils::cache<attachment_index_database>,
                    attachment_index_pattern, attachment_index_description> {
        void do_link(const attachment_index_description &t_description) override;

        attachment_index_database &get() override;

        const attachment_index_database &get() const override;

        void set(attachment_index_database value) override;

        bool has() const override;

        bool can_pull() const override;

        void pull() override;

        void stash() override;

        void push() override;

    protected:
        attachment_index_description produce_description(const boost::filesystem::path &t_location,
                                                         const attachment_index_pattern &t_pattern) override;

    private:
        std::shared_ptr<attachment_index_database> m_database;
    };
}





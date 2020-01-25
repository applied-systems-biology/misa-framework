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
#include <misaxx/core/descriptions/misa_file_description.h>
#include <misaxx/core/patterns/misa_file_pattern.h>
#include <misaxx/core/utils/cache.h>
#include <misaxx/core/descriptions/misa_exported_attachments_description.h>
#include <misaxx/core/patterns/misa_json_pattern.h>

namespace misaxx {

    /**
     * Cache that stores its attachments in a JSON file.
     * The unique location contains the absolute path of this file
     */
    struct misa_exported_attachments_cache : public misa_default_cache<misaxx::utils::cache<nlohmann::json>,
            misa_json_pattern, misa_exported_attachments_description> {

        nlohmann::json &get() override;

        const nlohmann::json &get() const override;

        void set(nlohmann::json value) override;

        bool has() const override;

        bool can_pull() const override;

        void pull() override;

        void stash() override;

        void push() override;

        void do_link(const misa_exported_attachments_description &t_description) override;

        /**
         * Saves the metadata included in this instance to the target JSON file
         */
        void save_attachments();

        void postprocess() override;

        void simulate_link() override;

    protected:

        misa_exported_attachments_description
        produce_description(const boost::filesystem::path &, const misa_json_pattern &) override;

    private:

        boost::filesystem::path m_path;
        nlohmann::json m_json = nlohmann::json::object();
    };
}
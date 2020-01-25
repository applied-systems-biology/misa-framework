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
#include <misaxx/core/patterns/misa_json_pattern.h>
#include <misaxx/core/descriptions/misa_json_description.h>

namespace misaxx {
    struct misa_json_cache : public misa_default_cache<utils::cache<nlohmann::json>,
            misa_json_pattern, misa_json_description>{
        void simulate_link() override;

        void do_link(const misa_json_description &t_description) override;

        const boost::filesystem::path &get_filename() const;

        nlohmann::json &get() override;

        const nlohmann::json &get() const override;

        void set(nlohmann::json value) override;

        bool has() const override;

        bool can_pull() const override;

        void pull() override;

        void stash() override;

        void push() override;

    protected:
        misa_json_description
        produce_description(const boost::filesystem::path &t_location, const misa_json_pattern &t_pattern) override;

    private:

        boost::filesystem::path m_filename;
        nlohmann::json m_data;
        bool m_loaded = false;
    };
}





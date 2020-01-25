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

#include <misaxx/core/misa_data_pattern.h>
#include <misaxx/core/descriptions/misa_file_description.h>

namespace misaxx {

    /**
     * A pattern that can set the filename of misaxx::misa_file_description description types
     */
    struct misa_file_pattern : public misa_data_pattern {

        /**
         * An optional preset filename. If it is present, the pattern will always use this filename
         */
        boost::filesystem::path filename;

        /**
         * Extensions that the file can have
         * If empty, the pattern accepts any file
         * This has no effect if the filename is not empty
         */
        std::vector<boost::filesystem::path> extensions;

        misa_file_pattern() = default;

        explicit misa_file_pattern(std::vector<boost::filesystem::path> t_extensions);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        /**
         * Returns true if the pattern has already a filename
         * @return
         */
        bool has_filename() const;

        /**
         * Returns true if the pattern has a set of file extensions to look for
         * @return
         */
        bool has_extensions() const;

        /**
         * Returns true if the path matches the pattern
         * @param t_path
         * @return
         */
        virtual bool matches(const boost::filesystem::path &t_path) const;

        /**
         * Sets the filename of the description
         * Requires that the pattern already has a filename
         * @param target
         */
        virtual void apply(misa_file_description &target) const;

        /**
         * Applies the pattern and
         * sets the filename of the description
         * @param target
         * @param t_directory
         */
        virtual void apply(misa_file_description &target, const boost::filesystem::path &t_directory) const;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_file_pattern& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_file_pattern& p) {
        p.from_json(j);
    }
}

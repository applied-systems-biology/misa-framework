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

#include <string>
#include <misaxx/core/misa_serializable.h>

namespace misaxx {
    /**
     * Contains basic information about a MISA++ module
     */
    class misa_module_info : public misa_serializable {
    public:
        misa_module_info() = default;

        /**
         * A short, lowercase name without spaces or special characters
         * @return
         */
        std::string get_id() const;

        /**
         * The version of the module
         * @return
         */
        std::string get_version() const;

        /**
         * A short, descriptive name of the module
         * @return
         */
        std::string get_name() const;

        /**
         * A description of the module
         * @return
         */
        std::string get_description() const;

        /**
         * The list of dependency modules
         * @return
         */
        std::vector<misa_module_info> get_dependencies() const;

        /**
         * Website of this module
         * @return
         */
        std::string get_url() const;

        /**
         * List of authors
         * @return
         */
        std::vector<std::string> get_authors() const;

        /**
         * Organization that developed this module
         * @return
         */
        std::string get_organization() const;

        /**
         * License of the module
         * @return
         */
        std::string get_license() const;

        /**
         * Information about how to cite the module
         * @return
         */
        std::string get_citation() const;

        /**
         * Sets the ID
         * @param t_id
         */
        void set_id(std::string t_id);

        /**
         * Sets the name
         * @param t_name
         */
        void set_name(std::string t_name);

        /**
         * Sets the description
         * @param t_description
         */
        void set_description(std::string t_description);

        /**
         * Sets the version
         * @param t_version
         */
        void set_version(std::string t_version);

        /**
         * Adds a dependency module info
         * @param t_dependency
         */
        void add_dependency(misa_module_info t_dependency);

        /**
         * Sets a URL
         * @param t_url
         */
        void set_url(std::string t_url);

        /**
         * Adds an author
         * @param t_author
         */
        void add_author(std::string t_author);

        /**
         * Sets the list of authors
         * @param t_authors
         */
        void set_authors(std::vector<std::string> t_authors);

        /**
         * Sets the organization
         * @param t_organization
         */
        void set_organization(std::string t_organization);

        /**
         * Sets the licence name
         * @param t_license
         */
        void set_license(std::string t_license);

        /**
         * Sets the citation
         * @param t_citation
         */
        void set_citation(std::string t_citation);

        /**
         * Sets if an external library is described
         * @param t_flag
         */
        void set_is_external(bool t_flag);

        /**
         * If true, an external non-MISA++ dependency is described
         * @return
         */
        bool is_external() const;

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

    private:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;

        std::string m_id;
        std::string m_version;
        std::string m_name;
        std::string m_description;
        std::vector<misa_module_info> m_dependencies;

        // Additional metadata
        std::string m_url;
        std::vector<std::string> m_authors;
        std::string m_organization;
        std::string m_license;
        std::string m_citation;

        // External (non-MISA++) dependencies
        bool m_is_external = false;
    };

    inline void to_json(nlohmann::json& j, const misa_module_info& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_module_info& p) {
        p.from_json(j);
    }
}




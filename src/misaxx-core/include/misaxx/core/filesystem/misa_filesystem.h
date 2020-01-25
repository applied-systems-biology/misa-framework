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

#include <misaxx/core/filesystem/misa_filesystem_entry.h>

namespace misaxx {
    /**
     * Filesystem of a module
     * It has two folders "imported" and "exported"
     */
    struct misa_filesystem : public misa_serializable {
        /**
         * Contains all imported data
         */
        filesystem::entry imported;
        /**
         * Contains all exported / output data
         */
        filesystem::entry exported;

        /**
         * Returns true if this filesystem is valid
         * @return
         */
        bool is_valid() const;

        /**
         * Creates a sub-filesystem
         * @param t_name
         * @return
         */
        misa_filesystem create_subsystem(const std::string &t_name);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        std::shared_ptr<misa_filesystem_entry> find_external_path(const boost::filesystem::path &t_path) const;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };
}

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

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <misaxx/core/filesystem/misa_filesystem.h>
#include <misaxx/core/filesystem/misa_filesystem_entry.h>
#include <iostream>

namespace misaxx {

    /**
     * Imports a MISA++ filesystem from JSON data
     */
    struct misa_filesystem_json_importer {

        nlohmann::json input_json;
        boost::filesystem::path json_path;

        /**
         * Internal function used for importing
         * @param t_json
         * @param t_entry
         */
        void import_entry(const nlohmann::json &t_json, const filesystem::entry &t_entry);

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import();
    };
}
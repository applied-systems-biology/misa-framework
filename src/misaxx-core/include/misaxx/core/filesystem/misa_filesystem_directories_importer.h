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
#include <misaxx/core/filesystem/misa_filesystem.h>
#include <misaxx/core/filesystem/misa_filesystem_entry.h>
#include <iostream>

namespace misaxx {

    /**
     * Imports a filesystem from an input folder.
     * The "imported" directory maps to the input path.
     * The "exported" directory maps to the output path.
     */
    struct misa_filesystem_directories_importer {
        boost::filesystem::path input_path;
        boost::filesystem::path output_path;

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import();

    private:

        void discoverImporterEntry(const filesystem::entry &t_entry);

    };
}

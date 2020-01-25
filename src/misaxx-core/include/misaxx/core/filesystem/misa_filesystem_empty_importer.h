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

#include <boost/filesystem/path.hpp>
#include <misaxx/core/filesystem/misa_filesystem_entry.h>
#include <misaxx/core/filesystem/misa_filesystem.h>

namespace misaxx {


    /**
     * Empty importer used by parameter schema builder
     */
    struct misa_filesystem_empty_importer {

        /**
         * Imports the filesystem
         * @return
         */
        misa_filesystem import();
    };
}

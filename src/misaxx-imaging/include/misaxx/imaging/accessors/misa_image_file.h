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

#include <misaxx/core/misa_cached_data.h>
#include <misaxx/imaging/caches/misa_image_file_cache.h>
#include <misaxx/core/misa_default_description_accessors.h>

namespace misaxx::imaging {

    /**
     * A simple 2D image file
     */
     struct misa_image_file : public misaxx::misa_cached_data<misa_image_file_cache>,
                                                   public misaxx::misa_description_accessors_from_cache<misa_image_file_cache, misa_image_file> {
        /**
         * Clones the image content read from access_readonly()
         * @return
         */
        cv::Mat clone() const;

        /**
         * Writes image data into the current file
         * @param t_data
         */
        void write(cv::Mat t_data);
    };
}




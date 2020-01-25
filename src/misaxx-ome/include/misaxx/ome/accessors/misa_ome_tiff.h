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

#include <misaxx/ome/caches/misa_ome_tiff_cache.h>
#include <misaxx/core/misa_cached_data.h>
#include <misaxx/ome/misa_ome_tiff_description_builder.h>
#include <misaxx/ome/misa_ome_tiff_description_modifier.h>

namespace misaxx::ome {
    /**
     * An OME TIFF file that contains a list of 2D image planes.
     * The planes can be accessed via their index within this structure or using a misa_ome_plane_location
     * that also encodes semantic location within a time/depth/channel space.
     */
    struct misa_ome_tiff : public misaxx::misa_cached_data<misa_ome_tiff_cache>,
                           public misaxx::misa_description_accessors_from_cache<misa_ome_tiff_cache, misa_ome_tiff> {

        using iterator = typename std::vector<misa_ome_plane>::iterator;
        using const_iterator = typename std::vector<misa_ome_plane>::const_iterator;

        iterator begin();

        iterator end();

        const_iterator begin() const;

        const_iterator end() const;

        size_t size() const;

        bool empty() const;

        misa_ome_plane at(size_t index) const;

        misa_ome_plane at(const misa_ome_plane_description &index);

        /**
         * Width of each plane in the TIFF
         * @param series
         * @return
         */
        size_t get_size_x(size_t series = 0) const;

        /**
         * Height of each plane in the TIFF
         * @param series
         * @return
         */
        size_t get_size_y(size_t series = 0) const;

        /**
         * Number of planes allocated within the time axis
         * @param series
         * @return
         */
        size_t get_size_t(size_t series = 0) const;

        /**
         * Number of planes allocated within the channel axis
         * This is not equal to the number of channels each pixel consists of.
         * @param series
         * @return
         */
        size_t get_size_c(size_t series = 0) const;

        /**
         * Number of planes allocated within the depth axis
         * @param series
         * @return
         */
        size_t get_size_z(size_t series = 0) const;

        /**
         * Returns a description builder that allows changing properties of the description more easily
         * The description builder works on a copy
         * @return
         */
        misa_ome_tiff_description_modifier derive() const;

        /**
         * Returns the OME TIFF metadata storage
         * @return
         */
        std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> get_ome_metadata() const;

        /**
         * Creates a builder that allows creating a TIFF description from scratch
         * Please note that if a source description is provided, additional metadata is not copied.
         * Use derive() instead in this case
         * @param src Optional source description
         * @return
         */
        static misa_ome_tiff_description_builder build(misa_ome_tiff_description src = misa_ome_tiff_description("image.ome.tif"));
    };
}





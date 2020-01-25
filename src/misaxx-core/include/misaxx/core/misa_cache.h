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
#include <misaxx/core/filesystem/misa_filesystem.h>
#include <misaxx/core/utils/dynamic_singleton_map.h>
#include <misaxx/core/utils/cache.h>

namespace misaxx {
    /**
     * Base class for all caches
     */
    struct misa_cache {

        using attachment_type = misaxx::utils::dynamic_singleton_map<misa_serializable>;
        using attachment_cache_type = misaxx::utils::memory_cache<attachment_type>;

        /**
         * Objects that are attached to this cache
         */
        attachment_cache_type attachments;

        /**
         * Links the cache to a filesystem location.
         * Assumes that data already exists within the location.
         * @param t_internal_location Internal cache location
         * @param t_location Absolute file path
         */
        virtual void link(const boost::filesystem::path &t_internal_location,
                const boost::filesystem::path &t_location,
                const std::shared_ptr<misa_description_storage> &t_description) = 0;

        /**
         * Describes the contents of this cache using filesystem metadata
         * @return
         */
        virtual std::shared_ptr<misa_description_storage> describe() const = 0;

        /**
         * Returns the path of this cache within the MISA++ filesystem.
         * The first segment of the path is "imported" or "exported"
         * @return
         */
        virtual boost::filesystem::path get_internal_location() const = 0;

        /**
         * Assumes that get_unique_location() is a child of get_location() and
         * returns get_internal_location() plus the realtive location
         * @return
         */
        boost::filesystem::path get_internal_unique_location() const;

        /**
         * Returns the location of this cache in the filesystem
         * @return
         */
        virtual boost::filesystem::path get_location() const = 0;

        /**
         * Unique location of this cache, which should be not equal with get_location() if a cache contains sub-caches
         * @return
         */
        virtual boost::filesystem::path get_unique_location() const = 0;

        /**
         * Returns true if the cache has currently data
         * @return
         */
        virtual bool has_data() = 0;

        /**
         * Applies postprocessing (e.g. saving data) after analysis
         */
        virtual void postprocess() {

        }

        /**
         * Returns the location interface of this cache
         * It should match the get_location() and get_unique_location() functions
         * Can be directly used for misa_locatable instances
         * Creates the location interface if necessary
         * @return
         */
        virtual std::shared_ptr<const misa_location> get_location_interface() const = 0;

    };
}

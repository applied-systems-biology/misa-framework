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

#include <misaxx/core/utils/dynamic_singleton_map.h>
#include <misaxx/core/utils/cache.h>
#include <misaxx/core/misa_serializable.h>
#include <misaxx/core/misa_cache.h>
#include <misaxx/core/misa_cached_data_base.h>
#include <misaxx/core/runtime/misa_cache_registry.h>
#include <misaxx/core/runtime/misa_runtime_properties.h>

namespace misaxx {

    /**
     * A shared pointer that manages cached data and comes with methods to import and export data from filesystem.
     * Please prefer interacting with cached data using this interface, as it automatically registers caches
     * into the runtime using misa_runtime_base::instance().register_cache.
     * @tparam Cache
     */
    template<class Cache>
    struct misa_cached_data : public misa_cached_data_base {

        using cache_type = Cache;
        using value_type = typename Cache::value_type;

        /**
         * Pointer to the internal cache data
         */
        std::shared_ptr<Cache> data;

        misa_cached_data() = default;

        /**
         * Creates cached data from an existing cache instance.
         * @param t_cache
         */
        explicit misa_cached_data(Cache t_cache);

        /**
         * Returns true if this cache is set from a parent module
         * @return
         */
        bool is_externally_set() const override;

        /**
         * Returns true if this cached data contains data
         * @return
         */
        bool has_data() const override;

        /**
         * Returns the cached data as pointer to misa_cache
         * @return
         */
        std::shared_ptr<misa_cache> get_cache_base() const override;

        /**
        * Links this cache to a filesystem location.
        * This calls the internal linkage method of the cached data.
        * If linking with filesystem entries, you can use the other methods
        * @param t_location
        * @param t_description
        */
        void force_link(const boost::filesystem::path &t_internal_location,
                        const boost::filesystem::path &t_location,
                        const std::shared_ptr<misa_description_storage> &t_description);

        /**
         * Links this cache to a filesystem location.
         * This calls the internal linkage method of the cached data.
         * If linking with filesystem entries, you can use the other methods
         * @param t_location
         * @param t_description
         */
        void force_link(const boost::filesystem::path &t_internal_location,
                        const boost::filesystem::path &t_location,
                        std::shared_ptr<misa_data_description> t_description);

        /**
         * Links this cache to a filesystem location.
         * The data is assumed to already exist. Necessary metadata should be contained within the filesystem.
         * Metadata is not copied during this operation.
         * @param t_location
         */
        void force_link(const filesystem::const_entry &t_location);

        /**
         * Links this cache to a filesystem location if not already set.
         * This calls the internal linkage method of the cached data.
         * If linking with filesystem entries, you can use the other methods
         * @param t_location
         * @param t_description
         */
        void suggest_link(const boost::filesystem::path &t_internal_location,
                          const boost::filesystem::path &t_location,
                          const std::shared_ptr<misa_description_storage> &t_description);

        /**
         * Links this cache to a filesystem location if not already set.
         * This calls the internal linkage method of the cached data.
         * If linking with filesystem entries, you can use the other methods
         * @param t_location
         * @param t_description
         */
        void suggest_link(const boost::filesystem::path &t_internal_location,
                          const boost::filesystem::path &t_location,
                          std::shared_ptr<misa_data_description> t_description);

        /**
         * Links this cache to a filesystem location if not already set.
         * The data is assumed to already exist. Necessary metadata should be contained within the filesystem.
         * Metadata is not copied during this operation.
         * @param t_location
         */
        void suggest_link(const filesystem::const_entry &t_location);

        /**
        * Links this cache to a filesystem location if not already set.
        * The data is assumed to not exist. Necessary metadata must be obtained from linked caches or manually set in code.
        * Metadata is copied if it is not unique
        * @param t_location
        */
        void suggest_create(const filesystem::entry &t_location,
                            const std::shared_ptr<misa_description_storage> &t_description);

        /**
         * Links this cache to a filesystem location if not already set.
         * The data is assumed to not exist. Necessary metadata must be obtained from linked caches or manually set in code.
         * Metadata is copied if it is not unique
         * @param t_location
         * @param t_description
         */
        void suggest_create(const filesystem::entry &t_location, std::shared_ptr<misa_data_description> t_description);

        /**
         * Links this cache to an imported filesystem path.
         * This path must exist. Otherwise, an exception is thrown.
         * @param t_filesystem
         * @param t_path
         */
        void suggest_import_location(const misa_filesystem &t_filesystem, const boost::filesystem::path &t_path);

        /**
         * Links this cache to an exported filesystem path.
         * This path must not exist. Otherwise, an exception is thrown.
         * @param t_filesystem
         * @param t_path
         * @param t_description
         */
        void suggest_export_location(const misa_filesystem &t_filesystem,
                                     const boost::filesystem::path &t_path,
                                     const std::shared_ptr<misa_description_storage> &t_description);

        /**
         * Links this cache to an exported filesystem path.
         * This path must not exist. Otherwise, an exception is thrown.
         * @param t_filesystem
         * @param t_path
         * @param t_description
         */
        void suggest_export_location(const misa_filesystem &t_filesystem,
                                     const boost::filesystem::path &t_path,
                                     std::shared_ptr<misa_data_description> t_description);

        /**
         * Returns a description of the current cache
         * @return
         */
        std::shared_ptr<misa_description_storage> describe() const override;

        /**
         * Thread-safe read-only access to the data.
         * @return
         */
        readonly_access <value_type> access_readonly() const {
            return readonly_access<value_type>(*data);
        }

        /**
        * Thread-safe read & write access to the data.
        * Other threads are blocked from access.
        * @return
        */
        readwrite_access <value_type> access_readwrite() {
            return readwrite_access<value_type>(*data);
        }

        /**
         * Thread-safe write-only access to the data.
         * Other threads are blocked from access.
         * @return
         */
        write_access <value_type> access_write() {
            return write_access<value_type>(*data);
        }

        /**
         * Returns the location of the cache.
         * This is the folder that contains the data. Please note that
         * this location might not be unique for different caches, as multiple caches might be created
         * on the same location.
         * Use get_unique_location() to find the actual file instead.
         * @return
         */
        boost::filesystem::path get_location() const override;

        /**
         * Returns the unique location of the cache.
         * This usually points to the actual file containing the data
         * @return
         */
        boost::filesystem::path get_unique_location() const override;

        /**
        * Returns the location of the cache within the internal file system
        * This is the folder that contains the data. Please note that
        * this location might not be unique for different caches, as multiple caches might be created
        * on the same location.
        * Use get_unique_location() to find the actual file instead.
        * @return
        */
        boost::filesystem::path get_internal_location() const override;

        /**
         * Gets the location interface of this cache. This location interface is compatible with
         * misaxx::misa_locatable instances.
         * @return
         */
        std::shared_ptr<const misa_location> get_location_interface() const override;

    };
}

#include "detail/misa_cached_data.h"
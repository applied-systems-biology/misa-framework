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

#include <memory>
#include <vector>
#include <unordered_set>

namespace misaxx {
    struct misa_cache;
    struct misa_cached_data_base;
}

/**
 * Contains helpers to interact with caches
 */
namespace misaxx::cache_registry {
    /**
     * Registers a cache into the runtime
     * This will enable postprocessing of the cache
     * @param t_cache
     */
    extern void register_cache(std::shared_ptr<misa_cache> t_cache);

    /**
     * Removes a cache from the registry
     * @param t_cache
     * @return if successful
     */
    extern bool unregister_cache(const std::shared_ptr<misa_cache> &t_cache);

    /**
     * Removes a cache from the registry
     * @param t_cache
     * @return if successful
     */
    extern bool unregister_cache(const misa_cached_data_base &t_cache);

    /**
     * Returns the registered caches
     * @return
     */
    extern const std::unordered_set<std::shared_ptr<misa_cache>> &get_registered_caches();
}
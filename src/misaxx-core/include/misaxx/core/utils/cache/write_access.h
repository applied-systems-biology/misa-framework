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

#include <mutex>
#include <shared_mutex>
#include <misaxx/core/utils/cache/cache.h>

namespace misaxx::utils {
    /**
     * Cache access that only writes data to the cache.
     * @tparam Cache
     */
    template<typename Value> struct write_access {
    public:
        using value_type = Value;

        explicit write_access(cache<Value> &t_cache) : m_cache(&t_cache), m_lock(t_cache.exclusive_lock()) {
            m_lock.lock();
        }

        ~write_access() {
            m_cache->push(); // Push into the cache
            m_cache->stash(std::move(m_lock));
        }

        write_access(const write_access<Value> &src) = delete;

        write_access(write_access<Value> &&src) noexcept = default;

        void set(value_type t_value) {
            m_cache->set(std::move(t_value));
        }

    private:
        cache<Value> *m_cache;
        std::unique_lock<std::shared_mutex> m_lock;
    };
}

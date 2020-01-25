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
    template<typename Value>
    class readonly_access {
    public:
        using value_type = Value;

        explicit readonly_access(cache<Value> &t_cache) : m_cache(&t_cache), m_lock(t_cache.shared_lock()) {
            m_lock.lock();
            m_cache->pull();
        }

        ~readonly_access() {
            m_cache->try_stash(std::move(m_lock)); // Push back into the cache
        }

        readonly_access(const readonly_access<Value> &src) = delete;

        readonly_access(readonly_access<Value> &&src) noexcept = default;

        const value_type &get() const {
            return m_cache->get();
        }

        value_type &get() {
            return m_cache->get();
        }

        const value_type &operator ->() const {
            return m_cache->get();
        }

        value_type &operator ->() {
            return m_cache->get();
        }

    private:
        cache<Value> *m_cache;
        std::shared_lock<std::shared_mutex> m_lock;
    };
}

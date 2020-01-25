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

namespace misaxx::utils {
    /**
     * Models a value (e.g. a pointer) that is associated with a lock
     * @tparam T
     * @tparam Lock
     */
    template<typename T, class Lock> class locked {
    public:
        using value_type = T;
        using lock_type = Lock;
        using mutex_type = typename Lock::mutex_type;

        T value;

        locked(T t_value, mutex_type &t_mutex) : value(std::move(t_value)), m_lock(Lock(t_mutex, std::defer_lock)) {
            m_lock.lock();
        }

        locked(T t_value, lock_type t_lock) : value(std::move(t_value)), m_lock(std::move(t_lock)) {
            if(!static_cast<bool>(m_lock)) {
                m_lock.lock();
            }
        }
    private:
        Lock m_lock;
    };
}

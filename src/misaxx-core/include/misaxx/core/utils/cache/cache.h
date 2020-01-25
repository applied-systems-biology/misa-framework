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
#include <mutex>
#include <shared_mutex>

namespace misaxx::utils {

    /**
     * Base class for all cached values.
     * To access the values in this cache safely, use the exclusive_access and readonly_access types!
     * @tparam Value
     */
    template<typename Value> struct cache {

    public:

        /**
         * The value stored inside this cache
         */
        using value_type = Value;

        cache() = default;

        virtual ~cache() noexcept {
            // throw an exception if we would clear away existing accesses to this cache
            if(!m_mutex.try_lock())
                std::terminate();
            else {
                m_mutex.unlock();
            }
        }

        cache(const cache<Value> &t_src) = delete;

        cache(cache<Value> &&t_src) noexcept {
            // We need to manually handle the mutex
            // Mutexes cannot be moved. We just gain cache to the mutex and do manual moving
            if(!t_src.m_mutex.try_lock())
                std::terminate();
            else {
                t_src.m_mutex.unlock();
            }

            // Nothing to move
        };

        /**
         * Creates a lock that gives exclusive access to the thread that runs this method.
         * The lock is deferred.
         * @return
         */
        std::unique_lock<std::shared_mutex> exclusive_lock() {
            return std::unique_lock<std::shared_mutex>(m_mutex, std::defer_lock);
        }

        /**
         * Creates a lock that gives exclusive access to the thread that runs this method.
         * The lock is deferred.
         * @return
         */
        std::shared_lock<std::shared_mutex> shared_lock() {
            return std::shared_lock<std::shared_mutex>(m_mutex, std::defer_lock);
        }

        /**
         * Gets the value. If it is not available, an exception is thrown.
         * Not thread-safe!
         * @return
         */
        virtual Value &get() = 0;

        /**
         * Gets the value. If it is not available, an exception is thrown.
         * Not thread-safe!
         * @return
         */
        virtual const Value &get() const = 0;

        /**
         * Sets the value in this cache from memory
         * Not thread-safe!
         * @param value
         */
        virtual void set(Value value) = 0;

        /**
         * Gets the value. If it is not available, pull() it
         * Not thread-safe!
         * @return
         */
        Value &access() {
            if(!has())
                pull();
            return get();
        }

        /**
         * Returns true if the value is currently in memory
         * Not thread-safe!
         * @return
         */
        virtual bool has() const = 0;

        /**
         * Returns true if the cache can pull data
         * Not thread-safe!
         * @return
         */
        virtual bool can_pull() const = 0;

        /**
         * Pulls the value into the memory
         * Not thread-safe!
         */
        virtual void pull() = 0;

        /**
         * Discard the current value and do not write it into the permanent storage
         * Please make sure that there is no other object accessing the data!
         * Not thread-safe!
         */
        virtual void stash() = 0;

        /**
         * Free the memory of the value and put it back into the permanent storage
         * Please make sure that there is no other object accessing the data!
         * Not thread-safe!
         */
        virtual void push() = 0;


        /**
         * Tries to discard the current value with stash(). Only works if there is no other access.
         * Can be safely used from multiple threads.
         * @param existing_lock An existing lock that should be taken over
         */
        void try_stash(std::shared_lock<std::shared_mutex> existing_lock = {}) {
            if(existing_lock.owns_lock())
                existing_lock.unlock();

            // Need to aquire an exclusive lock
            auto lock = exclusive_lock();
            if(lock.try_lock()) {
                stash();
            }
        }

        /**
         * Thread-saft stash() method
         * @param existing_lock
         */
        void stash(std::unique_lock<std::shared_mutex>) {
            stash();
        }

    private:

        /**
        * Mutex that governs access to the data
        */
        std::shared_mutex m_mutex;
    };
}

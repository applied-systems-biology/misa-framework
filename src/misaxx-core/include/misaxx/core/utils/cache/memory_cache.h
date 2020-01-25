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

#include <misaxx/core/utils/cache/cache.h>

namespace misaxx::utils {
    /**
     * Convenience class that caches a value in memory, but allows thread-safe interaction
     * @tparam Value
     */
    template<typename Value> class memory_cache : public cache<Value> {
    public:

        memory_cache() = default;

        explicit memory_cache(Value value) : m_value(std::move(value)) {

        }

        Value &get() override {
            return m_value;
        }

        const Value &get() const override {
            return m_value;
        }

        void set(Value value) override {
            m_value = std::move(value);
        }

        bool has() const override {
            return true;
        }

        bool can_pull() const override {
            return true;
        }

        void pull() override {

        }

        void stash() override {

        }

        void push() override {

        }
    private:
        Value m_value;
    };
}

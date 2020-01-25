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
#include <misaxx/core/patterns/misa_dummy_pattern.h>
#include <misaxx/core/misa_default_cache.h>

namespace misaxx {

    /**
     * Convenience cache type that is manually linked by other caches (e.g. sub-images) and therefor does not
     * require the pattern-description approach used by other caches.
     * This type of cache only requires a pattern
     * @tparam Cache
     * @tparam Description
     */
    template<class Cache, class Description> class misa_manual_cache : public misa_default_cache<Cache, misa_dummy_pattern, Description> {
    protected:
        Description produce_description(const boost::filesystem::path &t_location, const misa_dummy_pattern &t_pattern) override {
            // Manual linkage will create a proper description
            return Description();
        }
    };
}

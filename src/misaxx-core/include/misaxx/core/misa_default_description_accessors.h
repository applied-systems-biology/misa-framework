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

namespace misaxx {
    /**
     * Convenience type for creating accessors.
     * @tparam Pattern The pattern that is associated to the underlying cache
     * @tparam Description The description that is associated to the underlying cache
     */
    template<class Pattern, class Description, class Derived> struct misa_default_description_accessors {
        const Pattern &get_data_pattern() const {
            return static_cast<const Derived*>(this)->describe()->template get<Pattern>();
        }

        const Description &get_data_description() const {
            return static_cast<const Derived*>(this)->describe()->template get<Description>();
        }
    };

    /**
     * Derives misa_default_description_accessors from a cache type
     */
    template<class Cache, class Derived> using misa_description_accessors_from_cache = misa_default_description_accessors<typename Cache::pattern_type, typename Cache::description_type, Derived>;
}

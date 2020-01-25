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


#include <functional>

namespace misaxx::utils {

    /**
     * Convenience shortcut for optional reference wrapper similar to a C# ref
     */
    template<typename T> using ref = std::optional<std::reference_wrapper<T>>;

    /**
     * ref value that indicates that it does not reference any variable
     */
    inline constexpr std::nullopt_t no_ref = std::nullopt;

    /**
     * Returns true if the ref references a variable
     * @tparam Ref
     * @param t_ref
     * @return
     */
    template<class Ref> inline bool ref_has_variable(const Ref &t_ref) {
        return t_ref.has_value();
    };

    /**
     * Assigns a value to the provided ref.
     * Throws an exception if the ref does not reference a variable
     * @tparam Ref
     * @tparam T
     * @param t_ref
     * @param t_value
     */
    template<class Ref, class T> inline void assign_ref(const Ref &t_ref, T &&t_value) {
        t_ref.value().get() = std::forward<T>(t_value);
    }

    /**
     * Assigns the value to the ref if possible (the ref references a variable)
     * @tparam Ref
     * @tparam T
     * @param t_ref
     * @param t_value
     * @return
     */
    template<class Ref, class T> inline bool try_assign_ref(const Ref &t_ref, T &&t_value) {
        if(ref_has_variable(t_ref)) {
            assign_ref(t_ref, t_value);
            return true;
        }
        return false;
    }

}
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

namespace misaxx::imaging::utils {

    /**
     * Cantor pairing function that transforms two natural numbers into a unique numerical representation of those numbers.
     * @tparam T
     * @param x
     * @param y
     * @return
     */
    template<class T> T cantor_pairing(T x, T y) {
        return static_cast<T>(y + 0.5 * (x + y) * (x + y + 1));
    }

}
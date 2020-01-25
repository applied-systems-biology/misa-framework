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

#include <string>
#include <boost/lexical_cast.hpp>

namespace misaxx::utils {
    /**
     * Converts the input into a string similar to using std::to_string or a stream buffer.
     * Shortcut for boost::lexical_cast<std::string>
     *
     * @tparam Source
     * @param t_src
     * @return
     */
    template<class Source> inline std::string to_string(const Source &t_src) {
        return boost::lexical_cast<std::string>(t_src);
    }
}

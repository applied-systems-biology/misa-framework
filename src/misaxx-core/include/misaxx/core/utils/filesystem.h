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


#include <boost/filesystem/path.hpp>

namespace misaxx::utils {

    /**
    * Returns the relative path of the input path to its direct parent
    * This differs from Boost's relative() function by not returning a path if the parent is not a direct parent
    * @param t_parent
    * @param t_path
    * @return
    */
    extern boost::filesystem::path relativize_to_direct_parent(boost::filesystem::path t_parent, boost::filesystem::path t_path);

    /**
     * Converts the input path into a preferred representation.
     * For example, Windows paths are converted into Cygwin paths if Cygwin is detected
     * Returns the input path if no conversion is needed
     * @param path
     * @return
     */
    extern boost::filesystem::path make_preferred(boost::filesystem::path path);

}



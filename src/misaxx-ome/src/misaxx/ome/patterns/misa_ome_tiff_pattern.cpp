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

#include <misaxx/ome/patterns/misa_ome_tiff_pattern.h>

using namespace misaxx;
using namespace misaxx::ome;

misa_ome_tiff_pattern::misa_ome_tiff_pattern() : misaxx::misa_file_pattern({ ".tif", ".tiff" }) {

}

std::string misa_ome_tiff_pattern::get_documentation_name() const {
    return "OME TIFF pattern";
}

std::string misa_ome_tiff_pattern::get_documentation_description() const {
    return "Finds a *.tiff/*.tif file";
}

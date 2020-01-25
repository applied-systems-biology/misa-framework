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

#include <misaxx/imaging/patterns/misa_image_pattern.h>

using namespace misaxx;
using namespace misaxx::imaging;

misa_image_pattern::misa_image_pattern() : misa_file_pattern({ ".tif", ".tiff", ".png",
                                                                         ".bmp", ".pbm", ".pgm",
                                                                         ".ppm", ".jpeg", ".jpg",
                                                                         ".jpe", ".jp2" }) {

}

void misa_image_pattern::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_file_pattern::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa-imaging", "patterns/image-file"));
}

std::string misa_image_pattern::get_documentation_name() const {
    return "Image file pattern";
}

std::string misa_image_pattern::get_documentation_description() const {
    std::vector<std::string> ext;
    for(const auto &e : extensions) {
        ext.emplace_back(e.string());
    }
    return "Look for an image file that has one of the following extensions: " + boost::algorithm::join(ext, ", ");
}

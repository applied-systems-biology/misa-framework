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

#include <misaxx/imaging/accessors/misa_image_file.h>

cv::Mat misaxx::imaging::misa_image_file::clone() const {
    return this->access_readonly().get().clone();
}

void misaxx::imaging::misa_image_file::write(cv::Mat t_data) {
    this->access_write().set(std::move(t_data));
}

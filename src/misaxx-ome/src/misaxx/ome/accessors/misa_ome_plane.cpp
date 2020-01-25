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

#include <misaxx/ome/accessors/misa_ome_plane.h>
#include "../utils/ome_tiff_io.h"

cv::Mat misaxx::ome::misa_ome_plane::clone() const {
    return this->access_readonly().get().clone();
}

void misaxx::ome::misa_ome_plane::write(cv::Mat t_data) {
    this->access_write().set(std::move(t_data));
}

const misaxx::ome::misa_ome_plane_description &misaxx::ome::misa_ome_plane::get_plane_location() const {
    return this->data->get_plane_location();
}

size_t misaxx::ome::misa_ome_plane::get_size_x() const {
    return this->data->get_tiff_io()->get_size_x(get_plane_location().series);
}

size_t misaxx::ome::misa_ome_plane::get_size_y() const {
    return this->data->get_tiff_io()->get_size_y(get_plane_location().series);
}

std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> misaxx::ome::misa_ome_plane::get_ome_metadata() const {
    return this->data->get_tiff_io()->get_metadata();
}

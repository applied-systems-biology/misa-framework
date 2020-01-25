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

#include <misaxx/ome/accessors/misa_ome_tiff.h>
#include "../utils/ome_tiff_io.h"

misaxx::ome::misa_ome_tiff::iterator misaxx::ome::misa_ome_tiff::begin() {
    return this->data->get().begin();
}

misaxx::ome::misa_ome_tiff::iterator misaxx::ome::misa_ome_tiff::end() {
    return this->data->get().end();
}

misaxx::ome::misa_ome_tiff::const_iterator misaxx::ome::misa_ome_tiff::begin() const {
    return this->data->get().begin();
}

misaxx::ome::misa_ome_tiff::const_iterator misaxx::ome::misa_ome_tiff::end() const {
    return this->data->get().end();
}

size_t misaxx::ome::misa_ome_tiff::size() const {
    return this->data->get().size();
}

bool misaxx::ome::misa_ome_tiff::empty() const {
    return this->data->get().empty();
}

misaxx::ome::misa_ome_plane misaxx::ome::misa_ome_tiff::at(size_t index) const {
    return this->data->get().at(index);
}

misaxx::ome::misa_ome_plane misaxx::ome::misa_ome_tiff::at(const misaxx::ome::misa_ome_plane_description &index) {
    return this->data->get_plane(index);
}

size_t misaxx::ome::misa_ome_tiff::get_size_x(size_t series) const {
    return this->data->get_tiff_io()->get_size_x(series);
}

size_t misaxx::ome::misa_ome_tiff::get_size_y(size_t series) const {
    return this->data->get_tiff_io()->get_size_y(series);
}

size_t misaxx::ome::misa_ome_tiff::get_size_t(size_t series) const {
    return this->data->get_tiff_io()->get_size_t(series);
}

size_t misaxx::ome::misa_ome_tiff::get_size_c(size_t series) const {
    return this->data->get_tiff_io()->get_size_c(series);
}

size_t misaxx::ome::misa_ome_tiff::get_size_z(size_t series) const {
    return this->data->get_tiff_io()->get_size_z(series);
}

misaxx::ome::misa_ome_tiff_description_modifier misaxx::ome::misa_ome_tiff::derive() const {
    return misa_ome_tiff_description_modifier(this->get_data_description());
}

std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> misaxx::ome::misa_ome_tiff::get_ome_metadata() const {
    return this->data->get_tiff_io()->get_metadata();
}

misaxx::ome::misa_ome_tiff_description_builder
misaxx::ome::misa_ome_tiff::build(misaxx::ome::misa_ome_tiff_description src) {
    return misa_ome_tiff_description_builder(std::move(src));
}

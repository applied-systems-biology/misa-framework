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

#include <misaxx/ome/caches/misa_ome_plane_cache.h>
#include <misaxx/ome/attachments/misa_ome_planes_location.h>
#include "../utils/ome_tiff_io.h"

cv::Mat &misaxx::ome::misa_ome_plane_cache::get() {
    return m_cached_image;
}

const cv::Mat &misaxx::ome::misa_ome_plane_cache::get() const {
    return m_cached_image;
}

void misaxx::ome::misa_ome_plane_cache::set(cv::Mat value) {
    m_cached_image = std::move(value);
}

bool misaxx::ome::misa_ome_plane_cache::has() const {
    return !m_cached_image.empty();
}

bool misaxx::ome::misa_ome_plane_cache::can_pull() const {
    return static_cast<bool>(m_tiff);
}

void misaxx::ome::misa_ome_plane_cache::pull() {
    m_cached_image = m_tiff->read_plane(get_plane_location());
}

void misaxx::ome::misa_ome_plane_cache::stash() {
    m_cached_image.release();
}

void misaxx::ome::misa_ome_plane_cache::push() {
    if (m_cached_image.empty())
        throw std::runtime_error("Trying to write empty image to TIFF!");
    m_tiff->write_plane(m_cached_image, get_plane_location());
}

void misaxx::ome::misa_ome_plane_cache::do_link(const misaxx::ome::misa_ome_plane_description &t_description) {
    // Won't do anything, as we depend on the tiff_reader (and internal coordinates)
    if(!static_cast<bool>(m_tiff)) {
        throw std::runtime_error("Cannot link OME TIFF plane without a TIFF IO!");
    }
    this->set_unique_location(this->get_location() / "planes" /  (misaxx::utils::to_string(t_description) + ".tif"));
    std::cout << "[Cache] Linking OME TIFF plane @ " << t_description << "\n";
}

void misaxx::ome::misa_ome_plane_cache::set_tiff_io(std::shared_ptr<misaxx::ome::ome_tiff_io> t_tiff) {
    m_tiff = std::move(t_tiff);
}

std::shared_ptr<misaxx::ome::ome_tiff_io> misaxx::ome::misa_ome_plane_cache::get_tiff_io() const {
    return m_tiff;
}

const misaxx::ome::misa_ome_plane_description &misaxx::ome::misa_ome_plane_cache::get_plane_location() const {
    return this->describe()->template get<misa_ome_plane_description>();
}

std::shared_ptr<misaxx::misa_location> misaxx::ome::misa_ome_plane_cache::create_location_interface() const {
    auto result = std::make_shared<misaxx::ome::misa_ome_planes_location>();
    result->internal_location = get_internal_location();
    result->filesystem_location = get_location();
    result->filesystem_unique_location = get_unique_location();
    result->planes = { get_plane_location() };
    return result;
}

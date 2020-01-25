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

#include <misaxx/imaging/caches/misa_image_file_cache.h>
#include <misaxx/imaging/utils/tiffio.h>

cv::Mat &misaxx::imaging::misa_image_file_cache::get() {
    return m_value;
}

const cv::Mat &misaxx::imaging::misa_image_file_cache::get() const {
    return m_value;
}

void misaxx::imaging::misa_image_file_cache::set(cv::Mat value) {
    m_value = std::move(value);
}

bool misaxx::imaging::misa_image_file_cache::has() const {
    return !m_value.empty();
}

bool misaxx::imaging::misa_image_file_cache::can_pull() const {
    return boost::filesystem::is_regular_file(m_path);
}

void misaxx::imaging::misa_image_file_cache::pull() {
    if(m_path.has_extension() && (m_path.extension().string() == ".tif" || m_path.extension().string() == ".tiff")) {
        m_value = misaxx::imaging::utils::tiffread(m_path);
    }
    else {
        m_value = cv::imread(m_path.string(), cv::IMREAD_UNCHANGED);
    }
}

void misaxx::imaging::misa_image_file_cache::stash() {
    m_value.release();
}

void misaxx::imaging::misa_image_file_cache::push() {
    if(m_path.has_extension() && (m_path.extension().string() == ".tif" || m_path.extension().string() == ".tiff")) {
        misaxx::imaging::utils::tiffwrite(m_value, m_path, utils::tiff_compression::lzw);
    }
    else {
        cv::imwrite(m_path.string(), m_value);
    }
}

void misaxx::imaging::misa_image_file_cache::do_link(const misaxx::imaging::misa_image_description &t_description) {
    if(t_description.filename.empty())
        throw std::runtime_error("Cannot link to file description with empty file name!");
    m_path = this->get_location() / t_description.filename;
    this->set_unique_location(m_path);
}

misaxx::imaging::misa_image_description
misaxx::imaging::misa_image_file_cache::produce_description(const boost::filesystem::path &t_location,
                                                            const misaxx::imaging::misa_image_pattern &t_pattern) {
    misa_image_description result;
    t_pattern.apply(result, t_location);
    return result;
}

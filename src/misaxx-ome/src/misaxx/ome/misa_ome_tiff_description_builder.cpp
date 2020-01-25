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

#include <misaxx/ome/misa_ome_tiff_description_builder.h>
#include <ome/files/PixelProperties.h>
#include "src/misaxx/ome/utils/ome_to_opencv.h"
#include "src/misaxx/ome/utils/opencv_to_ome.h"
#include <misaxx/core/runtime/misa_runtime_properties.h>

using namespace misaxx;
using namespace misaxx::ome;

misa_ome_tiff_description_builder::misa_ome_tiff_description_builder(misa_ome_tiff_description src) : m_result(std::move(src)) {
    if(static_cast<bool>(m_result.metadata)) {
        for(size_t series = 0; series < m_result.metadata->getImageCount(); ++series) {
            m_series_list.emplace_back(helpers::create_ome_core_metadata(*m_result.metadata, series));
        }
    }
    else {
        m_result.metadata = std::make_shared<::ome::xml::meta::OMEXMLMetadata>();
    }
}

misa_ome_tiff_description_builder::operator misa_ome_tiff_description() {
    ::ome::files::fillMetadata(*m_result.metadata, m_series_list);
    return m_result;
}

std::shared_ptr<misaxx::misa_description_storage> misa_ome_tiff_description_builder::as_storage() {
    return misaxx::misa_description_storage::with(static_cast<misa_ome_tiff_description>(*this));
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::change_series(size_t series) {
    m_series = series;

    while(m_series_list.size() < series + 1) {
        m_series_list.emplace_back(std::make_shared<::ome::files::CoreMetadata>());
    }

    return *this;
}

::ome::files::CoreMetadata &misa_ome_tiff_description_builder::core_metadata() {
    change_series(m_series);
    return *m_series_list.at(m_series);
}

misa_ome_tiff_description_builder &
misa_ome_tiff_description_builder::pixel_channel_type(const ::ome::xml::model::enums::PixelType &t_pixel_type) {
    core_metadata().pixelType = t_pixel_type;
    core_metadata().bitsPerPixel = ::ome::files::bitsPerPixel(t_pixel_type);
    return *this;
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::pixel_channels(std::vector<size_t> channels) {
    if(core_metadata().sizeC.empty())
        throw std::runtime_error("Please set the number of channels in the channel axis first!");
    if (channels.size() == 1) {
        for (auto &s : core_metadata().sizeC) {
            s = channels.at(0);
        }
    } else {
        if (channels.size() != core_metadata().sizeC.size())
            throw std::runtime_error(
                    "The size of the provided channel configuration should match the number of planes in channel axis! Please set the number of channels in the channel axis first!");
        core_metadata().sizeC = std::move(channels);
    }

    return *this;
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::pixel_channels(size_t channels) {
    std::vector<size_t> c = {channels};
    return pixel_channels(std::move(c));
}

misa_ome_tiff_description_builder &
misa_ome_tiff_description_builder::of(size_t channels, const ::ome::xml::model::enums::PixelType &t_pixel_type) {
    pixel_channels(channels);
    return pixel_channel_type(t_pixel_type);
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::of_opencv(int opencv_type) {
    cv::Mat m(1, 1, opencv_type);
    pixel_channels(m.channels());
    return pixel_channel_type(opencv_depth_to_ome_pixel_type(m.depth()));
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::of_opencv(const cv::Mat &t_mat) {
    pixel_channels(t_mat.channels());
    return pixel_channel_type(opencv_depth_to_ome_pixel_type(t_mat.depth()));
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::depth(size_t size) {
    core_metadata().sizeZ = size;
    return *this;
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::channels(size_t size, size_t num_channels) {
    if(core_metadata().sizeC.size() < size) {
        while(core_metadata().sizeC.size() < size + 1) {
            core_metadata().sizeC.push_back(num_channels);
        }
    }
    else if(core_metadata().sizeC.size() > size) {
        core_metadata().sizeC.resize(size); // Remove the last
    }
    return *this;
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::duration(size_t size) {
    core_metadata().sizeT = size;
    return *this;
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::width(size_t size) {
    core_metadata().sizeX = size;
    return *this;
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::height(size_t size) {
    core_metadata().sizeY = size;
    return *this;
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::of_size(size_t w, size_t h) {
    return width(w).height(h);
}

misa_ome_tiff_description_builder &misa_ome_tiff_description_builder::with_filename(std::string filename) {
    m_result.filename = std::move(filename);
    return *this;
}



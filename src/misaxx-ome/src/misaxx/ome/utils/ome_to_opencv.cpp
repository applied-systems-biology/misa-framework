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

#include "ome_to_opencv.h"
#include <ome/files/VariantPixelBuffer.h>

using namespace misaxx::ome;

namespace {
    /**
    * Converts a OME variant pixel buffer into a cv::Mat
    * @tparam RawType
    * @param ome_buffer
    * @param size_x
    * @param size_y
    * @param channels
    * @param opencv_type
    * @return
    */
    template<typename RawType> inline cv::Mat ome_to_opencv_detail(const ::ome::files::VariantPixelBuffer &ome_buffer, int size_x, int size_y, int channels, int opencv_type) {
        cv::Mat result(size_y, size_x, opencv_type, cv::Scalar::all(0));

        if(!result.isContinuous())
            throw std::runtime_error("cv::Mat must be continuous!");

        const auto &src_array = ome_buffer.array<RawType>();

        ::ome::files::PixelBufferBase::indices_type idx;
        std::fill(idx.begin(), idx.end(), 0);

        for(int y = 0; y < result.rows; ++y) {
            auto *ptr = result.ptr<RawType>(y);
            for(int x = 0; x < result.cols; ++x) {
                idx[::ome::files::DIM_SPATIAL_X] = x;
                idx[::ome::files::DIM_SPATIAL_Y] = y;
                for(int c = 0; c < channels; ++c) {
                    idx[::ome::files::DIM_SUBCHANNEL] = c;
                    ptr[x * channels + c] = src_array(idx);
                }
            }
        }
        return result;
    }
}

cv::Mat misaxx::ome::ome_to_opencv(const ::ome::files::FormatReader &ome_reader, const misa_ome_plane_description &index) {

    using namespace ::ome::xml::model::enums;

    int size_x = static_cast<int>(ome_reader.getSizeX());
    int size_y = static_cast<int>(ome_reader.getSizeY());
    int channels = static_cast<int>(ome_reader.getRGBChannelCount(index.c));

    ::ome::files::VariantPixelBuffer ome_buffer;
    ome_reader.openBytes(index.index_within(ome_reader), ome_buffer);

    switch(ome_buffer.pixelType()) {
        case PixelType::UINT8:
            return ome_to_opencv_detail<uchar>(ome_buffer, size_x, size_y, channels, CV_8UC(channels));
        case PixelType::INT8:
            return ome_to_opencv_detail<char>(ome_buffer, size_x, size_y, channels, CV_8SC(channels));
        case PixelType::UINT16:
            return ome_to_opencv_detail<ushort>(ome_buffer, size_x, size_y, channels, CV_16UC(channels));
        case PixelType::INT16:
            return ome_to_opencv_detail<short>(ome_buffer, size_x, size_y, channels, CV_16SC(channels));
        case PixelType::INT32:
            return ome_to_opencv_detail<int>(ome_buffer, size_x, size_y, channels, CV_32SC(channels));
        case PixelType::FLOAT:
            return ome_to_opencv_detail<float>(ome_buffer, size_x, size_y, channels, CV_32FC(channels));
        case PixelType::DOUBLE:
            return ome_to_opencv_detail<double>(ome_buffer, size_x, size_y, channels, CV_64FC(channels));
        case PixelType::UINT32:
        case PixelType::COMPLEXFLOAT:
        case PixelType::COMPLEXDOUBLE:
        case PixelType::BIT:
        default:
            throw std::runtime_error("OpenCV does not support this pixel type!");
    }
}

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
#include <ome/files/VariantPixelBuffer.h>
#include <ome/files/FormatWriter.h>
#include <ome/xml/model/primitives/Quantity.h>
#include <ome/xml/model/enums.h>
#include <ome/files/FormatReader.h>
#include <ome/files/out/OMETIFFWriter.h>
#include <ome/files/detail/OMETIFF.h>
#include <ome/files/tiff/TIFF.h>
#include <ome/files/tiff/IFD.h>
#include <ome/files/tiff/Tags.h>
#include <ome/files/tiff/Field.h>
#include <ome/files/MetadataTools.h>
#include <ome/files/CoreMetadata.h>
#include <opencv2/opencv.hpp>
#include <misaxx/ome/descriptions/misa_ome_plane_description.h>

namespace misaxx::ome {

    /**
     * Converts an OpenCV pixel depth to an OME pixel type
     * @param opencv_depth
     * @return
     */
    extern ::ome::xml::model::enums::PixelType opencv_depth_to_ome_pixel_type(int opencv_depth);

    /**
     * Creates OME Core metadata from an OpenCV image
     * @param opencv_image
     * @param num_images
     * @return
     */
    extern std::shared_ptr<::ome::files::CoreMetadata> opencv_to_ome_series_metadata(const cv::Mat &opencv_image, ::ome::files::dimension_size_type num_images = 1);

    /**
     * Creates OME XML metadata from an OpenCV image
     * @param opencv_image
     * @param num_images
     * @param num_series
     * @return
     */
    extern std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> opencv_to_ome_metadata(const cv::Mat &opencv_image, ::ome::files::dimension_size_type num_images = 1, ::ome::files::dimension_size_type num_series = 1);

    template<typename RawType, int OMEPixelType> inline void opencv_to_ome_detail(const cv::Mat &opencv_image, ::ome::files::out::OMETIFFWriter &ome_writer, const misa_ome_plane_description &index) {
        using namespace ::ome::files;
        using namespace ::ome::xml::model::enums;
        const int size_x = opencv_image.cols;
        const int size_y = opencv_image.rows;
        const int channels = opencv_image.channels();
        auto buffer = std::make_shared<PixelBuffer<typename PixelProperties<OMEPixelType>::std_type>> (boost::extents[size_x][size_y][1][1][1][channels][1][1][1],
                                                               opencv_depth_to_ome_pixel_type(opencv_image.depth()),
                                                               ::ome::files::ENDIAN_NATIVE,
                                                               PixelBufferBase::make_storage_order(DimensionOrder::XYZTC, false));
        PixelBufferBase::indices_type idx;
        std::fill(idx.begin(), idx.end(), 0);

        for(int y = 0; y < opencv_image.rows; ++y) {
            const auto *ptr = opencv_image.ptr<RawType>(y);
            for(int x = 0; x < opencv_image.cols; ++x) {
                idx[DIM_SPATIAL_X] = x;
                idx[DIM_SPATIAL_Y] = y;
                for(int c = 0; c < channels; ++c) {
                    idx[DIM_SUBCHANNEL] = c;
                    buffer->at(idx) = ptr[x * channels + c];
                }
            }
        }

        VariantPixelBuffer vbuffer(buffer);
        ome_writer.saveBytes(index.index_within(ome_writer), vbuffer);
    }

    extern void opencv_to_ome(const cv::Mat &opencv_image, ::ome::files::out::OMETIFFWriter &ome_writer, const misa_ome_plane_description &index);
}
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

#include "opencv_to_ome.h"

using namespace misaxx;
using namespace misaxx::ome;

::ome::xml::model::enums::PixelType misaxx::ome::opencv_depth_to_ome_pixel_type(int opencv_depth) {
    switch (opencv_depth) {
        case CV_8U:
            return ::ome::xml::model::enums::PixelType::UINT8;
        case CV_8S:
            return ::ome::xml::model::enums::PixelType::INT8;
        case CV_16U:
            return ::ome::xml::model::enums::PixelType::UINT16;
        case CV_16S:
            return ::ome::xml::model::enums::PixelType::INT16;
        case CV_32S:
            return ::ome::xml::model::enums::PixelType::INT32;
        case CV_32F:
            return ::ome::xml::model::enums::PixelType::FLOAT;
        case CV_64F:
            return ::ome::xml::model::enums::PixelType::DOUBLE;
        default:
            throw std::runtime_error("Unsupported OpenCV pixel depth!");
    }
}

std::shared_ptr<::ome::files::CoreMetadata>
misaxx::ome::opencv_to_ome_series_metadata(const cv::Mat &opencv_image, ::ome::files::dimension_size_type num_images) {
    std::shared_ptr<::ome::files::CoreMetadata> core(std::make_shared<::ome::files::CoreMetadata>());
    core->sizeX = opencv_image.cols;
    core->sizeY = opencv_image.rows;
    core->sizeC.clear(); // defaults to 1 channel with 1 subchannel; clear this
    core->sizeC.push_back(opencv_image.channels());
    core->sizeZ = num_images;
    core->sizeT = 1;
    core->interleaved = false;
    core->dimensionOrder = ::ome::xml::model::enums::DimensionOrder::XYZTC;
    core->pixelType = opencv_depth_to_ome_pixel_type(opencv_image.depth());
    core->bitsPerPixel = ::ome::files::bitsPerPixel(core->pixelType);
    return core;
}

std::shared_ptr<::ome::xml::meta::OMEXMLMetadata>
misaxx::ome::opencv_to_ome_metadata(const cv::Mat &opencv_image, ::ome::files::dimension_size_type num_images,
                       ::ome::files::dimension_size_type num_series) {
    /* create-metadata-start */
    // OME-XML metadata store.
    auto meta = std::make_shared<::ome::xml::meta::OMEXMLMetadata>();

    // Create simple CoreMetadata and use this to set up the OME-XML
    // metadata.  This is purely for convenience in this example; a
    // real writer would typically set up the OME-XML metadata from an
    // existing MetadataRetrieve instance or by hand.
    std::vector<std::shared_ptr<::ome::files::CoreMetadata>> seriesList;

    for(::ome::files::dimension_size_type i = 0; i < num_series; ++i) {
        seriesList.push_back(opencv_to_ome_series_metadata(opencv_image, num_images));
    }

    ::ome::files::fillMetadata(*meta, seriesList);
    /* create-metadata-end */

    return meta;
}

void misaxx::ome::opencv_to_ome(const cv::Mat &opencv_image, ::ome::files::out::OMETIFFWriter &ome_writer,
                   const misa_ome_plane_description &index) {

    using namespace ::ome::xml::model::enums;

    switch (opencv_image.depth()) {
        case CV_8U: {
            opencv_to_ome_detail<uchar, PixelType::UINT8>(opencv_image, ome_writer, index);
        }
            break;
        case CV_8S: {
            opencv_to_ome_detail<char, PixelType::INT8>(opencv_image, ome_writer, index);
        }
            break;
        case CV_16U: {
            opencv_to_ome_detail<ushort, PixelType::UINT16>(opencv_image, ome_writer, index);
        }
            break;
        case CV_16S: {
            opencv_to_ome_detail<short, PixelType::INT16 >(opencv_image, ome_writer, index);
        }
            break;
        case CV_32S: {
            opencv_to_ome_detail<int, PixelType::INT32>(opencv_image, ome_writer, index);
        }
            break;
        case CV_32F: {
            opencv_to_ome_detail<float, PixelType::FLOAT>(opencv_image, ome_writer, index);
        }
            break;
        case CV_64F: {
            opencv_to_ome_detail<double, PixelType::DOUBLE >(opencv_image, ome_writer, index);
        }
            break;
        default:
            throw std::runtime_error("Unsupported OpenCV pixel depth!");
    }
}

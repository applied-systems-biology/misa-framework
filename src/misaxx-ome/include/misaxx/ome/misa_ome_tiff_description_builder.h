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

#include <memory>
#include <misaxx/ome/descriptions/misa_ome_tiff_description.h>
#include <opencv2/opencv.hpp>
#include <misaxx/core/misa_description_storage.h>
#include <misaxx/ome/utils/ome_helpers.h>

namespace misaxx::ome {

    /**
     * Builder that allows easy creation of OME XML Metadata from scratch.
     */
    struct misa_ome_tiff_description_builder {

        misa_ome_tiff_description_builder() = default;

        explicit misa_ome_tiff_description_builder(misa_ome_tiff_description src);

        /**
         * Returns the final TIFF description
         * @return
         */
        operator misa_ome_tiff_description();

        /**
         * Returns the final TIFF description as storage
         * @return
         */
        std::shared_ptr<misaxx::misa_description_storage> as_storage();

        /**
         * All consecutive edit operations run on the provided series.
         * Will create series if necessary
         * @param series
         * @return
         */
        misa_ome_tiff_description_builder &change_series(size_t series);

        ::ome::files::CoreMetadata &core_metadata();

        /**
         * Sets the type each channel in a plane has
         * @param t_pixel_type
         * @return
         */
        misa_ome_tiff_description_builder &pixel_channel_type(const ::ome::xml::model::enums::PixelType &t_pixel_type);

        /**
         * Sets the number of channels each pixels has
         * @param channels
         * @return
         */
        misa_ome_tiff_description_builder &pixel_channels(std::vector<size_t> channels);

        /**
         * Sets the number of channels each pixels has
         * Requires an already set number of planes in channel axis
         * @param channels
         * @return
         */
        misa_ome_tiff_description_builder &pixel_channels(size_t channels);

        /**
         * Sets the number of channels and the pixel type
         * @param channels
         * @param t_pixel_type
         * @return
         */
        misa_ome_tiff_description_builder &of(size_t channels, const ::ome::xml::model::enums::PixelType &t_pixel_type);

        /**
         * Sets the number of channels and the pixel type from an OpenCV type
         * @param opencv_type
         * @return
         */
        misa_ome_tiff_description_builder &of_opencv(int opencv_type);

        /**
         * Initializes width, height, channels and pixel type from OpenCV
         * @param t_mat
         * @return
         */
        misa_ome_tiff_description_builder &of_opencv(const cv::Mat &t_mat);

        /**
         * Sets the number of planes in the Z (depth) axis
         * @param size
         * @return
         */
        misa_ome_tiff_description_builder &depth(size_t size);

        /**
         * Sets the number of planes in the channel axis
         * This is not the number of channels each pixel has
         * @param size
         * @return
         */
        misa_ome_tiff_description_builder &channels(size_t size, size_t num_channels = 1);

        /**
         * Sets the number of planes in the time axis
         * @param size
         * @return
         */
        misa_ome_tiff_description_builder &duration(size_t size);

        /**
         * Sets the width of the planes
         * @param size
         * @return
         */
        misa_ome_tiff_description_builder &width(size_t size);

        /**
         * Sets the height of the planes
         * @param size
         * @return
         */
        misa_ome_tiff_description_builder &height(size_t size);

        /**
         * Sets the width and height
         * @param w
         * @param h
         * @return
         */
        misa_ome_tiff_description_builder &of_size(size_t w, size_t h);

        /**
         * Sets the filename
         * @param filename
         * @return
         */
        misa_ome_tiff_description_builder &with_filename(std::string filename);

        /**
         * Modifies the description with a custom function
         * @tparam Function
         * @param t_function Function that takes a std::vector<std::shared_ptr<::ome::files::CoreMetadata>>
         * @return
         */
        template<class Function>
        misa_ome_tiff_description_builder &modify(const Function &t_function) {
            t_function(m_series_list);
            return *this;
        }

    private:
        misa_ome_tiff_description m_result;
        /**
         * The list of series that is managed by this description builder
         */
        std::vector<std::shared_ptr<::ome::files::CoreMetadata>> m_series_list;
        /**
         * The current series
         */
        size_t m_series = 0;
    };

}
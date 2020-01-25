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

#include <ome/xml/meta/OMEXMLMetadata.h>
#include <misaxx/ome/descriptions/misa_ome_tiff_description.h>
#include <misaxx/core/misa_description_storage.h>
#include <misaxx/ome/utils/ome_helpers.h>
#include <opencv2/opencv.hpp>

namespace misaxx::ome {

    /**
     * Builder-like helper that allows creation of a new misa_ome_tiff_description based on
     * an existing one. Additional metadata is kept.
     */
    struct misa_ome_tiff_description_modifier {

        misa_ome_tiff_description_modifier() = default;

        explicit misa_ome_tiff_description_modifier(misa_ome_tiff_description src);

        /**
         * Returns the final TIFF description
         * @return
         */
        operator misa_ome_tiff_description();

        /**
         * Returns the final TIFF description as storage
         * @return
         */
        operator std::shared_ptr<misaxx::misa_description_storage>();

        /**
       * All consecutive edit operations run on the provided series.
       * Will create series if necessary
       * @param series
       * @return
       */
        misa_ome_tiff_description_modifier &change_series(size_t series);

        /**
        * Sets the type each channel in a plane has
        * @param t_pixel_type
        * @return
        */
        misa_ome_tiff_description_modifier &pixel_channel_type(const ::ome::xml::model::enums::PixelType &t_pixel_type);

        /**
         * Sets the number of channels each pixels has
         * @param channels
         * @return
         */
        misa_ome_tiff_description_modifier &pixel_channels(std::vector<size_t> channels);

        /**
         * Sets the number of channels each pixels has
         * Requires an already set number of planes in channel axis
         * @param channels
         * @return
         */
        misa_ome_tiff_description_modifier &pixel_channels(size_t channels);

        /**
         * Sets the number of channels and the pixel type
         * @param channels
         * @param t_pixel_type
         * @return
         */
        misa_ome_tiff_description_modifier &of(size_t channels, const ::ome::xml::model::enums::PixelType &t_pixel_type);

        /**
         * Sets the number of channels and the pixel type from an OpenCV type
         * @param opencv_type
         * @return
         */
        misa_ome_tiff_description_modifier &of_opencv(int opencv_type);

        /**
         * Sets the number of planes in the Z (depth) axis
         * @param size
         * @return
         */
        misa_ome_tiff_description_modifier &depth(size_t size);

        /**
         * Sets the number of planes in the channel axis
         * This is not the number of channels each pixel has
         * @param size
         * @return
         */
        misa_ome_tiff_description_modifier &channels(size_t size, size_t num_channels = 1);

        /**
         * Sets the number of planes in the time axis
         * @param size
         * @return
         */
        misa_ome_tiff_description_modifier &duration(size_t size);

        /**
         * Sets the width of the planes
         * @param size
         * @return
         */
        misa_ome_tiff_description_modifier &width(size_t size);

        /**
         * Sets the height of the planes
         * @param size
         * @return
         */
        misa_ome_tiff_description_modifier &height(size_t size);

        /**
         * Sets the width and height
         * @param w
         * @param h
         * @return
         */
        misa_ome_tiff_description_modifier &of_size(size_t w, size_t h);

        /**
         * Sets the filename
         * @param filename
         * @return
         */
        misa_ome_tiff_description_modifier &with_filename(std::string filename);

        /**
         * Modifies the description with a custom function
         * @tparam Function
         * @param t_function Function that takes a ::ome::xml::meta::OMEXMLMetadata
         * @return
         */
        template<class Function>
        misa_ome_tiff_description_modifier &modify(const Function &t_function) {
            t_function(*m_result.metadata);
            return *this;
        }

    private:

        size_t m_series = 0;
        std::vector<std::vector<size_t>> m_channels;
        misa_ome_tiff_description m_result;

        /**
         * Writes the channel configuration into the metadata
         */
        void update_channels() {
            for(size_t series = 0; series < m_channels.size(); ++series) {
                const auto &channels = m_channels[series];
                size_t sizeC = std::accumulate(channels.begin(), channels.end(), size_t(0));

                m_result.metadata->setPixelsSizeC(sizeC, series);

                const size_t effSizeC = channels.size();

                for (size_t c = 0; c < effSizeC; ++c)
                {
                    size_t rgbC = channels.at(c);

                    m_result.metadata->setChannelID(::ome::files::createID("Channel", series, c), series, c);
                    m_result.metadata->setChannelSamplesPerPixel(static_cast<int>(rgbC), series, c);
                }
            }
        }
    };
}





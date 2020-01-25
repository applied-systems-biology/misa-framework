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
#include <shared_mutex>
#include <unordered_set>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <misaxx/core/misa_cache.h>
#include <boost/regex.hpp>
#include <ome/xml/meta/OMEXMLMetadata.h>
#include <ome/files/Types.h>

namespace misaxx::ome {

    // Forward declare
    struct misa_ome_plane_description;

    struct ome_tiff_io_impl;

    /**
     * Allows thread-safe read and write access to an OME TIFF
     * This wrapper will automatically switch between an OME TIFF reader and an OME TIFF writer depending on what functionality
     * is currently being requested.
     *
     * Please note that this IO, similar to ome::files TIFF reader & writer needs to be closed manually
     */
    class ome_tiff_io {
    public:

        ome_tiff_io();

        /**
         * Opens an existing OME TIFF file
         * @param t_path
         */
        explicit ome_tiff_io(boost::filesystem::path t_path);

        /**
         *  Opens an existing OME TIFF file or creates a new one based on the metadata
         *  If the file already exists, the metadata is loaded from the file instead.
         * @param t_path
         * @param t_metadata
         */
        explicit ome_tiff_io(boost::filesystem::path t_path,
                                     std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> t_metadata);

        /**
         * Opens an existing OME TIFF file or creates a new one based on the reference
         * @param t_path
         * @param t_reference
         */
        explicit ome_tiff_io(boost::filesystem::path t_path, const ome_tiff_io &t_reference);

        ~ome_tiff_io();

        void write_plane(const cv::Mat &image, const misa_ome_plane_description &index);

        cv::Mat read_plane(const misa_ome_plane_description &index) const;

        /**
         * Thread-safe access to the metadata
         * @return
         */
        std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> get_metadata() const;

        boost::filesystem::path get_path() const;

        /**
         * Closes any open reader and writer. This method is thread-safe.
         */
        void close(bool remove_write_buffer = true);

        /**
         * The number of image series
         * @return
         */
        ::ome::files::dimension_size_type get_num_series() const;

        /**
         * The width of each plane
         * @param series
         * @return
         */
        ::ome::files::dimension_size_type get_size_x(::ome::files::dimension_size_type series) const;

        /**
         * The height of each plane
         * @param series
         * @return
         */
        ::ome::files::dimension_size_type get_size_y(::ome::files::dimension_size_type series) const;

        /**
         * Planes located in depth axis
         * @param series
         * @return
         */
        ::ome::files::dimension_size_type get_size_z(::ome::files::dimension_size_type series) const;

        /**
         * Planes located in time axis
         * @param series
         * @return
         */
        ::ome::files::dimension_size_type get_size_t(::ome::files::dimension_size_type series) const;

        /**
         * Planes located in channel axis (this is the same as OME's effectiveSizeC)
         * @param series
         * @return
         */
        ::ome::files::dimension_size_type get_size_c(::ome::files::dimension_size_type series) const;

        /**
         * Number of planes (Z * C * T)
         * @param series
         * @return
         */
        ::ome::files::dimension_size_type get_num_planes(::ome::files::dimension_size_type series) const;

        bool compression_is_enabled() const;

        void set_compression(bool enabled);

    private:

        ome_tiff_io_impl *m_pimpl;

    };
}

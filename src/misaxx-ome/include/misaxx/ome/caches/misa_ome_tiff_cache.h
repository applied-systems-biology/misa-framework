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

#include <misaxx/core/misa_cache.h>
#include <misaxx/core/misa_default_cache.h>
#include <misaxx/ome/patterns/misa_ome_tiff_pattern.h>
#include <misaxx/ome/descriptions/misa_ome_tiff_description.h>
#include <misaxx/core/misa_cached_data.h>
#include <misaxx/ome/accessors/misa_ome_plane.h>
#include <misaxx/core/misa_parameter.h>

namespace misaxx::ome {

    struct ome_tiff_io;

    /**
     * Cache that allows read and write access to an OME TIFF
     */
    class misa_ome_tiff_cache : public misaxx::misa_default_cache<misaxx::utils::memory_cache<std::vector<misa_ome_plane>>,
            misa_ome_tiff_pattern, misa_ome_tiff_description> {
    public:

        misa_ome_tiff_cache();

        void simulate_link() override;

        void do_link(const misa_ome_tiff_description &t_description) override;

        bool has() const override;

        /**
         * Returns the ome::files tiff reader instance.
         * @return
         */
        std::shared_ptr<ome_tiff_io> get_tiff_io() const;

        /**
         * Returns the plane cache from a plane location
         * @param t_cache
         * @param t_location
         * @return
         */
        misa_ome_plane get_plane(const misa_ome_plane_description &t_location) const;

        void postprocess() override;

    protected:
        std::shared_ptr<misa_location> create_location_interface() const override;

    protected:
        misa_ome_tiff_description produce_description(const boost::filesystem::path &t_location,
                                                      const misa_ome_tiff_pattern &t_pattern) override;

    private:

        std::shared_ptr<ome_tiff_io> m_tiff;

        misaxx::misa_parameter<bool> m_remove_write_buffer_parameter;
        misaxx::misa_parameter<bool> m_disable_ome_tiff_writing_parameter;
        misaxx::misa_parameter<bool> m_enable_compression_parameter;

    };
}
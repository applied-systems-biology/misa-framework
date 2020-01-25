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

#include <misaxx/ome/caches/misa_ome_tiff_cache.h>
#include <misaxx/ome/attachments/misa_ome_planes_location.h>
#include <misaxx/core/runtime/misa_parameter_registry.h>
#include <src/misaxx/ome/utils/ome_tiff_io.h>
#include <misaxx/core/misa_parameter.h>
#include <ome/common/log.h>
#include <misaxx/core/utils/filesystem.h>

misaxx::ome::misa_ome_tiff_cache::misa_ome_tiff_cache() {
    m_remove_write_buffer_parameter = misaxx::misa_parameter<bool> { {"runtime", "misaxx-ome", "remove-write-buffer"} };
    m_remove_write_buffer_parameter.schema->document_title("Remove OME TIFF write buffer")
            .document_description("If true, the OME TIFF write buffer is removed during postprocessing")
            .declare_optional(true);

    m_disable_ome_tiff_writing_parameter = misaxx::misa_parameter<bool> { {"runtime", "misaxx-ome", "disable-write-buffer-to-ome-tiff"} };
    m_disable_ome_tiff_writing_parameter.schema->document_title("Disable OME TIFF writing")
            .document_description("If true, the write buffer will not be postprocessed into a proper OME TIFF")
            .declare_optional(false);

    m_enable_compression_parameter = misaxx::misa_parameter<bool> { {"runtime", "misaxx-ome", "enable-compression"} };
    m_enable_compression_parameter.schema->document_title("Enable compression of output images")
            .document_description("If true, output data is compressed with LZW")
            .declare_optional(true);
}

void misaxx::ome::misa_ome_tiff_cache::do_link(const misaxx::ome::misa_ome_tiff_description &t_description) {

    // Set the OME log level (needed for Windows)
    ::ome::common::setLogLevel(::ome::logging::trivial::warning);

    if (t_description.filename.empty())
        throw std::runtime_error("Cannot link to file description with empty file name!");

    // We do cache initialization during linkage
    this->set_unique_location(this->get_location() / t_description.filename);

    // OME TIFF is very sensitive about file paths
    // Convert to preferred representation
    this->set_unique_location(misaxx::utils::make_preferred(this->get_unique_location()));

    if (boost::filesystem::exists(this->get_unique_location())) {
        std::cout << "[Cache] Opening OME TIFF " << this->get_unique_location() << "\n";
        m_tiff = std::make_shared<ome_tiff_io>(this->get_unique_location());

        // Put the loaded metadata into the description
        this->describe()->template get<misa_ome_tiff_description>().metadata = m_tiff->get_metadata();
    } else {
        std::cout << "[Cache] Creating OME TIFF " << this->get_unique_location() << "\n";

        // Create the TIFF and generate the image caches
        m_tiff = std::make_shared<ome_tiff_io>(this->get_unique_location(), t_description.metadata);
    }

    // Enable compression if needed
    m_tiff->set_compression(m_enable_compression_parameter.query());

    // Create the plane caches
    for (size_t series = 0; series < m_tiff->get_num_series(); ++series) {
        const auto size_Z = m_tiff->get_size_z(series);
        const auto size_C = m_tiff->get_size_c(series);
        const auto size_T = m_tiff->get_size_t(series);

        for (size_t z = 0; z < size_Z; ++z) {
            for (size_t c = 0; c < size_C; ++c) {
                for (size_t t = 0; t < size_T; ++t) {
                    misa_ome_plane cache;
                    cache.data = std::make_shared<misa_ome_plane_cache>();
                    cache.data->set_tiff_io(m_tiff);
                    cache.force_link(this->get_internal_location(),
                            this->get_location(), misaxx::misa_description_storage::with(
                            misa_ome_plane_description(series, z, c, t)));
                    this->get().emplace_back(std::move(cache));
                }
            }
        }
    }
}

bool misaxx::ome::misa_ome_tiff_cache::has() const {
    return static_cast<bool>(m_tiff);
}

std::shared_ptr<misaxx::ome::ome_tiff_io> misaxx::ome::misa_ome_tiff_cache::get_tiff_io() const {
    return m_tiff;
}

misaxx::ome::misa_ome_plane
misaxx::ome::misa_ome_tiff_cache::get_plane(const misaxx::ome::misa_ome_plane_description &t_location) const {
//            const auto num_series = m_tiff->get_num_series();
//            const auto size_Z = m_tiff->get_size_z(t_location.series);
    const auto size_C = m_tiff->get_size_c(t_location.series);
    const auto size_T = m_tiff->get_size_t(t_location.series);

    // Calculate the plane index
    size_t start_index = 0;
    for (size_t series = 0; series < t_location.series; ++series) {
        start_index += m_tiff->get_num_planes(series);
    }

    size_t index = start_index + t_location.t + t_location.c * size_T + t_location.z * size_T * size_C;
    return this->get().at(index);
}

void misaxx::ome::misa_ome_tiff_cache::postprocess() {
    misaxx::misa_default_cache<misaxx::utils::memory_cache<std::vector<misa_ome_plane>>,
            misa_ome_tiff_pattern, misa_ome_tiff_description>::postprocess();
    if (m_disable_ome_tiff_writing_parameter.query()) {
        std::cout << "[WARNING] No OME TIFF is written, because it is disabled by a parameter!" << "\n";
        return;
    }

    // Close the TIFF
    m_tiff->close(m_remove_write_buffer_parameter.query());
}

misaxx::ome::misa_ome_tiff_description
misaxx::ome::misa_ome_tiff_cache::produce_description(const boost::filesystem::path &t_location,
                                                      const misaxx::ome::misa_ome_tiff_pattern &t_pattern) {
    misa_ome_tiff_description result;
    t_pattern.apply(result, t_location);
    return result;
}

std::shared_ptr<misaxx::misa_location> misaxx::ome::misa_ome_tiff_cache::create_location_interface() const {
    auto result = std::make_shared<misaxx::ome::misa_ome_planes_location>();
    result->internal_location = get_internal_location();
    result->filesystem_location = get_location();
    result->filesystem_unique_location = get_unique_location();

    for (const auto &plane : this->get()) {
        result->planes.push_back(plane.get_plane_location());
    }

    return result;
}

void misaxx::ome::misa_ome_tiff_cache::simulate_link() {
    misa_default_cache::simulate_link();
}



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

#include <src/misaxx/ome/utils/ome_tiff_io.h>
#include <misaxx/ome/descriptions/misa_ome_plane_description.h>
#include <misaxx/core/utils/string.h>
#include <misaxx/imaging/utils/tiffio.h>
#include <ome/files/out/OMETIFFWriter.h>
#include <ome/files/in/OMETIFFReader.h>
#include <ome/xml/meta/Convert.h>
#include <misaxx/ome/utils/ome_helpers.h>
#include <ome/files/MetadataTools.h>
#include <opencv2/opencv.hpp>
#include "ome_to_opencv.h"
#include "opencv_to_ome.h"
#include "ome_to_ome.h"

namespace {
    /**
    * Exposes the internal OME XML, as the metadata maps do not contain all information for some reason
    */
    struct custom_ome_tiff_reader : public ::ome::files::in::OMETIFFReader {

        std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> get_xml_metadata() const {
            auto meta = cacheMetadata(*currentId); // Try the cached metadata of the current file
            boost::filesystem::path first_tiff_path = meta->getUUIDFileName(0, 0);
            first_tiff_path = boost::filesystem::canonical(first_tiff_path, currentId.value().parent_path());
            if(currentId != first_tiff_path) {
                meta = ::ome::files::createOMEXMLMetadata(first_tiff_path);
            }
            return meta;
        }
    };
}

namespace misaxx::ome {
    
    struct ome_tiff_io_impl {        
    public:

        using tiff_reader_type = std::shared_ptr<::ome::files::in::OMETIFFReader>;
        using tiff_writer_type = std::shared_ptr<::ome::files::out::OMETIFFWriter>;

        ome_tiff_io_impl() = default;

        /**
         * Opens an existing OME TIFF file
         * @param t_path
         */
        explicit ome_tiff_io_impl(boost::filesystem::path t_path);

        /**
         *  Opens an existing OME TIFF file or creates a new one based on the metadata
         *  If the file already exists, the metadata is loaded from the file instead.
         * @param t_path
         * @param t_metadata
         */
        explicit ome_tiff_io_impl(boost::filesystem::path t_path,
        std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> t_metadata);

        /**
         * Opens an existing OME TIFF file or creates a new one based on the reference
         * @param t_path
         * @param t_reference
         */
        explicit ome_tiff_io_impl(boost::filesystem::path t_path, const ome_tiff_io &t_reference);

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
        bool m_enable_compression = false;

        /**
         * Path of the TIFF that is read / written
         */
        mutable boost::filesystem::path m_path;

        /**
         * Because of limitations to OMETIFFWriter, we buffer any output TIFF in a separate directory
         */
        mutable std::map<misa_ome_plane_description, boost::filesystem::path> m_write_buffer;

        mutable std::shared_ptr<custom_ome_tiff_reader> m_reader;
        mutable std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> m_metadata;
        mutable std::shared_mutex m_mutex;

        void open_reader() const;

        void close_reader() const;

        void close_writer(bool remove_write_buffer) const;

        /**
         * Returns the write buffer path for a location
         * @param t_location
         * @return
         */
        boost::filesystem::path get_write_buffer_path(const misa_ome_plane_description &t_location) const;

        /**
        * Thread-safe access to the managed reader
        * If applicable, returns a reader to a plane in the write buffer
        * @return
        */
        tiff_reader_type get_reader(const misa_ome_plane_description &t_location) const;
    };
}

using namespace misaxx::ome;


ome_tiff_io_impl::ome_tiff_io_impl(boost::filesystem::path t_path) : m_path(std::move(t_path)) {
    if (!boost::filesystem::exists(m_path)) {
        throw std::runtime_error("Cannot read from non-existing file " + m_path.string());
    }
}

ome_tiff_io_impl::ome_tiff_io_impl(boost::filesystem::path t_path,
                                     std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> t_metadata) : m_path(
        std::move(t_path)), m_metadata(std::move(t_metadata)) {
    // We can load metadata from the file if it exists
    if (boost::filesystem::exists(m_path)) {
        m_metadata.reset();
    }
}

ome_tiff_io_impl::ome_tiff_io_impl(boost::filesystem::path t_path, const ome_tiff_io &t_reference)
        : ome_tiff_io_impl(std::move(t_path), t_reference.get_metadata()) {
}

ome_tiff_io_impl::tiff_reader_type
ome_tiff_io_impl::get_reader(const misa_ome_plane_description &t_location) const {
    if(!static_cast<bool>(m_reader)) {
        if(!m_write_buffer.empty()) {
            close_writer(true);
        }
        open_reader();
    }
    return m_reader;
}

boost::filesystem::path
ome_tiff_io_impl::get_write_buffer_path(const misa_ome_plane_description &t_location) const {
    return m_path.parent_path() / "__misa_ome_write_buffer__" / (m_path.filename().string() + "_" + misaxx::utils::to_string(t_location) + ".ome.tif");
}

void ome_tiff_io_impl::close_writer(bool remove_write_buffer) const {
    std::cout << "[MISA++ OME] Writing results as OME TIFF " << m_path << " ... " << "\n";
    // Save the write buffer files into the path
    auto writer = std::make_shared<::ome::files::out::OMETIFFWriter>();
    auto metadata = std::static_pointer_cast<::ome::xml::meta::MetadataRetrieve>(m_metadata);
    writer->setMetadataRetrieve(metadata);
    writer->setBigTIFF(true);
    writer->setId(m_path);
    const auto compression_types = writer->getCompressionTypes();
    if(compression_is_enabled() && compression_types.find("LZW") != compression_types.end()) {
        writer->setCompression("LZW");
    }

    for(const auto &kv : m_write_buffer) {
        std::cout << "[MISA++ OME] Writing results as OME TIFF " << m_path << " ... " << kv.first << "\n";
        cv::Mat tmp = misaxx::imaging::utils::tiffread(kv.second);
        opencv_to_ome(tmp, *writer, kv.first);

        // Remove write buffer if requested
        if(remove_write_buffer) {
            boost::filesystem::remove(kv.second);
        }
    }

    writer->close();
    m_write_buffer.clear();
}

void ome_tiff_io_impl::close_reader() const {
    m_reader->close();
    m_reader.reset();
}

void ome_tiff_io_impl::open_reader() const {
    m_reader = std::make_shared<custom_ome_tiff_reader>();
    m_reader->setMetadataFiltered(false);
    m_reader->setGroupFiles(true);
    m_reader->setId(m_path);

    // INFO: This would be the proper way of loading the metadata, but it does not work
    // For example PhysicalSize properties are missing
//            if(!static_cast<bool>(m_metadata)) {
//               m_metadata = ::ome::files::createOMEXMLMetadata(*m_reader);
//               ::ome::files::fillOriginalMetadata(*m_metadata, m_reader->getGlobalMetadata());
//
//               for(size_t series = 0; series < m_reader->getSeriesCount(); ++series) {
//                   m_reader->setSeries(series);
//                   ::ome::files::fillOriginalMetadata(*m_metadata, m_reader->getSeriesMetadata());
//               }
//            }

    if(!static_cast<bool>(m_metadata)) {
        m_metadata = m_reader->get_xml_metadata(); // Copy the XML data to be sure
    }
}

void ome_tiff_io_impl::close(bool remove_write_buffer) {
    std::unique_lock<std::shared_mutex> lock(m_mutex, std::defer_lock);
    lock.lock();
    if(static_cast<bool>(m_reader)) {
        close_reader();
    }
    if(!m_write_buffer.empty()) {
        close_writer(remove_write_buffer);
    }
}

std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> ome_tiff_io_impl::get_metadata() const {
    if(static_cast<bool>(m_metadata)) {
        return m_metadata;
    }
    else {
        // We are currently reading a file. Open it and fetch the metadata
        if(!m_write_buffer.empty())
            throw std::logic_error("Write buffer is active, but no metadata is set!");
        std::cout << "[MISA++ OME] Locking " << m_path << " to obtain OME XML metadata" << "\n";
        std::unique_lock<std::shared_mutex> lock { m_mutex, std::defer_lock };
        lock.lock();
        std::cout << "[MISA++ OME] Locking " << m_path << " to obtain OME XML metadata ... successful" << "\n";
        get_reader(misa_ome_plane_description(0, 0, 0, 0));
        return m_metadata;
    }
}

cv::Mat ome_tiff_io_impl::read_plane(const misa_ome_plane_description &index) const {
    if(index.series != 0)
        throw std::runtime_error("Only series 0 is currently supported!");

//    std::cout << "[MISA++ OME] Soft locking " << m_path << " to read data" << "\n";
    std::shared_lock<std::shared_mutex> lock { m_mutex, std::defer_lock };
    lock.lock();
//    std::cout << "[MISA++ OME] Soft locking " << m_path << " to read data .. successful" << "\n";

    if(m_write_buffer.find(index) == m_write_buffer.end()) {

        lock.unlock();
//        std::cout << "[MISA++ OME] Locking " << m_path << " to read data from OME TIFF" << "\n";
        std::unique_lock<std::shared_mutex> wlock { m_mutex, std::defer_lock };
        wlock.lock();
//        std::cout << "[MISA++ OME] Locking " << m_path << " to read data from OME TIFF .. successful" << "\n";

        return ome_to_opencv(*get_reader(index), index);
    } else {
        // The write buffer contains only standard TIFFs
        return misaxx::imaging::utils::tiffread(m_write_buffer.at(index));
    }
}

void ome_tiff_io_impl::write_plane(const cv::Mat &image, const misa_ome_plane_description &index) {
    // Lock this IO to allow writing to the write buffer
//    std::cout << "[MISA++ OME] Locking " << m_path << " to write data" << "\n";
    std::unique_lock<std::shared_mutex> lock { m_mutex, std::defer_lock };
    lock.lock();
//    std::cout << "[MISA++ OME] Locking " << m_path << " to write data ... successful" << "\n";

    if(index.series != 0)
        throw std::runtime_error("Only series 0 is currently supported!");

    // If the file already exists, we have to create a write buffer
    if(m_write_buffer.empty() && boost::filesystem::exists(m_path)) {
        std::cout << "[MISA++ OME] Preparing write mode for existing OME TIFF " << m_path << " ... " << "\n";
        for(size_t series = 0; series < get_num_series(); ++series) {
            m_reader->setSeries(series);
            const auto size_Z = m_reader->getSizeZ();
            const auto size_C = m_reader->getEffectiveSizeC();
            const auto size_T = m_reader->getSizeT();

            for(size_t z = 0; z < size_Z; ++z) {
                for(size_t c = 0; c < size_C; ++c) {
                    for (size_t t = 0; t < size_T; ++t) {
                        const misa_ome_plane_description location(series, z, c, t);
                        const auto location_name = misaxx::utils::to_string(location);
                        std::cout << "[MISA++ OME] Preparing write mode for existing OME TIFF " << m_path << " ... writing plane " << location_name << "\n";

                        const boost::filesystem::path buffer_path = get_write_buffer_path(location);
                        if(!boost::filesystem::is_directory(buffer_path.parent_path())) {
                            boost::filesystem::create_directories(buffer_path.parent_path());
                        }

                        cv::Mat tmp = ome_to_opencv(*m_reader, misa_ome_plane_description(series, z, c, t));
                        misaxx::imaging::utils::tiffwrite(tmp, buffer_path);
                    }
                }
            }
        }
    }

    const boost::filesystem::path buffer_path = get_write_buffer_path(index);
    if(!boost::filesystem::is_directory(buffer_path.parent_path())) {
        boost::filesystem::create_directories(buffer_path.parent_path());
    }
    misaxx::imaging::utils::tiff_compression compression;
    if(compression_is_enabled())
        compression = misaxx::imaging::utils::tiff_compression::lzw;
    else
        compression = misaxx::imaging::utils::tiff_compression::none;
    misaxx::imaging::utils::tiffwrite(image, buffer_path, compression);
    m_write_buffer[index] = buffer_path;
}

::ome::files::dimension_size_type ome_tiff_io_impl::get_num_series() const {
    return get_metadata()->getImageCount();
}

::ome::files::dimension_size_type ome_tiff_io_impl::get_size_x(::ome::files::dimension_size_type series) const {
    return get_metadata()->getPixelsSizeX(series);
}

::ome::files::dimension_size_type ome_tiff_io_impl::get_size_y(::ome::files::dimension_size_type series) const {
    return get_metadata()->getPixelsSizeY(series);
}

::ome::files::dimension_size_type ome_tiff_io_impl::get_size_z(::ome::files::dimension_size_type series) const {
    return get_metadata()->getPixelsSizeZ(series);
}

::ome::files::dimension_size_type ome_tiff_io_impl::get_size_t(::ome::files::dimension_size_type series) const {
    return get_metadata()->getPixelsSizeT(series);
}

::ome::files::dimension_size_type ome_tiff_io_impl::get_size_c(::ome::files::dimension_size_type series) const {
    return get_metadata()->getChannelCount(series);
}

::ome::files::dimension_size_type
ome_tiff_io_impl::get_num_planes(::ome::files::dimension_size_type series) const {
    return get_size_c(series) * get_size_t(series) * get_size_z(series);
}

boost::filesystem::path ome_tiff_io_impl::get_path() const {
    return m_path;
}

bool ome_tiff_io_impl::compression_is_enabled() const {
    return m_enable_compression;
}

void ome_tiff_io_impl::set_compression(bool enabled) {
    m_enable_compression = enabled;
}

ome_tiff_io::ome_tiff_io() : m_pimpl(new ome_tiff_io_impl()){

}

ome_tiff_io::ome_tiff_io(boost::filesystem::path t_path) : m_pimpl(new ome_tiff_io_impl(std::move(t_path))) {

}

ome_tiff_io::ome_tiff_io(boost::filesystem::path t_path,
                         std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> t_metadata) :
        m_pimpl(new ome_tiff_io_impl(std::move(t_path), std::move(t_metadata))){

}

ome_tiff_io::ome_tiff_io(boost::filesystem::path t_path, const ome_tiff_io &t_reference) :
        m_pimpl(new ome_tiff_io_impl(std::move(t_path), t_reference)){

}

ome_tiff_io::~ome_tiff_io() {
    delete m_pimpl;
}

void ome_tiff_io::write_plane(const cv::Mat &image, const misa_ome_plane_description &index) {
    m_pimpl->write_plane(image, index);
}

cv::Mat ome_tiff_io::read_plane(const misa_ome_plane_description &index) const {
    return m_pimpl->read_plane(index);
}

std::shared_ptr<::ome::xml::meta::OMEXMLMetadata> ome_tiff_io::get_metadata() const {
    return m_pimpl->get_metadata();
}

boost::filesystem::path ome_tiff_io::get_path() const {
    return m_pimpl->get_path();
}

void ome_tiff_io::close(bool remove_write_buffer) {
    m_pimpl->close(remove_write_buffer);
}

::ome::files::dimension_size_type ome_tiff_io::get_num_series() const {
    return m_pimpl->get_num_series();
}

::ome::files::dimension_size_type ome_tiff_io::get_size_x(::ome::files::dimension_size_type series) const {
    return m_pimpl->get_size_x(series);
}

::ome::files::dimension_size_type ome_tiff_io::get_size_y(::ome::files::dimension_size_type series) const {
    return m_pimpl->get_size_y(series);
}

::ome::files::dimension_size_type ome_tiff_io::get_size_z(::ome::files::dimension_size_type series) const {
    return m_pimpl->get_size_z(series);
}

::ome::files::dimension_size_type ome_tiff_io::get_size_t(::ome::files::dimension_size_type series) const {
    return m_pimpl->get_size_t(series);
}

::ome::files::dimension_size_type ome_tiff_io::get_size_c(::ome::files::dimension_size_type series) const {
    return m_pimpl->get_size_c(series);
}

::ome::files::dimension_size_type ome_tiff_io::get_num_planes(::ome::files::dimension_size_type series) const {
    return m_pimpl->get_num_planes(series);
}

bool ome_tiff_io::compression_is_enabled() const {
    return m_pimpl->compression_is_enabled();
}

void ome_tiff_io::set_compression(bool enabled) {
    m_pimpl->set_compression(enabled);
}
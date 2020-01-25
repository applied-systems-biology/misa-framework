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

#include <misaxx/imaging/utils/tiffio.h>
#include <tiff.h>
#include <tiffio.h>
#include <boost/filesystem.hpp>

/**
     * RAII wrapper around libtiff
     */
class tiff_reader {
public:

    explicit tiff_reader(const std::string &t_filename);

    ~tiff_reader();

    tiff_reader(const tiff_reader &t_src) = delete;

    tiff &instance() {
        return *m_tiff;
    }

    const tiff &instance() const {
        return *m_tiff;
    }

    unsigned int get_image_height() const;

    unsigned int get_image_width() const;

    unsigned short get_num_samples() const;

    unsigned short get_sample_format() const;

    unsigned short get_depth() const;

    cv::Size get_size() const;

    void read_row_(void *row_out, unsigned int y);

private:

    tiff *m_tiff;

};

tiff_reader::tiff_reader(const std::string &t_filename) : m_tiff(TIFFOpen(t_filename.c_str(), "r")) {

}

tiff_reader::~tiff_reader() {
    TIFFClose(m_tiff);
}

unsigned int tiff_reader::get_image_height() const {
    uint32 value;
    TIFFGetField(m_tiff, TIFFTAG_IMAGELENGTH, &value);
    return value;
}

unsigned int tiff_reader::get_image_width() const {
    uint32 value;
    TIFFGetField(m_tiff, TIFFTAG_IMAGEWIDTH, &value);
    return value;
}

unsigned short tiff_reader::get_num_samples() const {
    uint16 value;
    TIFFGetField(m_tiff, TIFFTAG_SAMPLESPERPIXEL, &value);
    return value;
}

unsigned short tiff_reader::get_sample_format() const {
    uint16 value;
    TIFFGetField(m_tiff, TIFFTAG_SAMPLEFORMAT, &value);
    return value;
}

unsigned short tiff_reader::get_depth() const {
    uint16 value;
    TIFFGetField(m_tiff, TIFFTAG_BITSPERSAMPLE, &value);
    return value;
}

cv::Size tiff_reader::get_size() const {
    return cv::Size2i(get_image_width(), get_image_height());
}

void tiff_reader::read_row_(void *row_out, unsigned int y) {
    TIFFReadScanline(m_tiff, row_out, y);
}

class tiff_writer {
public:

    explicit tiff_writer(const std::string &t_filename,
            const cv::Size &t_image_size,
            unsigned short t_num_samples,
            unsigned short t_depth,
            unsigned short t_sample_format,
            unsigned short t_compression);

    ~tiff_writer();

    tiff_writer(const tiff_writer &t_src) = delete;

    tiff &instance() {
        return *m_tiff;
    }

    const tiff &instance() const {
        return *m_tiff;
    }

    void write_row_(const void* row, unsigned int y, unsigned short sample = 0);

private:

    tiff *m_tiff;
};

tiff_writer::tiff_writer(const std::string &t_filename,
        const cv::Size &t_image_size,
        unsigned short t_num_samples,
        unsigned short t_depth,
        unsigned short t_sample_format,
        unsigned short t_compression) : m_tiff(TIFFOpen(t_filename.c_str(), "w")) {
    TIFFSetField(m_tiff, TIFFTAG_IMAGEWIDTH, static_cast<uint32>(t_image_size.width));  // set the width of the image
    TIFFSetField(m_tiff, TIFFTAG_IMAGELENGTH, static_cast<uint32>(t_image_size.height));    // set the height of the image
    TIFFSetField(m_tiff, TIFFTAG_BITSPERSAMPLE, t_depth);
    TIFFSetField(m_tiff, TIFFTAG_SAMPLESPERPIXEL, t_num_samples);
    TIFFSetField(m_tiff, TIFFTAG_SAMPLEFORMAT, t_sample_format);
    TIFFSetField(m_tiff, TIFFTAG_COMPRESSION, t_compression);
}

tiff_writer::~tiff_writer() {
    TIFFClose(m_tiff);
}

void tiff_writer::write_row_(const void *row, unsigned int y, unsigned short sample) {
    TIFFWriteScanline(m_tiff, const_cast<void*>(row), y, sample);
}

cv::Mat misaxx::imaging::utils::tiffread(const boost::filesystem::path &t_path) {
    tiff_reader reader {t_path.string()};
    int opencv_type;
    switch(reader.get_sample_format()) {
        case 0: // Default to UINT
        case SAMPLEFORMAT_UINT:
            switch(reader.get_depth()) {
                case 8:
                    opencv_type = CV_8UC(reader.get_num_samples());
                    break;
                case 16:
                    opencv_type = CV_16UC(reader.get_num_samples());
                    break;
                default:
                    throw std::runtime_error("Unsupported depth!");
            }
            break;
        case SAMPLEFORMAT_INT:
            switch(reader.get_depth()) {
                case 8:
                    opencv_type = CV_8SC(reader.get_num_samples());
                    break;
                case 16:
                    opencv_type = CV_16SC(reader.get_num_samples());
                    break;
                case 32:
                    opencv_type = CV_32SC(reader.get_num_samples());
                    break;
                default:
                    throw std::runtime_error("Unsupported depth!");
            }
            break;
        case SAMPLEFORMAT_IEEEFP:
            switch(reader.get_depth()) {
                case 32:
                    opencv_type = CV_32FC(reader.get_num_samples());
                    break;
                case 64:
                    opencv_type = CV_64FC(reader.get_num_samples());
                    break;
                default:
                    throw std::runtime_error("Unsupported depth!");
            }
            break;
        default:
            throw std::runtime_error("Unsupported TIFF sample format " + std::to_string(reader.get_sample_format()));
    }

    cv::Mat result(reader.get_size(), opencv_type);
    for(int row = 0; row < reader.get_image_height(); ++row) {
        reader.read_row_(result.ptr(row), row);
    }
    return result;
}

void misaxx::imaging::utils::tiffwrite(const cv::Mat &t_img, const boost::filesystem::path &t_path, tiff_compression t_compression) {

    if(t_img.channels() > 1) {
        if(boost::filesystem::exists(t_path)) {
            boost::filesystem::remove(t_path);
        }
        cv::imwrite(t_path.string(), t_img);
        return;
    }

    ushort num_samples = t_img.channels();
    ushort depth;
    ushort sample_format;

    switch(t_img.depth()) {
        case CV_8S:
            depth = 8;
            sample_format = SAMPLEFORMAT_INT;
            break;
        case CV_8U:
            depth = 8;
            sample_format = SAMPLEFORMAT_UINT;
            break;
        case CV_16S:
            depth = 16;
            sample_format = SAMPLEFORMAT_INT;
            break;
        case CV_16U:
            depth = 16;
            sample_format = SAMPLEFORMAT_UINT;
            break;
        case CV_32S:
            depth = 32;
            sample_format = SAMPLEFORMAT_INT;
            break;
        case CV_32F:
            depth = 32;
            sample_format = SAMPLEFORMAT_IEEEFP;
            break;
        case CV_64F:
            depth = 64;
            sample_format = SAMPLEFORMAT_IEEEFP;
            break;
        default:
            throw std::runtime_error("Unsupported depth!");
    }

    tiff_writer writer {t_path.string(), t_img.size(), num_samples, depth, sample_format, static_cast<ushort >(t_compression)};
    if(num_samples == 1) {
        for(int row = 0; row < t_img.rows; ++row) {
            writer.write_row_(t_img.ptr(row), row);
        }
    }
    else {
        throw std::runtime_error("Unsupported type!");
    }

}

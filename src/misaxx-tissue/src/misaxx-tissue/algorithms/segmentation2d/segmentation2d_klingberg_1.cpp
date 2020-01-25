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

#include "segmentation2d_klingberg_1.h"
#include <misaxx/ome/attachments/misa_ome_pixel_count.h>

using namespace misaxx;
using namespace misaxx::ome;
using namespace misaxx_tissue;

namespace cv::images {
    using mask = cv::Mat1b;
    using grayscale32f = cv::Mat1f;
    using labels = cv::Mat1i;
}

namespace {

    template<typename T>
    std::vector<T> get_sorted_pixels(const cv::Mat_<T> &img) {
        std::vector<T> pixels;
        pixels.reserve(img.rows * img.cols);
        for(int y = 0; y < img.rows; ++y) {
            const auto *row = img[y];
            for(int x = 0; x < img.cols; ++x) {
                pixels.push_back(row[x]);
            }
        }
        std::sort(pixels.begin(), pixels.end());
        return pixels;
    }

    /**
     * Gets percentiles (linear interpolation)
     * @tparam T
     * @param pixels
     * @param percentiles
     * @return
     */
    template<typename T>
    std::vector<double> get_percentiles(const std::vector<T> &pixels, const std::vector<double> &percentiles) {
        std::vector<double> result;
        for(double percentile : percentiles) {
            double rank = percentile / 100.0 * (pixels.size() - 1);
            size_t lower_rank = static_cast<size_t>(std::floor(rank));
            size_t higher_rank = static_cast<size_t>(std::ceil(rank));
            double frac = rank - lower_rank; // fractional section
            double p = pixels[lower_rank] + (pixels[higher_rank] - pixels[lower_rank]) * frac;
            result.push_back(p);
        }
        return result;
    }

    double get_max_value(const cv::Mat &img) {
        double max;
        cv::minMaxLoc(img, nullptr, &max);
        return max;
    }

    void normalize_by_max(cv::images::mask &img) {
        const double max = get_max_value(img);
        for(int y = 0; y < img.rows; ++y) {
            auto *row = img[y];
            for(int x = 0; x < img.cols; ++x) {
                row[x] = static_cast<uchar>(row[x] * 255.0 / max);
            }
        }
    }

    void normalize_by_max(cv::images::grayscale32f &img) {
        const double max = get_max_value(img);
        for(int y = 0; y < img.rows; ++y) {
            auto *row = img[y];
            for(int x = 0; x < img.cols; ++x) {
                row[x] = static_cast<float>(row[x] / max);
            }
        }
    }

//    void normalize_by_minmax(cv::images::mask &img) {
//        double dmin;
//        double dmax;
//        cv::minMaxLoc(img, &dmin, &dmax);
//        for(int y = 0; y < img.rows; ++y) {
//            auto *row = img[y];
//            for(int x = 0; x < img.cols; ++x) {
//                row[x] = static_cast<uchar>(std::clamp((row[x] - dmin) / (dmax - dmin) * 255, 0.0, 255.0));
//            }
//        }
//    }

//    void normalize_by_percentile_and_min_signal(cv::images::mask &img, double percentile, double min_signal) {
//        if(percentile < 100) {
//            const auto pixels = get_sorted_pixels(img);
//            auto percentiles =  get_percentiles(pixels, { percentile, 100.0 - percentile });
//            double upper_percentile = percentiles[0];
//            double lower_percentile = percentiles[1];
//
//            if(upper_percentile > min_signal) {
//                img = img - lower_percentile;
//                cv::threshold(img.clone(), img, upper_percentile - lower_percentile, 0, cv::THRESH_TRUNC);
//                normalize_by_max(img);
//            }
//            else {
//                img = 0;
//            }
//        }
//        else {
//            if(get_max_value(img) < min_signal) {
//                img = 0;
//            }
//            else {
//                // Percentile converges to max(image) with increasing percentile
//                // Collapses into MINMAX normalization
//                normalize_by_minmax(img);
//            }
//        }
//    }

    void remove_low_average_intensity_objects(cv::images::mask &img_mask, const cv::images::grayscale32f &img_reference, float min_intensity) {
        cv::images::labels objects;
        cv::connectedComponents(img_mask, objects, 4, CV_32S);

        std::unordered_map<int, float> counts;
        std::unordered_map<int, float> intensities;
        for(int y = 0; y < objects.rows; ++y) {
            const int *row = objects[y];
            const float *row_reference = img_reference[y];
            for (int x = 0; x < objects.cols; ++x) {
                if (row[x] > 0) {
                    counts[row[x]] += 1;
                    intensities[row[x]] += row_reference[x];
                }
            }
        }

        for(int y = 0; y < objects.rows; ++y) {
            const int *row = objects[y];
            uchar *row_mask = img_mask[y];
            for (int x = 0; x < objects.cols; ++x) {
                if (row[x] > 0) {
                    float mean = intensities[row[x]] / counts[row[x]];
//                    std::cout << std::to_string(mean) << " < " << std::to_string(min_intensity) << std::endl;
                    if(mean < min_intensity)
                        row_mask[x] = 0;
                }
            }
        }
    }

    cv::images::grayscale32f get_as_grayscale_float_copy(const cv::Mat &img) {
        if(img.type() == CV_32F) {
            return img.clone();
        }
        else if(img.type() == CV_64F) {
            cv::images::grayscale32f result;
            img.convertTo(result, CV_32F, 1);
            return result;
        }
        else if(img.type() == CV_8U) {
            cv::images::grayscale32f result;
            img.convertTo(result, CV_32F, 1.0 / 255.0);
            return result;
        }
        else if(img.type() == CV_16U) {
            cv::images::grayscale32f result;
            img.convertTo(result, CV_32F, 1.0 / std::numeric_limits<ushort>::max());
            return result;
        }
        else {
            throw std::runtime_error("Unsupported image depth: " + std::to_string(img.type()));
        }
    }

    void close_holes(cv::images::mask &img) {
        using T = uchar;
        const uchar white = 255;
        const uchar black = 0;
        cv::images::mask buffer { img.size(), 0 };
        std::vector<cv::Point> neighbors;
        neighbors.emplace_back(cv::Point(-1,0));
        neighbors.emplace_back(cv::Point(1,0));
        neighbors.emplace_back(cv::Point(0,1));
        neighbors.emplace_back(cv::Point(0,-1));

        std::stack<cv::Point> stack;

        // Find the points in x direction (1st dimension)
        cv::Point pos(0,0);

        int rows = img.rows;
        int cols = img.cols;

        for(int i = 0; i < rows; ++i) {
            pos.x = 0;

            if(img.at<T>(pos) == 0 && buffer.at<T>(pos) == 0) {
                buffer.at<T>(pos) = white;
                stack.push(pos);
            }

            pos.x = cols - 1;

            if(img.at<T>(pos) == 0 && buffer.at<T>(pos) == 0) {
                buffer.at<T>(pos) = white;
                stack.push(pos);
            }

            // Increase counter of y
            if(pos.y < rows) {
                ++pos.y;
            }
            else {
                pos.y = 0;
            }
        }

        pos = cv::Point(0,0);

        // Find the points in y direction (2nd dimension)

        for(int i = 0; i < cols; ++i) {
            pos.y = 0;

            if(img.at<T>(pos) == 0 && buffer.at<T>(pos) == 0) {
                buffer.at<T>(pos) = white;
                stack.push(pos);
            }

            pos.y = rows - 1;

            if(img.at<T>(pos) == 0 && buffer.at<T>(pos) == 0) {
                buffer.at<T>(pos) = white;
                stack.push(pos);
            }

            // Increase counter of y
            if(pos.x < cols) {
                ++pos.x;
            }
            else {
                pos.x = 0;
            }
        }

        // Apply
        while(!stack.empty()) {
            cv::Point pos2 = stack.top();
            stack.pop();

            for(const cv::Point & rel_neighbor : neighbors) {
                cv::Point absolute = rel_neighbor + pos2;

                if(absolute.x >= 0 && absolute.y >= 0 && absolute.x < img.cols && absolute.y < img.rows) {
                    if(img.at<T>(absolute) == 0 && buffer.at<T>(absolute) == 0) {
                        buffer.at<T>(absolute) = white;
                        stack.push(absolute);
                    }
                }
            }
        }

        // Invert the image
        for(int i = 0; i < buffer.rows; ++i) {
            auto *row = buffer.ptr<T>(i);
            for(int j = 0; j < buffer.cols; ++j) {
                row[j] = white - row[j];
            }
        }

        img = buffer;
    }

    cv::images::mask to_mask(const cv::images::grayscale32f &img, double threshold) {
        cv::images::grayscale32f tmp;
        cv::threshold(img, tmp, threshold, 255, cv::THRESH_BINARY);
        cv::images::mask result;
        tmp.convertTo(result, CV_8U, 1);
        return result;
    }

    cv::images::mask create_disk(int radius) {
        cv::images::mask result { cv::Size(radius * 2 + 1, radius * 2 + 1), 0 };

        const int c = result.rows / 2;
        for(int y = 0; y < result.rows; ++y) {
            auto *row = result[y];
            for(int x = 0; x < result.cols; ++x) {
                if(std::pow(x - c, 2) + std::pow(y - c, 2) < radius * radius) {
                    row[x] = 255;
                }
            }
        }

        return result;
    }

    int interpolation_from_string(const std::string interpolation_name) {
        if(interpolation_name == "cubic") {
            return cv::INTER_CUBIC;
        }
        else if(interpolation_name == "linear") {
            return cv::INTER_LINEAR;
        }
        else {
            throw std::runtime_error("Unsupported interpolation");
        }
    }
}

void segmentation2d_klingberg_1::create_parameters(misa_parameter_builder &t_parameters) {
    segmentation2d_base::create_parameters(t_parameters);
    m_median_filter_size = t_parameters.create_algorithm_parameter<int>("median-filter-size", 3);
    m_downscale_factor = t_parameters.create_algorithm_parameter<int>("downscale-factor", 10);
    m_thresholding_percentile = t_parameters.create_algorithm_parameter<double>("thresholding-percentile", 40);
    m_thresholding_percentile_factor = t_parameters.create_algorithm_parameter<double>("thresholding-percentile-factor", 1.5);
    m_morph_disk_radius = t_parameters.create_algorithm_parameter<int>("morph-disk-radius", 5);
    m_label_min_percentile = t_parameters.create_algorithm_parameter<double>("label-min-percentile", 2);
    m_resize_interpolation = t_parameters.create_algorithm_parameter<std::string>("resize-interpolation", "cubic");
    m_resize_interpolation.schema->make_enum<std::string>({ "cubic", "linear" });
}

void segmentation2d_klingberg_1::work() {

    auto module = get_module_as<module_interface>();

    cv::images::grayscale32f img = get_as_grayscale_float_copy(m_input_autofluoresence.access_readonly().get());

    // Median filtering + normalization
    cv::medianBlur(img.clone(), img, m_median_filter_size.query());
    normalize_by_max(img);

    // Generated parameters
    const double xsize = module->m_voxel_size.get_size_xy().get_value();
    const double gauss_sigma = 3.0 / xsize;

    cv::Size img_original_size = img.size();

    // Downscale
    cv::images::grayscale32f img_small;
    cv::resize(img, img_small, cv::Size(img.size().width / m_downscale_factor.query(), img.size().height / m_downscale_factor.query()), 0,0,
            interpolation_from_string(m_resize_interpolation.query()));

    // Gauss filter
    cv::GaussianBlur(img_small.clone(), img_small,cv::Size(), gauss_sigma, gauss_sigma);

    // Find a percentile we later use
    const double tissue_percentile = get_percentiles(get_sorted_pixels(img_small),  { m_thresholding_percentile.query() })[0];
//    std::cout << m_output_segmented2d.get_plane_location().z << ": Tissue percentile: " << (tissue_percentile * 255) << std::endl;

    // Binarize using percentile based threshold
    cv::images::mask small_mask = to_mask(img_small, tissue_percentile * m_thresholding_percentile_factor.query());

    // Morphological operations (dilation, hole closing, eroding)
    {
        const auto disk = create_disk(m_morph_disk_radius.query());

        cv::morphologyEx(small_mask.clone(), small_mask, cv::MORPH_DILATE, disk, cv::Point(-1,-1), 1, cv::BORDER_CONSTANT, cv::Scalar::all(0));
        close_holes(small_mask);
        cv::morphologyEx(small_mask.clone(), small_mask, cv::MORPH_ERODE, disk, cv::Point(-1,-1), 1, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    }
//
//    // First find all distinct objects in the mask
//    // Then remove all objects from the mask where the average intensity of pixels within img_reference
//    // is < k * tissue_percentile
    remove_low_average_intensity_objects(small_mask, img_small, static_cast<float>(tissue_percentile * m_label_min_percentile.query()));

    // Upscale, normalize & fully binarize
    cv::images::mask full_mask;
    cv::resize(small_mask, full_mask, img_original_size, 0,0,cv::INTER_CUBIC);
    cv::threshold(full_mask.clone(), full_mask, 0, 255,cv::THRESH_OTSU);

    // Count pixels for later
    m_output_segmented2d.attach(misaxx::ome::misa_ome_pixel_count(cv::countNonZero(full_mask)));

    // Save the mask
    m_output_segmented2d.write(std::move(full_mask));
}

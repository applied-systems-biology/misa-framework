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

#include <misaxx-microbench/module_interface.h>
#include <chrono>
#include <misaxx-microbench/attachments/microbench_runtimes.h>
#include "microbench_task.h"

using namespace misaxx_microbench;

using chrono_clock_t = std::chrono::high_resolution_clock;
using duration_ms_t = std::chrono::duration<double, std::milli >;
using timepoint_t = std::chrono::time_point<chrono_clock_t>;

namespace cv::images {
    using grayscale8u = cv::Mat1b;
    using grayscale32f = cv::Mat1f;
    using mask = cv::Mat1b;
    using labels = cv::Mat1i;
    using complex = cv::Mat2f;
}
namespace {
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
    std::vector<T> get_percentiles(const std::vector<T> &pixels, const std::vector<double> &percentiles) {
        std::vector<T> result;
        for(double percentile : percentiles) {
            double rank = percentile / 100.0 * (pixels.size() - 1);
            size_t lower_rank = static_cast<size_t>(std::floor(rank));
            size_t higher_rank = static_cast<size_t>(std::ceil(rank));
            double frac = rank - lower_rank; // fractional section
            double p = pixels[lower_rank] + (pixels[higher_rank] - pixels[lower_rank]) * frac;
            result.push_back(static_cast<T>(p));
        }
        return result;
    }

    cv::images::grayscale32f wiener2(const cv::images::grayscale32f &img, int neighborhood=3, float noise_variance = 0) {
        cv::images::grayscale32f img_mean {img.size(), 0};
        cv::images::grayscale32f img2 = img.mul(img, 1);
        cv::images::grayscale32f img2_mean {img.size(), 0};

        cv::boxFilter(img, img_mean, CV_32F, cv::Size(neighborhood, neighborhood));
        cv::boxFilter(img2, img2_mean, CV_32F, cv::Size(neighborhood, neighborhood));

        cv::images::grayscale32f img_var = img2_mean + img_mean.mul(img_mean, 1) + 1e-06;
        if(noise_variance <= 0) {
            auto nv = cv::mean(img_var);
            noise_variance = nv[0];
        }

        return img_mean + ((img_var - noise_variance) / img_var).mul(img - img_mean);
    }

    cv::images::grayscale32f fftpad(const cv::images::grayscale32f &img) {
        int optimalWidth = cv::getOptimalDFTSize(img.size().width);
        int optimalHeight = cv::getOptimalDFTSize(img.size().height);
        int bright = optimalWidth - img.size().width;
        int bbottom = optimalHeight - img.size().height;
        cv::images::grayscale32f padded = img;
        cv::copyMakeBorder(img, padded, 0, bbottom, 0, bright, cv::BORDER_CONSTANT, cv::Scalar::all(0));
        return padded;
    }

    cv::images::grayscale32f fftunpad(const cv::images::grayscale32f &img, const cv::images::grayscale32f &source) {
        cv::Size source_size = source.size();
        cv::Rect roi { 0, 0, source_size.width, source_size.height };
        cv::images::grayscale32f result { source_size, 0 };
        img(roi).copyTo(result);
        return result;
    }
}

void microbench_task::work() {
    auto module = get_module_as<misaxx_microbench::module_interface>();
    std::vector<timepoint_t > times {};
    times.push_back(chrono_clock_t::now());

    auto img_access = module->m_input_image.access_readonly();
    cv::images::grayscale32f img = get_as_grayscale_float_copy(img_access.get());
    times.push_back(chrono_clock_t::now());

    // Median filter
    cv::images::grayscale32f img_median_filtered {img.size(), 0};
    {
        cv::images::grayscale8u img_8u {img.size(), 0};
        img.convertTo(img_8u, CV_8U, 255);
        cv::images::mask img_median_filtered_ {img.size(), 0};
        cv::medianBlur(img_8u, img_median_filtered_, 21);
        img_median_filtered_.convertTo(img_median_filtered, CV_32F, 1.0f / 255.0f);
    }
    times.push_back(chrono_clock_t::now());

    // Morphology benchmark
    cv::images::grayscale32f img_dilated {img.size(), 0};
    cv::morphologyEx(img, img_dilated, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(31, 31)));
    times.push_back(chrono_clock_t::now());

    // FFT / IFFT
    cv::images::grayscale32f img_ifft {};
    {
        cv::images::grayscale32f img_padded = fftpad(img);
        cv::images::complex img_fft {img_padded.size(), cv::Vec2f(0, 0)};
        cv::dft(img_padded, img_fft, cv::DFT_COMPLEX_OUTPUT);
        cv::images::complex img_ifft_ {img_padded.size(), cv::Vec2f(0, 0)};
        cv::dft(img_fft, img_ifft_, cv::DFT_REAL_OUTPUT | cv::DFT_SCALE);
        cv::images::grayscale32f img_ifftr { img_padded.size(), 0 };
        cv::mixChannels({ img_ifft_ }, {img_ifftr}, {0, 0});
        img_ifft = fftunpad(img_ifftr, img);
    }

    times.push_back(chrono_clock_t::now());

    // Otsu
    cv::images::grayscale32f img_otsu {img.size(), 0};
    {
        cv::images::grayscale8u img_8u {img.size(), 0};
        img.convertTo(img_8u, CV_8U, 255);
        cv::images::mask img_otsu_ {img.size(), 0};
        cv::threshold(img_8u, img_otsu_, 0, 255, cv::THRESH_OTSU);
        img_otsu_.convertTo(img_otsu, CV_32F, 1.0f / 255.0f);
    }
    times.push_back(chrono_clock_t::now());

    // Percentile
    cv::images::grayscale32f img_percentile {img.size(), 0};
    {
        float threshold = get_percentiles<float>(get_sorted_pixels<float>(img), { 65.0 })[0];
        cv::threshold(img, img_percentile, threshold, 255, cv::THRESH_BINARY);
    }
    times.push_back(chrono_clock_t::now());

    // Canny
    cv::images::grayscale32f img_canny {img.size(), 0};
    {
        cv::images::grayscale8u img_8u {img.size(), 0};
        img.convertTo(img_8u, CV_8U, 255);
        cv::GaussianBlur(img_8u.clone(), img_8u, cv::Size(0, 0), 1);
        cv::images::mask img_canny_ {img.size(), 0};
        cv::Canny(img_8u, img_canny_, 0.1 * 255, 0.2 * 255, 3);
        img_canny_.convertTo(img_canny, CV_32F, 1.0f / 255.0f);
    }
    times.push_back(chrono_clock_t::now());

    // Wiener2
    cv::images::grayscale32f img_wiener2 = wiener2(img);
    times.push_back(chrono_clock_t::now());

    // IO
    module->m_output_median.write(img_median_filtered);
    module->m_output_morphology.write(img_dilated);
    module->m_output_fft_ifft.write(img_ifft);
    module->m_output_otsu.write(img_otsu);
    module->m_output_percentile.write(img_percentile);
    module->m_output_canny.write(img_canny);
    module->m_output_wiener.write(img_wiener2);
    times.push_back(chrono_clock_t::now());

    // Save benchmark results
    microbench_runtimes runtimes {};
    std::vector<std::string> time_points = { "",  "io", "median", "morphology", "fft-ifft", "otsu", "percentile", "canny", "wiener2", "io"};
    for(size_t i = 1; i < time_points.size(); ++i) {
        runtimes.data[time_points[i]] += std::chrono::duration_cast<duration_ms_t> (times[i] - times[i - 1]).count() / 1000.0;
    }

    module->m_runtimes.attach(runtimes);
}

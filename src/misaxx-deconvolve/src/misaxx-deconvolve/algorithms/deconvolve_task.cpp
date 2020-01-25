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

#include "deconvolve_task.h"
#include <opencv2/opencv.hpp>
#include <misaxx-deconvolve/module_interface.h>
#include <cmath>
#include <misaxx/imaging/utils/tiffio.h>

using namespace misaxx_deconvolve;

namespace cv::images {
    using grayscale8u = cv::Mat1b;
    using grayscale32f = cv::Mat1f;
    using mask = cv::Mat1b;
    using labels = cv::Mat1i;
    using complex = cv::Mat2f;
}

namespace {

    cv::images::grayscale32f get_as_grayscale_float_copy(const cv::Mat &img) {
        if (img.type() == CV_32F) {
            return img.clone();
        } else if (img.type() == CV_64F) {
            cv::images::grayscale32f result;
            img.convertTo(result, CV_32F, 1);
            return result;
        } else if (img.type() == CV_8U) {
            cv::images::grayscale32f result;
            img.convertTo(result, CV_32F, 1.0 / 255.0);
            return result;
        } else if (img.type() == CV_16U) {
            cv::images::grayscale32f result;
            img.convertTo(result, CV_32F, 1.0 / std::numeric_limits<ushort>::max());
            return result;
        } else {
            throw std::runtime_error("Unsupported image depth: " + std::to_string(img.type()));
        }
    }

    cv::Size get_fft_size(const cv::Mat &img, const cv::Mat &kernel) {
        return cv::Size(img.size().width + kernel.size().width - 1,
                        img.size().height + kernel.size().height - 1);
    }

    cv::images::grayscale32f
    fftunpad(const cv::images::grayscale32f &deconvolved, const cv::Size &target_size, const cv::Size &source_size) {
        cv::Size ap{};
        if (source_size.width % 2 == 0)
            ap.width = 1;
        if (source_size.height % 2 == 0)
            ap.height = 1;
//        cv::Size fftOptPad = get_fftoptpad(source_size, target_size);
        int padded_width = deconvolved.size().width;
        int padded_height = deconvolved.size().height;

        int bleft = (padded_width - source_size.width - ap.width) / 2;
        int btop = (padded_height - source_size.height - ap.height) / 2;
        cv::Rect roi{bleft, btop, source_size.width, source_size.height};
        cv::images::grayscale32f result{source_size, 0};
        deconvolved(roi).copyTo(result);
        return result;
    }



    cv::images::grayscale32f fftpad(const cv::images::grayscale32f &img, const cv::Size &target_size, bool shift = false) {
        cv::Size ap{};
        if (img.size().width % 2 == 0)
            ap.width = 1;
        if (img.size().height % 2 == 0)
            ap.height = 1;

        cv::Size c{};
        c.width = (target_size.width - img.size().width - ap.width) / 2;
        c.height = (target_size.height - img.size().height - ap.height) / 2;

        int bleft = c.width;
        int btop = c.height;
        int bright = c.width + ap.width;
        int bbottom = c.height + ap.height;

        // Further pad to optimal FFT size
        {
            int currentWidth = bleft + bright + img.size().width;
            int currentHeight = btop + bbottom + img.size().height;
            int optimalWidth = cv::getOptimalDFTSize(currentWidth);
            int optimalHeight = cv::getOptimalDFTSize(currentHeight);

            int dow = optimalWidth - currentWidth;
            int doh = optimalHeight - currentHeight;
            int ow0 = dow / 2 + 1;
            int ow1 = dow - ow0;
            int oh0 = doh / 2 + 1;
            int oh1 = doh - oh0;

            // Add to padding
            bleft += ow0;
            btop += oh0;
            bright += ow1;
            bbottom += oh1;
        }

        cv::images::grayscale32f padded = img;
        cv::copyMakeBorder(img, padded, btop, bbottom, bleft, bright, cv::BORDER_CONSTANT, cv::Scalar::all(0));


        if (shift) {
            cv::images::grayscale32f tmp {};
            cv::repeat(padded, 2, 2, tmp);
            int sx = padded.cols;
            int sy = padded.rows;
            padded = tmp(cv::Rect(sx / 2, sy / 2, sx, sy));
        }

        return padded;
    }

    cv::images::complex fft(const cv::images::grayscale32f &padded) {
        cv::images::complex result{padded.size(), cv::Vec2f{0, 0}};
        cv::dft(padded, result, cv::DFT_COMPLEX_OUTPUT);
        return result;
    }

    cv::images::grayscale32f ifft(const cv::images::complex &fft) {
        cv::images::grayscale32f result{fft.size(), 0};
        cv::idft(fft, result, cv::DFT_REAL_OUTPUT | cv::DFT_SCALE);
        return result;
    }

    cv::images::grayscale32f get_laplacian8_kernel() {
        cv::images::grayscale32f result{cv::Size(3, 3), 0};
        for (int y = 0; y < result.rows; ++y) {
            float *row = result[y];
            for (int x = 0; x < result.cols; ++x) {
                row[x] = x == 1 && y == 1 ? -1.0f : 1.0f / 8.0f;
            }
        }
        return result;
    }

    cv::images::complex get_laplacian_fft(const cv::Size &target_size, const cv::Point &nudge = cv::Point()) {
        cv::Size quadrant_size{target_size.width / 2 + 1, target_size.height / 2 + 1};

        cv::images::complex quadrant{quadrant_size, cv::Vec2f(0, 0)};
        for (int y = 0; y < quadrant.rows; ++y) {
            cv::Vec2f *row = quadrant[y];
            const float wy = M_PI * (y + nudge.y) / (target_size.height / 2.0f);
            for (int x = 0; x < quadrant.cols; ++x) {
                const float wx = M_PI * (x + nudge.x) / (target_size.width / 2.0f);
                row[x] = cv::Vec2f(wx * wx + wy * wy, std::numeric_limits<float>::epsilon());
            }
        }

        cv::images::complex result{target_size, cv::Vec2f(0, 0)};

        cv::copyMakeBorder(quadrant,
                           result,
                           0,
                           target_size.height - quadrant_size.height,
                           0,
                           target_size.width - quadrant_size.width,
                           cv::BORDER_REFLECT);

        return result;
    }

    inline cv::Vec2f complex_add(cv::Vec2f a, cv::Vec2f b) {
        return cv::Vec2f{a[0] + b[0], a[1] + b[1]};
    }

    inline cv::Vec2f complex_mul(cv::Vec2f a, cv::Vec2f b) {
        return cv::Vec2f{a[0] * b[0] - a[1] * b[1], a[0] * b[1] + a[1] * b[0]};
    }

    inline cv::Vec2f scalar_mul(cv::Vec2f a, float b) {
        return cv::Vec2f{a[0] * b, a[1] * b};
    }

    inline cv::Vec2f complex_div(cv::Vec2f a, cv::Vec2f b) {
        const float A0 = a[0] * b[0] + a[1] * b[1];
        const float B0 = a[1] * b[0] - a[0] * b[1];
        const float D = b[0] * b[0] + b[1] * b[1];
        return cv::Vec2f{A0 / D, B0 / D};
    }

    inline float complex_abs(cv::Vec2f a) {
        return std::sqrt(a[0] * a[0] + a[1] * a[1]);
    }

    void save_fftreal(const cv::images::complex &fft, const std::string &file) {
        cv::images::grayscale32f result{fft.size(), 0};
        cv::mixChannels({fft}, {result}, {0, 0});
        misaxx::imaging::utils::tiffwrite(result, file);
    }

    void save_fftimag(const cv::images::complex &fft, const std::string &file) {
        cv::images::grayscale32f result{fft.size(), 0};
        cv::mixChannels({fft}, {result}, {1, 0});
        misaxx::imaging::utils::tiffwrite(result, file);
    }

}

void deconvolve_task::work() {

    auto module_interface = get_module_as<misaxx_deconvolve::module_interface>();
    auto access_convolved = module_interface->m_output_convolved.access_readonly();
    auto access_psf = module_interface->m_input_psf.access_readonly();

    const float rif_lambda = 0.001f;

//    {
//        cv::Mat abcd = get_as_grayscale_float_copy(misaxx::imaging::utils::tiffread("/home/ruman/abcd.tif"));
//        abcd = fftpad(abcd, abcd.size(), true);
//        misaxx::imaging::utils::tiffwrite(abcd, "/home/ruman/abcd_cv.tif");
//        std::terminate();
//    }

    cv::Size target_size = get_fft_size(access_convolved.get(), access_psf.get());

    cv::images::complex Y = fft(fftpad(access_convolved.get(), target_size));
    cv::images::complex H = fft(fftpad(access_psf.get(), target_size, true));
//    cv::images::complex L = fft(fftpad(get_laplacian8_kernel(), target_size, true));

    cv::images::complex L = get_laplacian_fft(H.size());
    cv::images::complex X{Y.size(), cv::Vec2f(0, 0)};

    for(int y = 0; y < Y.rows; ++y) {
        const cv::Vec2f *rH = H[y];
        const cv::Vec2f *rY = Y[y];
        const cv::Vec2f *rL = L[y];
        cv::Vec2f *rX = X[y];
        for(int x = 0; x < Y.cols; ++x) {
            const cv::Vec2f H2 = complex_mul(rH[x], rH[x]);
            const cv::Vec2f L2 = scalar_mul(complex_mul(rL[x], rL[x]), rif_lambda);
            const cv::Vec2f FA = complex_add(H2, L2);
            const cv::Vec2f FP = complex_div(rH[x], FA);
            const cv::Vec2f D = complex_mul(rY[x], FP);
            rX[x] = D;
        }
    }

    cv::images::grayscale32f deconvolved = fftunpad(ifft(X), target_size, access_convolved.get().size());
    module_interface->m_output_deconvolved.write(deconvolved);


}

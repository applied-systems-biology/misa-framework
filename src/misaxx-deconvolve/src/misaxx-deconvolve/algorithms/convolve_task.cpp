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

#include "convolve_task.h"
#include <opencv2/opencv.hpp>
#include <misaxx-deconvolve/module_interface.h>

using namespace misaxx_deconvolve;

namespace cv::images {
    using grayscale8u = cv::Mat1b;
    using grayscale32f = cv::Mat1f;
    using mask = cv::Mat1b;
    using labels = cv::Mat1i;
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
}

void convolve_task::work() {
    auto module_interface = get_module_as<misaxx_deconvolve::module_interface>();
    auto access_img = module_interface->m_input_image.access_readonly();
    cv::images::grayscale32f img = get_as_grayscale_float_copy(access_img.get());
    auto access_psf = module_interface->m_input_psf.access_readonly();

    cv::images::grayscale32f convolved {img.size(), 0};
    cv::filter2D(img, convolved, CV_32F, access_psf.get());

    module_interface->m_output_convolved.write(convolved);
}

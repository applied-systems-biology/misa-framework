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

#include "visualize_task.h"
#include <opencv2/opencv.hpp>
#include <misaxx-ome-visualizer/attachments/colormap.h>
#include <misaxx-ome-visualizer/module_interface.h>

using namespace misaxx_ome_visualizer;
using namespace misaxx;


void visualize_task::create_parameters(misa_parameter_builder &t_parameters) {

}

void visualize_task::work() {
    auto input_access = m_input.access_readonly();
    auto output_access = m_output.access_write();

    if(input_access.get().type() == CV_32S) {
        const auto attachment_access = get_module_as<module_interface>()->m_input.access_attachments_readonly();
        const auto &map = attachment_access.get().at<colormap>();

        // Apply recoloring
        cv::Mat3b result { input_access.get().size(), cv::Vec3b(0,0,0) };

        for(int y = 0; y < result.rows; ++y) {
            const int *row_src = input_access.get().ptr<int>(y);
            cv::Vec3b *row_result = result[y];
            for(int x = 0; x < result.cols; ++x) {
                row_result[x] = map.data.at(row_src[x]);
            }
        }

        output_access.set(std::move(result));
    }
    else if(input_access.get().channels() == 1) {
        cv::Mat result { };
        cv::cvtColor(input_access.get(), result, cv::COLOR_GRAY2BGR);
        output_access.set(std::move(result));
    }
    else {
        throw std::runtime_error("Unsupported OpenCV type " + std::to_string(input_access.get().type()));
    }
}


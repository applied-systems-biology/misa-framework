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

#include <misaxx-ome-visualizer/attachments/colormap.h>
#include <misaxx/ome/accessors/misa_ome_tiff.h>
#include <misaxx-ome-visualizer/module_interface.h>
#include "find_colormap_task.h"

using namespace misaxx;
using namespace misaxx_ome_visualizer;

void misaxx_ome_visualizer::find_colormap_task::create_parameters(misaxx::misa_parameter_builder &t_parameters) {
    m_filter_labels = t_parameters.create_algorithm_parameter<bool>("filter-labels", false)
            .document_title("Filter labels").document_description("If enabled, labels not included within the list of filtered labels are colored black.");
    m_filtered_labels = t_parameters.create_algorithm_parameter<std::string>("filtered-labels", "")
            .document_title("Filtered labels")
            .document_description("Comma-separated list of labels to be filtered");
    m_invert_filter_labels = t_parameters.create_algorithm_parameter<bool>("invert-filter-labels", false)
            .document_title("Invert filter labels")
            .document_description("If enabled, labels within the list are excluded");
}

void misaxx_ome_visualizer::find_colormap_task::work() {
    misaxx::ome::misa_ome_tiff images = get_module_as<module_interface>()->m_input;
    std::unordered_set<int> label_colors;
    for(size_t i = 0; i < images.size(); ++i) {
        std::cout << "Analyzing color map (" << i << "/" << images.size() << ")\n";
        auto input_access = images.at(i).access_readonly();
        if(input_access.get().type() == CV_32S) {
            for(int y = 0; y < input_access.get().rows; ++y) {
                const int *row = input_access.get().ptr<int>(y);
                for(int x = 0; x < input_access.get().cols; ++x) {
                    label_colors.insert(row[x]);
                }
            }
        }
    }


    if(!label_colors.empty()) {
        colormap attachment;

        cv::Mat hsv_in(1,1,CV_8UC3);
        cv::Mat bgr_out(1,1,CV_8UC3);

        hsv_in.at<cv::Vec3b>()[1] = 255;
        hsv_in.at<cv::Vec3b>()[2] = 255;

        for(int color : label_colors) {
            // Generate a color for this label
            double hue = (std::abs(color) % 256) / 255.0;
            hsv_in.at<cv::Vec3b>()[0] = static_cast<uchar>(hue * 180);
            cv::cvtColor(hsv_in, bgr_out, cv::COLOR_HSV2BGR);

            // Set into recoloring map
            attachment.data[color] = bgr_out.at<cv::Vec3b>(0);
        }

        if(m_filter_labels.query()) {
            std::unordered_set<int> filtered_labels;
            std::vector<std::string> cell;
            std::string filter_string = m_filtered_labels.query();
            boost::split(cell, filter_string, boost::is_any_of(","));
            for(std::string x : cell) {
                boost::trim(x);
                filtered_labels.insert(std::stoi(x));
            }

            if(!m_invert_filter_labels.query()) {
               std::unordered_set<int> to_remove;
               for(const auto &kv : attachment.data) {
                   if(filtered_labels.find(kv.first) == filtered_labels.end()) {
                       to_remove.insert(kv.first);
                   }
               }
                for(int filtered : to_remove) {
                    if(attachment.data.find(filtered) != attachment.data.end())
                        attachment.data[filtered] = cv::Vec3b(0,0,0);
                }
            }
            else {
                for(int filtered : filtered_labels) {
                    if(attachment.data.find(filtered) != attachment.data.end())
                        attachment.data[filtered] = cv::Vec3b(0,0,0);
                }
            }
        }

        // Color background black
        attachment.data[0] = cv::Vec3b(0,0,0);
        images.attach(std::move(attachment));
    }
}

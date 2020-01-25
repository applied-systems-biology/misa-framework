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

#include "quantification_klingberg.h"
#include <cmath>

using namespace misaxx;
using namespace misaxx::ome;
using namespace misaxx_kidney_glomeruli;

namespace {

    /**
     * Properties collected for labeling
     */
    struct glomerulus_properties {
        size_t pixels = 0;
    };

    std::unordered_map<int, glomerulus_properties> get_glomeruli_properties(const cv::Mat &labels) {
        std::unordered_map<int, glomerulus_properties> result;
        for(int y = 0; y < labels.rows; ++y) {
            const int *row = labels.ptr<int>(y);
            for(int x = 0; x < labels.cols; ++x) {
                if(row[x] > 0) {
                    result[row[x]].pixels += 1;
                }
            }
        }
        return result;
    };

}


void quantification_klingberg::work() {
    auto module = get_module_as<module_interface>();

    glomeruli result;

    for(const auto &plane : m_input_segmented3d) {
        auto access = plane.access_readonly();

        for(const auto& [group, glom_properties] : get_glomeruli_properties(access.get())) {

            if(group == 0)
                continue;

            glomerulus &glom = result.data[group];
            glom.pixels.count += glom_properties.pixels;
        }
    }

    // Calculate the properties of the glomeruli
    double glomerulus_min_volume = 4.0 / 3.0 * M_PI * std::pow(m_glomeruli_min_rad.query(), 3);
    double glomerulus_max_volume = 4.0 / 3.0 * M_PI * std::pow(m_glomeruli_max_rad.query(), 3);

    double diameter_sum = 0;
    double diameter_sum_sq = 0;

    for(auto &kv : result.data) {
        glomerulus &glom = kv.second;
        glom.label = kv.first;
        glom.volume = glom.pixels.get_volume(module->m_voxel_size);
        glom.diameter = misa_quantity<double, misa_ome_unit_length<1>>(2 * pow(3.0 / 4.0 * glom.volume.get_value() / M_PI, 1.0 / 3.0),
                                                                       misa_ome_unit_length<1>::ome_unit_type::MICROMETER);
        glom.valid = glom.volume.get_value() >= glomerulus_min_volume && glom.volume.get_value() <= glomerulus_max_volume;
        if(glom.valid) {
            ++result.valid_glomeruli_number;
            diameter_sum += glom.diameter.get_value();
            diameter_sum_sq += std::pow(glom.diameter.get_value(), 2);
        }
        else {
            ++result.invalid_glomeruli_number;
        }
    }

    result.valid_glomeruli_diameter_average = diameter_sum / result.valid_glomeruli_number;
    result.valid_glomeruli_diameter_variance = (diameter_sum_sq / result.valid_glomeruli_number) -
            std::pow(result.valid_glomeruli_diameter_average, 2);

    module->m_output_quantification.attach_foreign(std::move(result), module->m_output_segmented3d);
}

void quantification_klingberg::create_parameters(misa_parameter_builder &t_parameters) {
    quantification_base::create_parameters(t_parameters);
    m_glomeruli_min_rad = t_parameters.create_algorithm_parameter<double>("glomeruli-min-rad", 15);
    m_glomeruli_max_rad = t_parameters.create_algorithm_parameter<double>("glomeruli-max-rad", 65);
}

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

#include <misaxx/core/misa_module.h>
#include <misaxx-segment-cells/module.h>
#include "algorithms/segment_experiment.h"
#include "algorithms/quantify_conidia.h"

using namespace misaxx_segment_cells;

void module::create_blueprints(blueprint_list &t_blueprints, parameter_list &t_parameters) {
    t_blueprints.add(create_blueprint<segment_experiment>("segment-experiment"));
    t_blueprints.add(create_blueprint<quantify_conidia>("quantify-conidia"));
}

void module::build(const blueprint_builder &t_builder) {
    auto module_interface = get_module_as<misaxx_segment_cells::module_interface>();

    group segmentation_group {};

    for(const std::string &filename : module_interface->m_inputImages.get_filenames()) {
        auto &task = t_builder.build<segment_experiment>("segment-experiment");
        task.m_inputImage = module_interface->m_inputImages.at(filename);
        task.m_outputImage = module_interface->m_outputSegmented.at(filename);
        segmentation_group << task;
    }

    {
        // Quantification
        auto &task = t_builder.build<quantify_conidia>("quantify-conidia");
        segmentation_group >> task;
    }
}

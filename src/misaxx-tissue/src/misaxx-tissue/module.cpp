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

#include <misaxx-tissue/module.h>
#include "algorithms/segmentation2d/segmentation2d_klingberg_1.h"
#include "algorithms/segmentation3d/segmentation3d_none.h"
#include "algorithms/quantification.h"

using namespace misaxx;
using namespace misaxx::ome;
using namespace misaxx_tissue;

void module::create_blueprints(misa_dispatcher::blueprint_list &t_blueprint, misa_dispatcher::parameter_list &t_parameters) {

    m_segmentation2d_algorithm = t_parameters.create_algorithm_parameter<std::string>("segmentation2d");
//    m_segmentation3d_algorithm = t_parameters.create_algorithm_parameter<std::string>("segmentation3d");

    t_blueprint.add(create_blueprint_enum_parameter(m_segmentation2d_algorithm, {
        create_blueprint<segmentation2d_klingberg_1>("segmentation2d_klingberg_1")
    }, "segmentation2d_klingberg_1"));
//    t_blueprint.add(create_blueprint_enum_parameter(m_segmentation3d_algorithm, {
//            create_blueprint<segmentation3d_none>("segmentation3d_none")
//    }, "segmentation3d_none"));
    t_blueprint.add(create_blueprint<quantification>("quantification"));

}

void module::build(const misa_dispatcher::blueprint_builder &t_builder) {

    m_voxel_size = misaxx::ome::misa_ome_voxel_size(*m_input_autofluorescence.get_ome_metadata(), 0,
                                                   misaxx::ome::misa_ome_voxel_size::ome_unit_type::MICROMETER);

    group segmentation2d;
    for (auto &plane : this->m_input_autofluorescence) {
        auto &worker = t_builder.build<segmentation2d_base>(m_segmentation2d_algorithm.query());
        worker.m_input_autofluoresence = plane;
        worker.m_output_segmented2d = this->m_output_segmented.at(plane.get_plane_location());
        segmentation2d << worker;
    }

//    auto &segmentation3d = t_builder.build<segmentation3d_base>(m_segmentation3d_algorithm.query());
    auto &quant = t_builder.build<quantification>("quantification");
    chain workflow;
    workflow.add_dependency(segmentation2d);
    workflow >> quant;
}

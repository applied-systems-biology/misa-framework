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
#include <misaxx-microbench/module.h>
#include <src/misaxx-microbench/algorithms/microbench_task.h>

using namespace misaxx_microbench;

void module::create_blueprints(blueprint_list &t_blueprints, parameter_list &t_parameters) {
    t_blueprints.add(create_blueprint<microbench_task>("microbench"));
}

void module::build(const blueprint_builder &t_builder) {
    t_builder.build<microbench_task>("microbench");
}

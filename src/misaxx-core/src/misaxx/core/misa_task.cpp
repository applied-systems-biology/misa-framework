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

#include <misaxx/core/misa_task.h>

using namespace misaxx;

misa_task::misa_task(const misa_worker::node &t_node, const misa_worker::module &t_module) : misa_worker(t_node, t_module) {
    auto is_parallelizeable_path = t_node->get_algorithm_path()->get_path();
    is_parallelizeable_path.emplace_back("task::is_parallelizeable");
    auto schema = misaxx::parameter_registry::register_parameter(is_parallelizeable_path);
    schema->declare_optional<bool>(true)
            .document_title("Is Parallelizable")
            .document_description("If enabled, this task can be run in parallel");
    is_parallelizeable_parameter = misa_parameter<bool>(std::move(is_parallelizeable_path), std::move(schema));
}

void misa_task::simulate_work() {
}

void misa_task::execute_work() {
    if(misaxx::runtime_properties::is_simulating())
        simulate_work();
    else
        work();
}

bool misa_task::is_parallelizeable() const {
    return is_parallelizeable_parameter.query();
}

void misa_task::create_parameters(parameter_list &) {
}

const misa_parameter_builder &misa_task::get_parameters() const {
    return *m_parameter_builder;
}

void misa_task::prepare_work() {
    // Check if we actually need to create parameters
    if(!static_cast<bool>(m_parameter_builder)) {
        m_parameter_builder = std::make_unique<misa_parameter_builder>(*this);
        create_parameters(*m_parameter_builder);
    }
}



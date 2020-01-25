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

#include <misaxx/core/misa_parameter_builder.h>
#include <misaxx/core/misa_worker.h>

using namespace misaxx;

misa_parameter_builder::misa_parameter_builder(misa_worker &t_worker) : m_worker(t_worker.self()){

}

std::vector<std::string> misa_parameter_builder::get_algorithm_path() {
    return m_worker.lock()->get_node()->get_algorithm_path()->get_path();
}

std::vector<std::string> misa_parameter_builder::get_sample_path() {
    return m_worker.lock()->get_node()->get_sample_path()->get_path();
}

std::vector<std::string> misa_parameter_builder::get_node_path() {
    return m_worker.lock()->get_node()->get_global_path()->get_path();
}

std::vector<std::string> misa_parameter_builder::get_runtime_path() {
    return {{ "runtime" }};
}

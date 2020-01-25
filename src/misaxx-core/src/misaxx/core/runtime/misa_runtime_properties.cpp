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

#include <misaxx/core/runtime/misa_runtime_properties.h>
#include <misaxx/core/runtime/misa_runtime.h>
#include <misaxx/core/filesystem/misa_filesystem.h>

using namespace misaxx;

int misaxx::runtime_properties::get_num_threads() {
    return misa_runtime::instance().get_num_threads();
}

bool misaxx::runtime_properties::is_simulating() {
    return misa_runtime::instance().is_simulating();
}

bool misaxx::runtime_properties::requested_skipping() {
    return misa_runtime::instance().requests_skipping();
}

bool misaxx::runtime_properties::is_running() {
    return misa_runtime::instance().is_running();
}

std::shared_ptr<misa_work_node> misaxx::runtime_properties::get_root_node() {
    return misa_runtime::instance().get_root_node();
}

misa_filesystem misaxx::runtime_properties::get_root_filesystem() {
    return misa_runtime::instance().get_filesystem();
}

misa_module_info runtime_properties::get_module_info() {
    return misa_runtime::instance().get_module_info();
}

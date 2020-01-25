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

#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/misa_worker.h>
#include <misaxx/core/misa_dispatcher.h>
#include "misa_work_node_impl.h"

using namespace misaxx;

std::shared_ptr<misa_work_node>
misa_work_node::create_instance(const std::string &t_name, const std::shared_ptr<misa_work_node> &t_parent,
                                misa_work_node::instantiator_type t_instantiator) {
    return std::make_shared<misa_work_node_impl>(t_name, t_parent, std::move(t_instantiator));
}

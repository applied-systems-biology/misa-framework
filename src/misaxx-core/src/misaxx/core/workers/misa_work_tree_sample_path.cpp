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

#include <misaxx/core/workers/misa_work_tree_sample_path.h>
#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/misa_worker.h>
#include <misaxx/core/misa_root_module_base.h>

using namespace misaxx;

misa_work_tree_sample_path::misa_work_tree_sample_path(const std::shared_ptr<const misa_work_node> &t_node) {
    auto parent = t_node->get_parent().lock();
    if(!static_cast<bool>(parent)) {
        // Do nothing here
    }
    else if(static_cast<bool>(dynamic_cast<misa_root_module_base*>(parent->get_instance().get()))) {
        // If the parent is a root module, set the root of the path to the sample name
        m_node_path.push_back(parent);
        m_node_path.push_back(t_node);
        m_path.emplace_back("samples");
        m_path.push_back(t_node->get_name());
    }
    else {
        m_node_path = parent->get_sample_path()->m_node_path;
        m_path = parent->get_sample_path()->m_path;
    }
}

const std::vector<std::weak_ptr<const misa_work_node>> &misa_work_tree_sample_path::get_node_path() const {
    return m_node_path;
}

const std::vector<std::string> &misa_work_tree_sample_path::get_path() const {
    return m_path;
}

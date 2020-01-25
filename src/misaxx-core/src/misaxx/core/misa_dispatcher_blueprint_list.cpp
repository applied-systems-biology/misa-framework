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

#include <misaxx/core/misa_dispatcher_blueprint_list.h>
#include <misaxx/core/misa_dispatcher.h>

using namespace misaxx;

misa_dispatcher_blueprint_list::misa_dispatcher_blueprint_list(misa_dispatcher &t_worker) : m_worker(std::dynamic_pointer_cast<misa_dispatcher>(t_worker.self())) {
}

void misa_dispatcher_blueprint_list::add(misa_dispatcher_blueprint_list::blueprint t_blueprint) {
    m_blueprints[t_blueprint->get_name()] = std::move(t_blueprint);
}

void misa_dispatcher_blueprint_list::add(std::vector<misa_dispatcher_blueprint_list::blueprint> t_blueprints) {
    for(auto &blueprint : t_blueprints) {
        add(std::move(blueprint));
    }
}

misa_dispatcher_blueprint_list::blueprint misa_dispatcher_blueprint_list::get(const std::string &t_key) const {
    return m_blueprints.at(t_key);
}

const std::unordered_map<std::string, misa_dispatcher_blueprint_list::blueprint> &
misa_dispatcher_blueprint_list::get_entries() const {
    return m_blueprints;
}

misa_dispatcher &misa_dispatcher_blueprint_list::get_worker() const {
    return *m_worker.lock();
}

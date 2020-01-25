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
#include "misa_work_node_impl.h"

using namespace misaxx;

misa_work_node_impl::misa_work_node_impl(const std::string &t_name,
                               const std::shared_ptr<misa_work_node> &t_parent,
                               misa_work_node_impl::instantiator_type t_instantiator) : m_name(t_name),
                                                                                   m_parent(t_parent),
                                                                                   m_instantiator(std::move(t_instantiator)) {
}

const std::string &misa_work_node_impl::get_name() const {
    return m_name;
}

const std::weak_ptr<misa_work_node> misa_work_node_impl::get_parent() const {
    return m_parent;
}

bool misa_work_node_impl::is_parallelizeable() {
    return get_or_create_instance()->is_parallelizeable();
}

misa_work_subtree_status misa_work_node_impl::get_subtree_status() const {
    if(dynamic_cast<const misa_task*>(m_instance.get()) != nullptr) {
        // Tasks never build a subtree
        return misa_work_subtree_status ::complete;
    }
    if(m_status == misa_worker_status::done) {
        return misa_work_subtree_status ::complete;
    }
    else if(m_status == misa_worker_status::waiting) {
        for(const auto &child : m_instance->get_node()->get_children()) {
            if(child->get_subtree_status() != misa_work_subtree_status ::complete) {
                return misa_work_subtree_status::incomplete;
            }
        }
        return misa_work_subtree_status ::complete;
    }
    else {
        return misa_work_subtree_status::incomplete;
    }
}

misa_worker_status misa_work_node_impl::get_worker_status() const {
    // Check the status of the worker if it is waiting for subworkers
    if(m_status == misa_worker_status::waiting) {
        if(!static_cast<bool>(m_instance)) {
            throw std::logic_error("The worker is working without an instance!");
        }

        for(const auto &child : m_instance->get_node()->get_children()) {
            if(child->get_worker_status() != misa_worker_status::done) {
                return misa_worker_status::working;
            }
        }

        return misa_worker_status::done;
    }
    return m_status;
}

void misa_work_node_impl::repeat_work() {
    m_status = misa_worker_status ::queued_repeat;
}

void misa_work_node_impl::skip_work() {
    m_status = misa_worker_status ::done;
}

void misa_work_node_impl::prepare_work() {
    if(m_status == misa_worker_status::undone || m_status == misa_worker_status::queued_repeat) {
        m_status = misa_worker_status ::ready;
        get_or_create_instance()->prepare_work();
    }
    else {
        throw std::logic_error("Called prepare_work on invalid work node!");
    }
}

void misa_work_node_impl::work() {
    if(m_status != misa_worker_status::ready)
        throw std::runtime_error("Run prepare_work() before work()! Current status is " + std::to_string(static_cast<int>(m_status.load())));
    m_status = misa_worker_status ::working;
    auto instance = get_instance();
    instance->execute_work();
    if(m_status != misa_worker_status::queued_repeat) {
        // If the worker has no children, we can already declare that it is finished
        if(instance->get_node()->get_children().empty())
            m_status = misa_worker_status ::done;
        else
            m_status = misa_worker_status ::waiting;
    }
}

std::shared_ptr<misaxx::misa_worker> misa_work_node_impl::get_or_create_instance() {
    if(!m_instance) {
        m_instance = m_instantiator(self());
    }
    return m_instance;
}

std::shared_ptr<misa_work_node>
misa_work_node_impl::make_child(const std::string &t_name, misa_work_node_impl::instantiator_type t_instantiator) {
    auto ptr = std::make_shared<misa_work_node_impl>(t_name, self(), std::move(t_instantiator));
    m_children.push_back(ptr);
    return ptr;
}

std::vector<std::shared_ptr<misa_work_node>> &misa_work_node_impl::get_children() {
    return m_children;
}

const std::vector<std::shared_ptr<misa_work_node>> &misa_work_node_impl::get_children() const {
    return m_children;
}

std::unordered_set<std::shared_ptr<misa_work_node>> &misa_work_node_impl::get_dependencies() {
    return m_dependencies;
}

bool misa_work_node_impl::dependencies_satisfied() {
    for(auto &dep : m_dependencies) {
        if(dep->get_worker_status() != misa_worker_status::done)
            return false;
    }
    return true;
}

std::shared_ptr<misa_work_node> misa_work_node_impl::self() {
    return shared_from_this();
}

std::shared_ptr<const misa_work_node> misa_work_node_impl::self() const {
    return shared_from_this();
}

std::shared_ptr<const misa_work_tree_node_path> misa_work_node_impl::get_global_path() const {
    if(!static_cast<bool>(m_global_path)) {
        m_global_path = std::make_shared<misa_work_tree_node_path>(self());
    }
    return m_global_path;
}

std::shared_ptr<const misa_work_tree_algorithm_path> misa_work_node_impl::get_algorithm_path() const {
    if(!static_cast<bool>(m_algorithm_path)) {
        m_algorithm_path = std::make_shared<misa_work_tree_algorithm_path>(self());
    }
    return m_algorithm_path;
}

std::shared_ptr<const misa_work_tree_sample_path> misa_work_node_impl::get_sample_path() const {
    if(!static_cast<bool>(m_object_path)) {
        m_object_path = std::make_shared<misa_work_tree_sample_path>(self());
    }
    return m_object_path;
}

std::shared_ptr<misa_worker> misa_work_node_impl::get_instance() const {
    return m_instance;
}

const std::unordered_set<std::shared_ptr<misa_work_node>> &misa_work_node_impl::get_dependencies() const {
    return m_dependencies;
}

void misa_work_node_impl::set_nothread(bool value) {
    if(value) {
        if(m_status != misa_worker_status::ready)
            throw std::logic_error("set_nothread(true) called on work node that is not ready");
        m_status = misa_worker_status ::nothread;
    }
    else {
        if(m_status != misa_worker_status::nothread)
            throw std::logic_error("set_nothread(false) called on work node that is not nothread");
        m_status = misa_worker_status ::ready;
    }
}



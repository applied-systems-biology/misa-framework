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

#pragma once


#include <misaxx/core/workers/misa_work_node.h>

namespace misaxx {

    struct misa_work_node_impl : public misaxx::misa_work_node {
    public:

        explicit misa_work_node_impl(const std::string &t_name, const std::shared_ptr<misa_work_node> &t_parent, instantiator_type t_instantiator);

        /**
        * Gets the name of this node
        * @return
        */
        const std::string &get_name() const override;

        /**
         * Gets the parent of this node
         * @return
         */
        const std::weak_ptr<misa_work_node> get_parent() const override;

        /**
         * Gets the full path of this node
         * @return
         */
        std::shared_ptr<const misa_work_tree_node_path> get_global_path() const override;

        /**
         * Gets the algorithm path of this node
         * @return
         */
        std::shared_ptr<const misa_work_tree_algorithm_path> get_algorithm_path() const override;

        /**
         * Gets the object path of this node
         * @return
         */
        std::shared_ptr<const misa_work_tree_sample_path> get_sample_path() const override;

        /**
         * Returns true if this node's work can be parallelized
         * @return
         */
        bool is_parallelizeable() override;

        /**
         * Returns true if this node has all children. This means that the node iterator can stop watching this node for changes.
         * @return
         */
        misa_work_subtree_status get_subtree_status() const override;

        /**
         * Returns true if the node has no work to do
         * @return
         */
        misa_worker_status get_worker_status() const override;

        /**
         * Allows the worker instance to reject work.
         */
        void repeat_work() override;

        /**
         * Allows the runtime to skip the work (will be set to done)
         */
        void skip_work() override;

        /**
         * Called by the runtime to setup working.
         * Must be called before work()
         */
        void prepare_work() override;

        /**
         * Starts the actual work of this node
         * prepare_work() must be called beforehand
         * This function can be run in parallel
         */
        void work() override;

        void set_nothread(bool value) override;

        /**
         * Returns a pointer to the instance. If necessary, create the instance
         * @return
         */
        std::shared_ptr<misa_worker> get_or_create_instance() override;

        /**
         * Returns a pointer to the instance.
         * @return
         */
        std::shared_ptr<misa_worker> get_instance() const override;

        /**
         * Create a child node
         * @param t_name
         * @param t_instantiator
         * @return
         */
        std::shared_ptr<misa_work_node> make_child(const std::string &t_name, instantiator_type t_instantiator) override;

        /**
         * Returns the list of children
         * @return
         */
        std::vector<std::shared_ptr<misa_work_node>> &get_children() override;

        /**
         * Returns the list of children
         * @return
         */
        const std::vector<std::shared_ptr<misa_work_node>> &get_children() const override;

        /**
         * Returns the list of dependencies this node has
         * @return
         */
        std::unordered_set<std::shared_ptr<misa_work_node>> &get_dependencies() override;

        /**
        * Returns the list of dependencies this node has
        * @return
        */
        const std::unordered_set<std::shared_ptr<misa_work_node>> &get_dependencies() const override;

        /**
         * Returns true if all dependencies are satisfied
         * @return
         */
        bool dependencies_satisfied() override;

        /**
        * Returns a managed pointer to this node
        * @return
        */
        std::shared_ptr<misa_work_node> self() override;

        /**
        * Returns a managed pointer to this node
        * @return
        */
        std::shared_ptr<const misa_work_node> self() const override;

    private:

        /**
         * Name of this node
         */
        std::string m_name;

        /**
         * Pointer to the parent
         */
        std::weak_ptr<misa_work_node> m_parent;

        /**
         * Child nodes
         */
        std::vector<std::shared_ptr<misa_work_node>> m_children;

        /**
         * Nodes that are dependencies of this node
         */
        std::unordered_set<std::shared_ptr<misa_work_node>> m_dependencies;

        /**
         * Status of the worker
         */
        std::atomic<misa_worker_status> m_status = misa_worker_status::undone;

        /**
         * The actual worker instance
         */
        instance_ptr_type m_instance;

        /**
         * The instantiator responsible for creating the worker instance when requested
         */
        instantiator_type m_instantiator;

        mutable std::shared_ptr<misa_work_tree_node_path> m_global_path;

        mutable std::shared_ptr<misa_work_tree_algorithm_path> m_algorithm_path;

        mutable std::shared_ptr<misa_work_tree_sample_path> m_object_path;
    };

}
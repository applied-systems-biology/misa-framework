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

#include <string>
#include <memory>
#include <vector>
#include <misaxx/core/workers/misa_worker_status.h>
#include <unordered_set>
#include <any>
#include <misaxx/core/workers/misa_work_subtree_status.h>
#include <misaxx/core/misa_parameter_base.h>
#include <memory>
#include <mutex>
#include <misaxx/core/workers/misa_work_tree_node_path.h>
#include <misaxx/core/workers/misa_work_tree_algorithm_path.h>
#include <misaxx/core/workers/misa_work_tree_sample_path.h>
#include <misaxx/core/workers/misa_worker_status.h>
#include <functional>
#include <atomic>

namespace misaxx {

    struct misa_worker;

    /**
     * Internally used node in the task tree. A node is used to instantiate the actual worker.
     */
    class misa_work_node : public std::enable_shared_from_this<misa_work_node> {

    public:

        using instance_ptr_type = std::shared_ptr<misa_worker>;
        using instantiator_type = std::function<instance_ptr_type(const std::shared_ptr<misa_work_node> &)>;

        static std::shared_ptr<misa_work_node> create_instance(const std::string &t_name,
                                                               const std::shared_ptr<misa_work_node> &t_parent,
                                                               misa_work_node::instantiator_type t_instantiator);

        /**
         * Gets the name of this node
         * @return
         */
        virtual const std::string &get_name() const = 0;

        /**
         * Gets the parent of this node
         * @return
         */
        virtual const std::weak_ptr<misa_work_node> get_parent() const = 0;

        /**
         * Gets the full path of this node
         * @return
         */
        virtual std::shared_ptr<const misa_work_tree_node_path> get_global_path() const = 0;

        /**
         * Gets the algorithm path of this node
         * @return
         */
        virtual std::shared_ptr<const misa_work_tree_algorithm_path> get_algorithm_path() const = 0;

        /**
         * Gets the object path of this node
         * @return
         */
        virtual std::shared_ptr<const misa_work_tree_sample_path> get_sample_path() const = 0;

        /**
         * Returns true if this node's work can be parallelized
         * @return
         */
        virtual bool is_parallelizeable() = 0;

        /**
         * Returns true if this node has all children. This means that the node iterator can stop watching this node for changes.
         * @return
         */
        virtual misa_work_subtree_status get_subtree_status() const = 0;

        /**
         * Returns true if the node has no work to do
         * @return
         */
        virtual misa_worker_status get_worker_status() const = 0;

        /**
         * Allows the worker instance to reject work.
         */
        virtual void repeat_work() = 0;

        /**
         * Moves a ready work node into the status "nothread"
         */
        virtual void set_nothread(bool value) = 0;

        /**
         * Allows the runtime to skip the work (will be set to done)
         */
        virtual void skip_work() = 0;

        /**
         * Called by the runtime to setup working.
         * Must be called before work()
         */
        virtual void prepare_work() = 0;

        /**
         * Starts the actual work of this node
         * prepare_work() must be called beforehand
         * This function can be run in parallel
         */
        virtual void work() = 0;

        /**
         * Returns a pointer to the instance. If necessary, create the instance
         * @return
         */
        virtual std::shared_ptr<misa_worker> get_or_create_instance() = 0;

        /**
         * Returns a pointer to the instance.
         * @return
         */
        virtual std::shared_ptr<misa_worker> get_instance() const = 0;

        /**
         * Create a child node
         * @param t_name
         * @param t_instantiator
         * @return
         */
        virtual std::shared_ptr<misa_work_node>
        make_child(const std::string &t_name, instantiator_type t_instantiator) = 0;

        /**
         * Returns the list of children
         * @return
         */
        virtual std::vector<std::shared_ptr<misa_work_node>> &get_children() = 0;

        /**
         * Returns the list of children
         * @return
         */
        virtual const std::vector<std::shared_ptr<misa_work_node>> &get_children() const = 0;

        /**
         * Returns the list of dependencies this node has
         * @return
         */
        virtual std::unordered_set<std::shared_ptr<misa_work_node>> &get_dependencies() = 0;

        /**
        * Returns the list of dependencies this node has
        * @return
        */
        virtual const std::unordered_set<std::shared_ptr<misa_work_node>> &get_dependencies() const = 0;

        /**
         * Returns true if all dependencies are satisfied
         * @return
         */
        virtual bool dependencies_satisfied() = 0;

        /**
        * Returns a managed pointer to this node
        * @return
        */
        virtual std::shared_ptr<misa_work_node> self() = 0;

        /**
        * Returns a managed pointer to this node
        * @return
        */
        virtual std::shared_ptr<const misa_work_node> self() const = 0;


    };
}

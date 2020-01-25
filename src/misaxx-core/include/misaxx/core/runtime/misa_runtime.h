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

#include <memory>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <misaxx/core/misa_json_schema_property.h>
#include <misaxx/core/misa_module_info.h>

namespace misaxx {

    struct misa_runtime_impl;
    struct misa_cache;
    struct misa_runtime_log;
    struct misa_filesystem;
    struct misa_dispatcher;
    struct misa_module_interface;
    struct misa_work_node;

    struct misa_runtime {
    private:

        static misa_runtime *m_singleton;
        misa_runtime_impl *m_pimpl;

    public:

        misa_runtime();

        virtual ~misa_runtime();

        /**
          * Returns the number of threads
          * @return
          */
        int get_num_threads() const;

        /**
         * Returns true if the runtime is in simulation mode
         * @return
         */
        bool is_simulating() const;

        /**
         * Returns true if this runtime is currently working
         * @return
         */
        bool is_running() const;

        /**
         * If true, the runtime will write attachments
         * @return
         */
        bool is_writing_attachments() const;

        /**
         * If true, the runtime will write attachments lazily if attachment writing is enabled
         * @return
         */
        bool is_lazily_writing_attachments() const;

        /**
         * If true, a full-detailed runtime log is created
         * @return
         */
        bool is_creating_full_runtime_log() const;

        /**
         * Returns true if tasks should attempt to skip workloads
         * @return
         */
        bool requests_skipping() const;

        /**
         * Returns true if the runtime will create a worker graph
         * @return
         */
        bool is_creating_worker_graph() const;

        /**
         * Registers a cache into this runtime (e.g. used for attachment export)
         * @param t_cache
         */
        void register_cache(std::shared_ptr<misa_cache> t_cache);

        /**
         * Unregisters a cache
         * @param t_cache
         */
        bool unregister_cache(const std::shared_ptr<misa_cache> &t_cache);

        /**
       * Returns the list of registered caches
       * @return
       */
        const std::unordered_set<std::shared_ptr<misa_cache>> &get_registered_caches() const;

        /**
         * Returns the JSON that contains the parameters
         * @return
         */
        nlohmann::json &get_parameters();

        /**
          * Gets the raw JSON value of a path
          * @param t_path
          * @return
          */
        nlohmann::json get_parameter_value(const std::vector<std::string> &t_path) const;

        /**
         * Returns an optional instance to a schema builder.
         * If it returns nullptr, the schema builder will be ignored.
         * @return
         */
        std::shared_ptr<misa_json_schema_property> get_schema_builder();

        /**
         * Returns the runtime log
         * @return
         */
        misa_runtime_log &get_runtime_log();

        /**
         * Returns the root node
         * @return
         */
        std::shared_ptr<misa_work_node> get_root_node() const;

        /**
         * Returns the root node that is used if a schema is generated
         * @return
         */
        std::shared_ptr<misa_work_node> get_schema_root_node() const;

        /**
         * Returns the current filesystem
         * @return
         */
        misa_filesystem get_filesystem();

        /**
         * Returns the module info
         * @return
         */
        misa_module_info get_module_info();

    protected:

        /**
        * Enables or disables simulation mode
        * @param value
        */
        void set_is_simulating(bool value);

        /**
         * Sets the number of threads
         * @param threads
         */
        void set_num_threads(int threads);

        /**
         * Enabled/disabled writing attachments
         * @param value
         */
        void set_write_attachments(bool value);

        /**
         * Enabled/disables avoiding to write empty attachment files
         * @param value
         */
        void set_lazy_write_attachments(bool value);

        /**
         * Enables/disables creation of a full runtime log
         * @param value
         */
        void set_enable_full_runtime_log(bool value);

        /**
         * Enables/disables behavior to automatically skip work
         * @param value
         */
        void set_request_skipping(bool value);

        /**
         * Enables/disables creation of a worker graph
         * @param value
         */
        void set_create_worker_graph(bool value);

        /**
         * Sets the parameter JSON
         * @param t_json
         */
        void set_parameter_json(nlohmann::json t_json);

         /**
         * Sets the filesystem
         * @param t_filesystem
         */
        void set_filesystem(const misa_filesystem &t_filesystem);

        /**
         * Sets the module info of this runtime
         * @param info
         */
        void set_module_info(misa_module_info info);

        /**
         * Sets the root node
         * @param root
         */
        void set_root_node(std::shared_ptr<misa_work_node> root);

        /**
         * Sets the root node used for schema generation
         * @param schema_root
         */
        void set_schema_root_node(std::shared_ptr<misa_work_node> schema_root);

        /**
         * Prepares the runtime and runs the workload
         */
        virtual void prepare_and_run();

    public:

        /**
         * Global access to the current runtime instance
         * @return
         */
        static misa_runtime &instance(); // TODO: Get rid of global access. Instead, pass the runtime along
    };
}



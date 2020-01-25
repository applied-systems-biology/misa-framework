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

#include <misaxx/core/runtime/misa_runtime.h>
#include <misaxx/core/runtime/misa_runtime_log.h>
#include <misaxx/core/misa_module_interface.h>
#include <misaxx/core/filesystem/misa_filesystem_empty_importer.h>
#include <misaxx/core/filesystem/misa_filesystem_directories_importer.h>
#include <misaxx/core/filesystem/misa_filesystem_json_importer.h>
#include <iomanip>
#include <misaxx/core/utils/manual_stopwatch.h>
#include <omp.h>
#include <misaxx/core/utils/string.h>
#include <misaxx/core/misa_cached_data.h>
#include <misaxx/core/misa_worker.h>
#include <misaxx/core/misa_dispatcher.h>

using namespace misaxx;

///////////////////////////////////////////////////////////////////////
// Runtime PIMPL
///////////////////////////////////////////////////////////////////////

namespace {
    /**
     * Removes all children that are not "__OBJECT__" and ensures that "__OBJECT__" is present
     * @param entry
     */
    void prepare_make_filesystem_template(misaxx::filesystem::entry &entry) {
        std::vector<std::string> names;
        for(const auto &kv : *entry) {
            names.push_back(kv.first);
        }
        for(const auto &name : names) {
            if(name != "__OBJECT__") {
                entry->remove(name);
            }
        }
        entry->resolve({"__OBJECT__"});
    }

    void prepare_make_schema_template(misa_json_schema_property &property) {
        std::vector<std::string> names;
        for(const auto &kv : property.properties) {
            names.push_back(kv.first);
        }
        for(const auto &name : names) {
            if (name != "__OBJECT__") {
                property.properties.erase(name);
            }
        }
        property.resolve("__OBJECT__");
    }

    void write_workers_as_graph(const std::shared_ptr<const misa_work_node> &root, const boost::filesystem::path &path) {
        std::unordered_map<const misa_work_node*, std::string> labels;
        std::stack<const misa_work_node*> stack;
        stack.push(root.get());

        std::ofstream sw;
        sw.open(path.string());
        sw << "digraph workers {\n";

        // Declare all nodes
        while(!stack.empty()) {
            const misa_work_node* top = stack.top();
            stack.pop();

            auto it = labels.find(top);
            if(it == labels.end()) {
                if(dynamic_cast<const misaxx::misa_dispatcher*>(top->get_instance().get()) != nullptr) {
                    std::string name = top->get_name();
                    boost::replace_all(name, "\"", "\\\"");
                    std::string label = "D" + std::to_string(labels.size());

                    // Write full description
                    sw << label <<  "[label=\"" << name << "\", shape=folder];" << "\n";

                    labels[top] = label;
                }
                else {
                    std::string name = top->get_name();
                    boost::replace_all(name, "\"", "\\\"");
                    std::string label = "T" + std::to_string(labels.size());

                    // Write full description
                    sw << label <<  "[label=\"" << name << "\", shape=box];" << "\n";

                    labels[top] = label;
                }
            }

            for(const auto &ptr : top->get_children()) {
                stack.push(ptr.get());
            }
        }

        // Add dependencies
        stack.push(root.get());
        while(!stack.empty()) {
            const misa_work_node *top = stack.top();
            stack.pop();

            for(const auto &ptr : top->get_dependencies()) {
                sw << labels.at(ptr.get()) << " -> " << labels.at(top) << " [style=dashed]" << ";\n";
            }

            for(const auto &ptr : top->get_children()) {
                stack.push(ptr.get());
                sw << labels.at(ptr.get()) << " -> " << labels.at(top) << ";\n";
            }
        }

        sw << "}\n";
    }
}

namespace misaxx {
    struct misa_runtime_impl {
    public:

        misa_module_info m_module_info;

        std::shared_ptr<misa_work_node> m_root;

        std::shared_ptr<misa_work_node> m_schema_root;

        std::unordered_set<std::shared_ptr<misa_cache>> m_registered_caches;

        /**
         * List of nodes that are not completed and need to be watched
         */
        std::vector<misa_work_node *> m_nodes_todo;
        /**
         * List of nodes for fast lookup of unknown nodes
         */
        std::unordered_set<misa_work_node *> m_nodes_todo_lookup;

        // Variables are protected and will be set by the inheriting runtime

        bool m_is_simulating = false;

        /**
         * If true, tasks are requested to skip if the results already exist
         */
        bool m_requests_skipping = false;

        /**
       * Number of threads used by the runtime.
       * If the number is 1, the application will run in the current thread.
       */
        int m_num_threads = 1;

        /**
         * If true, write attachments
         */
        bool m_write_attachments = true;

        /**
         * If true, write only attachments with user-generated data
         * Otherwise, also writes "empty" attachments (containing description storage and location)
         */
        bool m_lazy_write_attachments = true;

        /**
         * If true, log the start and stop times of each worker
         */
        bool m_write_full_runtime_log = false;

        /**
         * If true, create a *.dot graph of the workers
         */
        bool m_create_worker_graph = false;

        /**
         * Runtime log
         */
        misa_runtime_log m_runtime_log;

        /**
         * Parameters for the workers
         */
        nlohmann::json m_parameters;

        /**
         * Used to create a configuration schema
         */
        std::shared_ptr<misa_json_schema_property> m_parameter_schema_builder;

        misa_runtime_impl();

        void run();

        bool is_running() {
            return !m_nodes_todo.empty();
        }
        
        misa_filesystem &get_filesystem() {
            if(!static_cast<bool>(m_root))
                throw std::runtime_error("No root module set!");
            return m_root->get_or_create_instance()->get_module()->filesystem;
        }
        
    private:

        size_t m_known_nodes_count = 0;

        size_t m_finished_nodes_count = 0;

        size_t m_last_waiting_announcement = 0;

        size_t m_last_rejecting_announcement = 0;

        bool m_tree_complete = false;

        void run_single_threaded();

        void run_parallel();

        void progress(const std::string &t_text);

        void progress(const misa_work_node &t_node, const std::string &t_text);

        void postprocess_caches();

        void postprocess_cache_attachments();

        void postprocess_parameter_schema();
    };

    misa_runtime_impl::misa_runtime_impl() : m_parameter_schema_builder(std::make_shared<misa_json_schema_property>()) {

    }

    void misa_runtime_impl::run() {
        misaxx::utils::manual_stopwatch stopwatch("Runtime");
        stopwatch.start();

        // Check if both schema- and main-root are existing
        if(!static_cast<bool>(m_root)) {
            throw std::runtime_error("Runtime has no root node!");
        }
        if(!static_cast<bool>(m_schema_root)) {
            throw std::runtime_error("Runtime has no schema root node!");
        }

        // The schema root works on the same filesystem as the main root
        // Note: misa_filesystem is a shared_ptr-like object
        m_schema_root->get_or_create_instance()->get_module()->filesystem = get_filesystem();

        // Clear separation between schema space and main space
        if(!m_is_simulating) {
            m_nodes_todo.push_back(m_root.get());
            m_nodes_todo_lookup.insert(m_root.get());
        }
        else {
            m_nodes_todo.push_back(m_schema_root.get());
            m_nodes_todo_lookup.insert(m_schema_root.get());
        }

        ++m_known_nodes_count;

        const bool enable_threading = m_num_threads > 1 && !m_is_simulating;

        if (!m_write_full_runtime_log) {
            for (int thread = 0; thread < m_num_threads; ++thread) {
                m_runtime_log.start(thread, "Undefined workload");
            }
        }

        if (enable_threading)
            run_parallel();
        else
            run_single_threaded();

        if (!m_write_full_runtime_log) {
            for (int thread = 0; thread < m_num_threads; ++thread) {
                m_runtime_log.stop(thread);
            }
        }

        // Postprocessing steps
        stopwatch.new_operation("Postprocessing");
        postprocess_caches();
        postprocess_cache_attachments();
        if (m_is_simulating) {
            postprocess_parameter_schema();
        }
        if (!m_is_simulating) {
            if (!boost::filesystem::exists( get_filesystem().exported->external_path())) {
                boost::filesystem::create_directories(get_filesystem().exported->external_path());
            }

            const auto parameters_path = get_filesystem().exported->external_path() / "parameters.json";
            const auto module_info_path = get_filesystem().exported->external_path() / "misa-module-info.json";

            // Write the parameter file
            std::cout << "<#> <#> Writing parameters to " << parameters_path << "\n";
            std::ofstream writer;
            writer.open(parameters_path.string());
            writer << std::setw(4) << m_parameters;
            writer.close();

            // Write module info
            std::cout << "<#> <#> Writing module info to " << module_info_path << "\n";
            writer.open(module_info_path.string());
            writer << std::setw(4) << nlohmann::json(m_module_info);
            writer.close();
        }
        if(m_create_worker_graph) {
            std::cout << "<#> <#> Writing worker graph as DOT file ... " << "\n";
            write_workers_as_graph(m_root, get_filesystem().exported->external_path() / "misa-workers.dot");
        }
        if (!m_is_simulating) {
            std::cout << "<#> <#> Building parameter schema for results folder ... " << "\n";

            // Write the parameter schema
            m_is_simulating = true;

            // Clear everything
            m_nodes_todo.clear();
            m_nodes_todo_lookup.clear();
            m_known_nodes_count = 0;
            m_parameter_schema_builder = std::make_shared<misa_json_schema_property>();
            m_finished_nodes_count = 0;
            m_tree_complete = false;

            // Set output paths
            const auto runtime_log_output_path =  get_filesystem().exported->external_path() / "runtime-log.json";
            const auto output_path =  get_filesystem().exported->external_path() / "parameter-schema.json";

            // Push the schema root into the tree
            m_nodes_todo.push_back(m_schema_root.get());
            m_nodes_todo_lookup.insert(m_schema_root.get());
            ++m_known_nodes_count;

            // Run the parameter schema workload
            run_single_threaded();

            postprocess_parameter_schema();

            {
                std::cout << "<#> <#> Writing parameter schema to " << output_path.string() << "\n";
                nlohmann::json j;
                m_parameter_schema_builder->to_json(j);
                std::ofstream out;
                out.open(output_path.string());
                out << std::setw(4) << j;
                out.close();
            }

            m_is_simulating = false;

            // Write the runtime log
            {
                std::cout << "<#> <#> Writing runtime log to " << runtime_log_output_path.string() << "\n";
                nlohmann::json j;
                m_runtime_log.to_json(j);
                std::ofstream out;
                out.open(runtime_log_output_path.string());
                out << std::setw(4) << j;
                out.close();
            }
        }

        stopwatch.stop();
    }

    void misa_runtime_impl::progress(const std::string &t_text) {
        if (m_tree_complete) {
            std::cout << "<" << static_cast<int>(m_finished_nodes_count * 1.0 / m_known_nodes_count * 100) << "%" << ">";
        } else {
            std::cout << "<#>";
        }
        std::cout << " " << "<" << m_finished_nodes_count << " / " << m_known_nodes_count << ">";

        std::cout << "\t" << t_text;
        std::cout << "\n";
    }

    void misa_runtime_impl::progress(const misa_work_node &t_node, const std::string &t_text) {
        if (m_tree_complete) {
            std::cout << "<" << static_cast<int>(m_finished_nodes_count * 1.0 / m_known_nodes_count * 100) << "%" << ">";
        } else {
            std::cout << "<#>";
        }
        std::cout << " " << "<" << m_finished_nodes_count << " / " << m_known_nodes_count << ">";
        std::cout << "\t" << t_text << " " << *t_node.get_global_path() << " @ " << static_cast<const void*>(&t_node);
        std::cout << "\n";
    }

    void misa_runtime_impl::run_single_threaded() {
        while (!m_nodes_todo.empty()) {

            size_t missing_dependency = 0;
            size_t rejected = 0;
            bool tree_complete = true;

            for (size_t i = 0; i < m_nodes_todo.size(); ++i) {
                auto *nd = m_nodes_todo[i];

                if (nd->get_subtree_status() != misa_work_subtree_status::complete)
                    tree_complete = false;

                // First check if we even can do work
                if (!nd->dependencies_satisfied()) {
                    ++missing_dependency;
                    continue;
                }

                auto subtree_before = nd->get_subtree_status();
                if (nd->get_worker_status() == misa_worker_status::undone ||
                    nd->get_worker_status() == misa_worker_status::queued_repeat) {
                    if (nd->get_worker_status() == misa_worker_status::queued_repeat) {
                        progress(*nd, "Retrying single-threaded work on");
                    } else {
                        progress(*nd, "Starting single-threaded work on");
                    }
                    if (m_write_full_runtime_log) {
                        m_runtime_log.start(0, misaxx::utils::to_string(*nd->get_global_path()));
                    }
                    nd->prepare_work();
                    nd->work();
                    if (m_write_full_runtime_log) {
                        m_runtime_log.stop(0);
                    }
                }
                if (nd->get_worker_status() == misa_worker_status::queued_repeat) {
                    // If the work was rejected, don't do any additional steps afterwards
                    ++rejected;
                    continue;
                }
                if (subtree_before == misa_work_subtree_status::incomplete) {
                    // Look for new nodes to visit
                    for (auto &child : nd->get_children()) {
                        if (child->get_worker_status() == misa_worker_status::undone &&
                            m_nodes_todo_lookup.find(child.get()) == m_nodes_todo_lookup.end()) {
                            m_nodes_todo_lookup.insert(child.get());
                            m_nodes_todo.push_back(child.get());
                            ++m_known_nodes_count;
                        }
                    }
                }
                if (nd->get_worker_status() == misa_worker_status::done &&
                    nd->get_subtree_status() == misa_work_subtree_status::complete) {
                    // Use quick-delete to remove the completed node
                    std::swap(m_nodes_todo[i], m_nodes_todo[m_nodes_todo.size() - 1]);
                    m_nodes_todo.resize(m_nodes_todo.size() - 1);
                    --i;
                    ++m_finished_nodes_count;
                    progress(*nd, "Work finished on");
                }
            }

            m_tree_complete |= tree_complete;

            if (missing_dependency > 0 && missing_dependency != m_last_waiting_announcement) {
                progress("Info: " + std::to_string(missing_dependency) + " workers are waiting for dependencies");
                m_last_waiting_announcement = missing_dependency;
            }
            if (rejected > 0 && rejected != m_last_rejecting_announcement) {
                progress("Info: " + std::to_string(rejected) + " workers are rejecting to work");
                m_last_rejecting_announcement = rejected;
            }
        }
    }

    void misa_runtime_impl::run_parallel() {
        omp_set_num_threads(this->m_num_threads);

        #pragma omp parallel
        {
            #pragma omp master
            {
                progress("Runtime dispatcher started in thread " + std::to_string(omp_get_thread_num()));
                const int master_thread_id = omp_get_thread_num();
                while (!m_nodes_todo.empty()) {

                    size_t missing_dependency = 0;
                    size_t rejected = 0;
                    bool tree_complete = true;

                    for (size_t i = 0; i < m_nodes_todo.size(); ++i) {
                        auto *nd = m_nodes_todo[i];

                        if (nd->get_subtree_status() != misa_work_subtree_status::complete)
                            tree_complete = false;

                        // First check if we even can do work
                        if (!nd->dependencies_satisfied()) {
                            ++missing_dependency;
                            continue;
                        }

                        auto subtree_before = nd->get_subtree_status();
                        if (nd->get_worker_status() == misa_worker_status::undone ||
                            nd->get_worker_status() == misa_worker_status ::nothread ||
                            nd->get_worker_status() == misa_worker_status::queued_repeat) {

                            if (!nd->is_parallelizeable()) {
                                if (nd->get_worker_status() == misa_worker_status::queued_repeat) {
                                    progress(*nd, "Retrying single-threaded work on");
                                } else {
                                    progress(*nd, "Starting single-threaded work on");
                                }
                                if (m_write_full_runtime_log) {
                                    m_runtime_log.start(0, misaxx::utils::to_string(*nd->get_global_path()));
                                }
                                if(nd->get_worker_status() != misa_worker_status::ready &&
                                    nd->get_worker_status() != misa_worker_status::nothread) {
                                    nd->prepare_work();
                                }

                                nd->work();
                                if (m_write_full_runtime_log) {
                                    m_runtime_log.stop(0);
                                }
                            } else {
                                switch(nd->get_worker_status()) {
                                    case misa_worker_status ::nothread:
                                        break;
                                    case misa_worker_status::queued_repeat:
                                        progress(*nd, "Retrying parallelized work on");
                                        break;
                                    case misa_worker_status::undone:
                                    case misa_worker_status::ready:
                                    case misa_worker_status::working:
                                    case misa_worker_status::done:
                                    case misa_worker_status::waiting:
                                    default:
                                        progress(*nd, "Starting parallelized work on");
                                }

                                // Wake the worker up
                                if(nd->get_worker_status() == misa_worker_status::nothread) {
                                    nd->set_nothread(false);
                                }

                                // Prepare work if not ready or in nothread mode
                                if(nd->get_worker_status() != misa_worker_status::ready) {
                                    nd->prepare_work();
                                }

                                #pragma omp task firstprivate(nd) firstprivate(master_thread_id)
                                {
                                    if(omp_get_thread_num() == master_thread_id) {
                                        nd->set_nothread(true);
                                    }
                                    else {
                                        if (m_write_full_runtime_log) {
                                            m_runtime_log.start(omp_get_thread_num(),
                                                                misaxx::utils::to_string(*nd->get_global_path()));
                                        }
                                        nd->work();
                                        if (m_write_full_runtime_log) {
                                            m_runtime_log.stop(omp_get_thread_num());
                                        }
                                    }
                                }
                            }
                        }
                        if (nd->get_worker_status() == misa_worker_status::queued_repeat) {
                            // If the work was rejected, don't do any additional steps afterwards
                            ++rejected;
                            continue;
                        }
                        if (subtree_before == misa_work_subtree_status::incomplete) {
                            // Look for new nodes to visit
                            for (auto &child : nd->get_children()) {
                                if (child->get_worker_status() == misa_worker_status::undone &&
                                    m_nodes_todo_lookup.find(child.get()) == m_nodes_todo_lookup.end()) {
                                    m_nodes_todo_lookup.insert(child.get());
                                    m_nodes_todo.push_back(child.get());
                                    ++m_known_nodes_count;
                                }
                            }
                        }
                        if (nd->get_worker_status() == misa_worker_status::done &&
                            nd->get_subtree_status() == misa_work_subtree_status::complete) {
                            // Use quick-delete to remove the completed node
                            std::swap(m_nodes_todo[i], m_nodes_todo[m_nodes_todo.size() - 1]);
                            m_nodes_todo.resize(m_nodes_todo.size() - 1);
                            --i;
                            ++m_finished_nodes_count;
                            progress(*nd, "Work finished on");
                        }
                    }

                    m_tree_complete |= tree_complete;

                    if (missing_dependency > 0 && missing_dependency != m_last_waiting_announcement) {
                        progress("Info: " + std::to_string(missing_dependency) +
                                 " workers are waiting for dependencies");
                        m_last_waiting_announcement = missing_dependency;
                    }
                    if (rejected > 0 && rejected != m_last_rejecting_announcement) {
                        progress("Info: " + std::to_string(rejected) + " workers are rejecting to work");
                        m_last_rejecting_announcement = rejected;
                    }
                }
                progress("Runtime dispatcher thread ended");
            }
        }
    }

    void misa_runtime_impl::postprocess_caches() {
        if (!m_is_simulating) {
            std::cout << "[Caches] Post-processing caches ..." << "\n";
            if (!m_write_full_runtime_log) {
                m_runtime_log.start(0, "Postprocessing");
            }
            for (const std::shared_ptr<misa_cache> &ptr : m_registered_caches) {
                if (m_write_full_runtime_log) {
                    m_runtime_log.start(0, "Postprocessing " + ptr->get_location().string() + " (" +
                                           ptr->get_unique_location().string() + ")");
                }
                std::cout << "[Caches] Post-processing cache " << ptr->get_location() << " (" << ptr->get_unique_location()
                          << ")" << "\n";
                ptr->postprocess();
                if (ptr->has_data()) {
                    std::cout << "[Caches] Info: " << ptr->get_location() << " (" << ptr->get_unique_location()
                              << ")" << " reports that it still contains data" << "\n";
                }
                if (m_write_full_runtime_log) {
                    m_runtime_log.stop(0);
                }
            }
            if (!m_write_full_runtime_log) {
                m_runtime_log.stop(0);
            }
        }
    }

    void misa_runtime_impl::postprocess_cache_attachments() {
        if (!m_write_attachments) {
            std::cout << "[Attachments] Post-processing attachments ... Skipped" << "\n";
            return;
        }

        std::cout << "[Attachments] Post-processing attachments ..." << "\n";

        if (!m_write_full_runtime_log) {
            m_runtime_log.start(0, "Attachments");
        }

        // Attachment JSON schemata
        nlohmann::json attachment_schemata;

        for (const std::shared_ptr<misa_cache> &ptr : m_registered_caches) {

            if (ptr->get_unique_location().empty())
                continue;

            if (!m_is_simulating) {

                if (m_write_full_runtime_log) {
                    m_runtime_log.start(0, "Attachments: " + ptr->get_location().string() + " (" +
                                           ptr->get_unique_location().string() + ")");
                }

                readonly_access<typename misa_cached_data_base::attachment_type> access(ptr->attachments); // Open the cache

                if (m_lazy_write_attachments && access.get().empty()) {
                    continue;
                }

                std::cout << "[Attachments] Post-processing attachment " << ptr->get_location() << " ("
                          << ptr->get_unique_location() << ")" << "\n";

                boost::filesystem::path filesystem_unique_link_path = ptr->get_internal_unique_location();
                boost::filesystem::path filesystem_generic_link_path = ptr->get_internal_location();

                // Replace extension with JSON
                const boost::filesystem::path filesystem_export_base_path = get_filesystem().exported->external_path();
                boost::filesystem::path cache_attachment_path =
                        (filesystem_export_base_path / "attachments" / filesystem_unique_link_path).string() + ".json";
                boost::filesystem::create_directories(cache_attachment_path.parent_path());

                nlohmann::json exported_json = nlohmann::json(nlohmann::json::object());

                for (const auto &kv : access.get()) {
                    const std::unique_ptr<misa_serializable> &attachment_ptr = kv.second;

                    if (!cache_attachment_path.empty()) {
                        // Export the attachment as JSON
                        attachment_ptr->to_json(exported_json[attachment_ptr->get_serialization_id().get_id()]);

                        // Export attachment JSON schema
                        if (attachment_schemata.find(attachment_ptr->get_serialization_id().get_id()) ==
                            attachment_schemata.end()) {
                            auto schema = std::make_shared<misa_json_schema_property>();
                            attachment_ptr->to_json_schema(*schema);
                            schema->to_json(attachment_schemata[attachment_ptr->get_serialization_id().get_id()]);
                        }
                    }
                }

                // Attach the location
                ptr->get_location_interface()->to_json(exported_json["location"]);

                // Add schema for location type if needed
                if(attachment_schemata.find( ptr->get_location_interface()->get_serialization_id().get_id()) ==
                   attachment_schemata.end()) {
                    auto schema = std::make_shared<misa_json_schema_property>();
                    ptr->get_location_interface()->to_json_schema(*schema);
                    schema->to_json(attachment_schemata[ptr->get_location_interface()->get_serialization_id().get_id()]);
                }

                // Attach the description storage if needed
                if (!access.get().has<misa_description_storage>()) {
                    misa_location link(ptr->get_internal_location(), filesystem_generic_link_path, filesystem_unique_link_path);
                    ptr->describe()->set_location(ptr->get_location_interface());
                    ptr->describe()->to_json(exported_json[ptr->describe()->get_serialization_id().get_id()]);

                    // Add schema for description storage if needed
                    if(attachment_schemata.find( ptr->describe()->get_serialization_id().get_id()) ==
                       attachment_schemata.end()) {
                        auto schema = std::make_shared<misa_json_schema_property>();
                        ptr->describe()->to_json_schema(*schema);
                        schema->to_json(attachment_schemata[ptr->describe()->get_serialization_id().get_id()]);
                    }
                }

                // Add schemata for pattern & description
                if(ptr->describe()->has_pattern()) {
                    const misa_data_pattern &descr = ptr->describe()->get<misa_data_pattern>();
                    if(attachment_schemata.find( descr.get_serialization_id().get_id()) ==
                       attachment_schemata.end()) {
                        auto schema = std::make_shared<misa_json_schema_property>();
                        descr.to_json_schema(*schema);
                        schema->to_json(attachment_schemata[descr.get_serialization_id().get_id()]);
                    }
                }
                if(ptr->describe()->has_description()) {
                    const misa_data_description &descr = ptr->describe()->get<misa_data_description>();
                    if(attachment_schemata.find( descr.get_serialization_id().get_id()) ==
                       attachment_schemata.end()) {
                        auto schema = std::make_shared<misa_json_schema_property>();
                        descr.to_json_schema(*schema);
                        schema->to_json(attachment_schemata[descr.get_serialization_id().get_id()]);
                    }
                }

                if (!m_is_simulating) {
                    std::ofstream sw;
                    sw.open(cache_attachment_path.string());
                    sw << std::setw(4) << exported_json;
                }

                if (m_write_full_runtime_log) {
                    m_runtime_log.stop(0);
                }
            }
        }

        // Write attachment serialization IDs
        if (!m_is_simulating) {
            const boost::filesystem::path filesystem_export_base_path = get_filesystem().exported->external_path();
            std::ofstream sw;
            sw.open((filesystem_export_base_path / "attachments" / "serialization-schemas.json").string());
            sw << std::setw(4) << attachment_schemata;
        }

        if (!m_write_full_runtime_log) {
            m_runtime_log.stop(0);
        }
    }

    void misa_runtime_impl::postprocess_parameter_schema() {

//        // DEBUG: Filesystem structure
//        for(const auto &entry : get_filesystem().imported->traverse()) {
//            std::cout << "IMPORTED: " << entry->internal_path() << std::endl;
//        }
//        for(const auto &entry : get_filesystem().exported->traverse()) {
//            std::cout << "EXPORTED: " << entry->internal_path() << std::endl;
//        }

        // Save filesystem to parameter schema
        prepare_make_filesystem_template(get_filesystem().imported);
        prepare_make_filesystem_template(get_filesystem().exported);
        get_filesystem().to_json_schema(*(m_parameter_schema_builder->resolve("filesystem")->resolve("json-data")));
        (*m_parameter_schema_builder)["filesystem"]["source"].define<std::string>("json");

        // Workaround: Due to inflexibility with schema generation, manually put "__OBJECT__" nodes into list builders
        // /properties/algorithm -> nothing to do
        // /properties/objects/properties/__OBJECT__ -> /properties/objects/additionalProperties
        prepare_make_schema_template( (*m_parameter_schema_builder)["samples"]);
        (*m_parameter_schema_builder)["samples"].make_template();

        // /properties/runtime::filesystem/properties/json-data/properties/imported/properties/children/properties/__OBJECT__ -> /properties/runtime::filesystem/properties/json-data/properties/imported/properties/children/additionalProperties
        prepare_make_schema_template((*m_parameter_schema_builder)["filesystem"]["json-data"]["imported"]["children"]);
        (*m_parameter_schema_builder)["filesystem"]["json-data"]["imported"]["children"].make_template();
        prepare_make_schema_template((*m_parameter_schema_builder)["filesystem"]["json-data"]["exported"]["children"]);
        (*m_parameter_schema_builder)["filesystem"]["json-data"]["exported"]["children"].make_template();

        // Write runtime parameters
        (*m_parameter_schema_builder)["runtime"]["num-threads"].document_title("Number of threads")
                .document_description("Changes the number of threads")
                .declare_optional<int>(1);
        (*m_parameter_schema_builder)["runtime"]["request-skipping"].document_title("Skip existing results")
                .document_description("Informs algorithms that existing results should not be overwritten")
                .declare_optional<bool>(false);
        (*m_parameter_schema_builder)["runtime"]["write-worker-graph"].document_title("Export workers as graph")
                .document_description("Creates a file 'misa-workers.dot' that shows the DAG of workers")
                .declare_optional<bool>(false);
        (*m_parameter_schema_builder)["runtime"]["full-runtime-log"].document_title("Full runtime log")
                .document_description("If enabled, the runtime log will contain all individual workers")
                .declare_optional(false);
        (*m_parameter_schema_builder)["runtime"]["postprocessing"]["write-attachments"].document_title("Write attachments")
                .document_description("If enabled, write data attached to caches after the work is done")
                .declare_optional(true);
        (*m_parameter_schema_builder)["runtime"]["postprocessing"]["lazy-write-attachments"].document_title("Only write attachments with actual data")
                .document_description("If enabled, only non-empty attachment files will be written")
                .declare_optional(true);
    }


}

namespace {
    void load_filesystem(misaxx::misa_runtime_impl &runtime) {
        // Do no importing if we build a schema
        if(runtime.m_is_simulating) {
            misa_filesystem_empty_importer importer;
            runtime.get_filesystem() = importer.import();
            return;
        }

        // Otherwise load importer from parameters
        nlohmann::json &params = runtime.m_parameters["filesystem"];
        if(params["source"] == "directories") {
            misa_filesystem_directories_importer importer;
            importer.input_path = params["input-directory"].get<std::string>();
            importer.output_path = params["output-directory"].get<std::string>();
            runtime.get_filesystem() = importer.import();
        }
        else if(params["source"] == "json") {
            misa_filesystem_json_importer importer;
            if(params.find("json-data") != params.end()) {
                importer.input_json = params["json-data"];
            }
            else {
                importer.json_path = params["json-path"].get<std::string>();
            }
            runtime.get_filesystem() = importer.import();
        }
        else {
            throw std::runtime_error("Unknown filesystem type " + params["source"].get<std::string>());
        }
    }
}

///////////////////////////////////////////////////////////////////////
// Runtime interface
///////////////////////////////////////////////////////////////////////

misa_runtime * misaxx::misa_runtime::m_singleton = nullptr;

misaxx::misa_runtime::misa_runtime() : m_pimpl(new misa_runtime_impl()) {
    // The current runtime is now
    // TODO: Get rid of this.
    misa_runtime::m_singleton = this;
}

misaxx::misa_runtime::~misa_runtime() {
    delete m_pimpl;
}

int misaxx::misa_runtime::get_num_threads() const {
    return m_pimpl->m_num_threads;
}

bool misaxx::misa_runtime::is_simulating() const {
    return m_pimpl->m_is_simulating;
}

bool misaxx::misa_runtime::is_running() const {
    return m_pimpl->is_running();
}

bool misa_runtime::requests_skipping() const {
    return m_pimpl->m_requests_skipping;
}

bool misaxx::misa_runtime::is_writing_attachments() const {
    return m_pimpl->m_write_attachments;
}

bool misaxx::misa_runtime::is_lazily_writing_attachments() const {
    return m_pimpl->m_lazy_write_attachments;
}

bool misa_runtime::is_creating_full_runtime_log() const {
    return m_pimpl->m_write_full_runtime_log;
}

bool misa_runtime::is_creating_worker_graph() const {
    return m_pimpl->m_create_worker_graph;
}

std::shared_ptr<misa_json_schema_property> misa_runtime::get_schema_builder() {
    return m_pimpl->m_parameter_schema_builder;
}

misa_runtime_log &misa_runtime::get_runtime_log() {
    return m_pimpl->m_runtime_log;
}

std::shared_ptr<misa_work_node> misa_runtime::get_root_node() const {
    return m_pimpl->m_root;
}

std::shared_ptr<misa_work_node> misa_runtime::get_schema_root_node() const {
    return m_pimpl->m_schema_root;
}

misa_filesystem misa_runtime::get_filesystem() {
    return m_pimpl->get_filesystem();
}

misa_module_info misa_runtime::get_module_info() {
    return m_pimpl->m_module_info;
}

void misaxx::misa_runtime::register_cache(std::shared_ptr<misaxx::misa_cache> t_cache) {
    m_pimpl->m_registered_caches.insert(std::move(t_cache));
}

bool misaxx::misa_runtime::unregister_cache(const std::shared_ptr<misaxx::misa_cache> &t_cache) {
    if (m_pimpl->m_registered_caches.count(t_cache) > 0) {
        m_pimpl->m_registered_caches.erase(t_cache);
        return true;
    }
    return false;
}

const std::unordered_set<std::shared_ptr<misaxx::misa_cache>> &misaxx::misa_runtime::get_registered_caches() const {
    return m_pimpl->m_registered_caches;
}

nlohmann::json &misaxx::misa_runtime::get_parameters() {
    return m_pimpl->m_parameters;
}

nlohmann::json misaxx::misa_runtime::get_parameter_value(const std::vector<std::string> &t_path) const {
    nlohmann::json *result = &m_pimpl->m_parameters;
    for(const auto &key : t_path) {
        if(result->find(key) == result->end()) {
            return nlohmann::json { };
        }

        result = &result->at(key);
    }
    return *result;
}

void misa_runtime::set_is_simulating(bool value) {
    if (is_running())
        throw std::runtime_error("Cannot change simulation mode while the runtime is working!");
    m_pimpl->m_is_simulating = value;
}

void misa_runtime::set_num_threads(int threads) {
    if (is_running())
        throw std::runtime_error("Cannot change the number of threads while the runtime is working!");
    if (threads < 1)
        throw std::runtime_error("Invalid number of threads!");
    m_pimpl->m_num_threads = threads;
}

void misa_runtime::set_write_attachments(bool value) {
    if (is_running())
        throw std::runtime_error("Cannot change runtime properties while the runtime is working!");
    m_pimpl->m_write_attachments = value;
}

void misa_runtime::set_lazy_write_attachments(bool value) {
    if (is_running())
        throw std::runtime_error("Cannot change runtime properties while the runtime is working!");
    m_pimpl->m_lazy_write_attachments = value;
}

void misa_runtime::set_enable_full_runtime_log(bool value) {
    if (is_running())
        throw std::runtime_error("Cannot change runtime properties while the runtime is working!");
    m_pimpl->m_write_full_runtime_log = value;
}

void misa_runtime::set_request_skipping(bool value) {
    if (is_running())
        throw std::runtime_error("Cannot change runtime properties while the runtime is working!");
    m_pimpl->m_requests_skipping = value;
}

void misa_runtime::set_create_worker_graph(bool value) {
    if (is_running())
        throw std::runtime_error("Cannot change runtime properties while the runtime is working!");
    m_pimpl->m_create_worker_graph = value;
}

void misa_runtime::set_parameter_json(nlohmann::json t_json) {
    if (is_running())
        throw std::runtime_error("Cannot change runtime properties while the runtime is working!");
    m_pimpl->m_parameters = std::move(t_json);
}

void misa_runtime::set_filesystem(const misa_filesystem &t_filesystem) {
    if (is_running())
        throw std::runtime_error("Cannot change runtime properties while the runtime is working!");
    m_pimpl->get_filesystem() = t_filesystem;
}

void misa_runtime::set_module_info(misa_module_info info) {
    if (is_running())
        throw std::runtime_error("Cannot change runtime properties while the runtime is working!");
    m_pimpl->m_module_info = std::move(info);
}

void misa_runtime::set_root_node(std::shared_ptr<misa_work_node> root) {
    if (is_running())
        throw std::runtime_error("Cannot change runtime properties while the runtime is working!");
    if(static_cast<bool>(m_pimpl->m_root)) {
        // Recover the filesystem
        misa_filesystem fs = m_pimpl->get_filesystem();
        m_pimpl->m_root = std::move(root);
        set_filesystem(fs);
    }
    else {
        m_pimpl->m_root = std::move(root);
    }
}

void misa_runtime::set_schema_root_node(std::shared_ptr<misa_work_node> schema_root) {
    if (is_running())
        throw std::runtime_error("Cannot change runtime properties while the runtime is working!");
    // Reset the filesystem. It will be shared with the main filesystem
    schema_root->get_or_create_instance()->get_module()->filesystem = misa_filesystem {};
    m_pimpl->m_schema_root = std::move(schema_root);
}

void misaxx::misa_runtime::prepare_and_run() {
    load_filesystem(*m_pimpl);
    m_pimpl->run();
}

misa_runtime &misa_runtime::instance() {
    return *m_singleton;
}












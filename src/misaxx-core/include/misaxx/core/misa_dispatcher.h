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

#include <misaxx/core/misa_dispatcher.h>
#include <misaxx/core/workers/misa_work_dependency_chain.h>
#include <misaxx/core/workers/misa_work_dependency_group.h>
#include <misaxx/core/misa_worker.h>
#include <misaxx/core/misa_dispatch_blueprint.h>
#include <misaxx/core/workers/misa_work_node.h>
#include <misaxx/core/misa_dispatcher_builder.h>
#include <misaxx/core/utils/ref.h>

namespace misaxx {

    /**
     * A dispatcher is a worker that is only responsible for creating tasks or dispatching sub-dispatchers.
     * It should not do any work.
     */
    struct misa_dispatcher : public misa_worker {

        using chain = misa_work_dependency_chain;
        using group = misa_work_dependency_group;
        using blueprint_list = misa_dispatcher_blueprint_list;
        using parameter_list = misa_parameter_builder;
        using blueprint_builder = misa_dispatcher_builder;
        using blueprint = std::shared_ptr<misa_dispatch_blueprint_base>;
        template<typename T> using parameter = misa_parameter<T>;

        misa_dispatcher(const node &t_node, const module &t_module);

    protected:

        /**
         * Creates a blueprint for dispatching a task or sub-dispatcher
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance>
        blueprint create_blueprint(const std::string &t_name) {
            static_assert(!std::is_base_of<misa_module_interface, Instance>::value, "This function does not support submodules! Use create_submodule_blueprint() instead.");
            auto result = std::make_shared<misa_dispatch_blueprint<Instance>>();
            result->name = t_name;
            result->function = [t_name](misa_dispatcher &t_worker) -> Instance & {
                return t_worker.template dispatch_instance<Instance>(t_name, t_worker.get_module());
            };
            return result;
        }

        /**
         * Creates a blueprint that instantiates a submodule
         * @param t_submodule
         * @return
         */
        template<class Instance, class Interface = typename Instance::module_interface_type>
        blueprint create_submodule_blueprint(const std::string &t_name, Interface t_interface = Interface()) {
            static_assert(std::is_base_of<misa_module_interface, Instance>::value, "This function only supports submodules! Use create_blueprint() instead.");

            // Modules need to be initialized immediately
            Instance &instance = dispatch_instance<Instance>(t_name, std::move(t_interface));
            // Create a sub-filesystem within the current module
            instance.filesystem = get_module()->filesystem.create_subsystem(t_name);
            instance.setup();

            // The blueprint just works on a pointer
            Instance *instance_ptr = &instance;

            auto result = std::make_shared<misa_dispatch_blueprint<Instance>>();
            result->name = t_name;
            result->allow_multi_instancing = false;
            result->function = [instance_ptr](misa_dispatcher &t_worker) -> Instance& {
                return *instance_ptr;
            };

            return result;
        }

        /**
        * Creates a blueprint that instantiates a submodule
        * This overload is able to take a pointer to an module interface and take over the contents to the newly created blueprint.
        * The interface pointer is redirected to the created module instance.
        * @param t_submodule
        * @return
        */
        template<class Instance, class Interface = typename Instance::module_interface_type>
        blueprint create_submodule_blueprint(const std::string &t_name, std::shared_ptr<Interface> &t_interface) {
            static_assert(std::is_base_of<misa_module_interface, Instance>::value, "This function only supports submodules! Use create_blueprint() instead.");

            // Modules need to be initialized immediately
            Instance &instance = dispatch_instance<Instance>(t_name, std::move(*t_interface));

            // Redirect the shared pointer to the instance
            t_interface = instance.template get_module_as<Interface>();

            // Create a sub-filesystem within the current module
            instance.filesystem = get_module()->filesystem.create_subsystem(t_name);
            instance.setup();

            // The blueprint just works on a pointer
            Instance *instance_ptr = &instance;

            auto result = std::make_shared<misa_dispatch_blueprint<Instance>>();
            result->name = t_name;
            result->allow_multi_instancing = false;
            result->function = [instance_ptr](misa_dispatcher &t_worker) -> Instance& {
                return *instance_ptr;
            };

            return result;
        }

        /**
         * Exports a list of blueprints as enum parameter
         * @param t_parameter Parameter to be modified
         * @param t_blueprints
         * @param t_default
         * @return
         */
        std::vector<blueprint> create_blueprint_enum_parameter(misa_parameter<std::string> &t_parameter,
                                                               std::vector<blueprint> t_blueprints,
                                                               const std::optional<std::string> &t_default = std::nullopt);

    public:

        /**
         * Inherited from the base class. You can use create_blueprint instead.
         * @param t_parameters
         */
        void create_parameters(misa_parameter_builder &t_parameters) override;

        /**
         * This method should be overriden to create the blueprint of the tree created by this dispatcher
         * @param t_blueprint
         */
        virtual void create_blueprints(blueprint_list &t_blueprints, parameter_list &t_parameters) = 0;

        /**
         * This method creates the tree based on the blueprint
         * @param t_builder
         */
        virtual void build(const blueprint_builder &t_builder) = 0;

        /**
         * Builds a simulated tree based on the blueprint
         */
        virtual void build_simulation(const blueprint_builder &t_builder);

        /**
         * Called by the runtime
         * Always runs in the main thread
         */
        void prepare_work() override;

        /**
         * Called by the runtime to execute the workload
         * Always runs in the main thread
         */
        void execute_work() override;

        /**
         * Returns always false
         * @return
         */
        bool is_parallelizeable() const override;

        /**
         * Returns the parameter builder
         * @return
         */
        const misa_parameter_builder &get_parameters() const override;

    private:

        /**
         * Builder that contains a blueprint of the tree that is created by this dispatcher
         */
        std::unique_ptr<misa_dispatcher_builder> m_builder;

        /**
         * Builde that allows creating & accessing parameters
         */
        std::unique_ptr<misa_parameter_builder> m_parameter_builder;


        /**
         * Basic dispatch function that instantiates a task or dispatcher.
         * Although already instantiated, the runtime will ensure that dependencies are satisfied before work() is called.
         * @tparam Instance
         * @param t_name
         * @return
         */
        template<class Instance, typename... Args>
        Instance &dispatch_instance(const std::string &t_name, Args &&... args) {
            // We can call everything by reference as we will instantiate directly afterwards anyways
            auto nd = this->get_node()->make_child(t_name, [&](const std::shared_ptr<misa_work_node> &t_node) {
                auto task = std::make_shared<Instance>(t_node, std::forward<Args>(args)...);
                return task;
            });

            return dynamic_cast<Instance&>(*nd->get_or_create_instance());
        }
    };
}

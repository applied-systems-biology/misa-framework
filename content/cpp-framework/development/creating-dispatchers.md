+++
title = "Creating dispatchers"
weight = 50
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

The MISA++ framework manages workloads by parallelized execution of atomic *tasks*. Additionally, *dispatchers* can be created that acts as inner node within a tree of workers. Tasks on the other hand are always leaves.

The module dispatcher (`./include/<module-name>/module.h`) acts as root of this tree and as entry point to define the tasks that process data and their dependencies.

Dispatchers require you to override following virtual functions:

```cpp
void create_blueprints(misa_dispatcher::blueprint_list &blueprints, misa_dispatcher::parameter_list &parameters);
void build(const misa_dispatcher::blueprint_builder &builder);
```

To allow the automated documentation of the whole tree of tasks, the `misaxx::misa_dispatcher` interface requires a data-independent declaration of all sub-dispatchers and tasks that the dispatcher might create, as well as all parameters that might be queried from the parameter file.

The function that defines all those available options is `create_blueprints(...)`. After defining all possible tasks, sub-dispatchers and parameters, you can use them within the `build(...)` function.

{{% notice warning %}}
Please keep in mind that `create_blueprints(...)` can be run in simulation mode (to acquire the parameter schema), which means that you should not query data or parameters
from within the function without checking `misaxx::runtime_properties::is_simulating()`.
{{% /notice %}}

# Creating and instantiating blueprints

A "possible task/dispatcher" is called a *blueprint* and defined within the `create_blueprints(...)` function. The dispatcher class offers the `create_blueprint<T>(...)` method that creates such a blueprint. `T` can be any **non-module** dispatcher or task. The created blueprint must be **added to the blueprint list** to activate it.

Blueprints are instantiated within the `build(...)` function by using the `T &misa_dispatcher::blueprint_builder::build<T>()` function.

{{% panel theme="default" header="Example" %}}
```cpp
void module::create_blueprints(misa_dispatcher::blueprint_list &blueprint_list, misa_dispatcher::parameter_list &parameters) {
    blueprint_list.add(create_blueprint<my_task>("my-task")); // Announces a task "my-task"
    create_blueprint<my_task>("wrong"); // WRONG: Forgot to add it to the blueprint list
}

void module::build(const misa_dispatcher::blueprint_builder &builder) {
    builder.build<my_task>("my-task"); // Instantiates a my_task "my-task"
    builder.build<my_task>("my-task"); // Can be done multiple times
    builder.build<unregistered_task>("wrong2"); // ERROR: Task is not registered
    builder.build<unregistered_task>("my-task"); // ERROR: "my-task" cannot be converted into unregistered_task
    my_task_base &task = builder.build<my_task_base>("my-task"); // NO error: If my_task inherits from my_task_base
}
```
{{% /panel %}}

{{% notice warning %}}
Instantiating workers does not mean that their work is **executed**. Instantiation only registers them into the runtime for later processing.
{{% /notice %}}

# Dependency management

Instantiating a worker with `build` is sufficient if there are no dependencies between workers.
If dependencies are required, dispatchers provide functionality to create dependencies between workers.
The framework runtime automatically ensures that those dependencies are satisfied.

You can use following classes to easily define dependencies:

```cpp
misaxx::misa_dispatcher::group
misaxx::misa_dispatcher::chain
```

## Groups

A group can be used to group workers together. It has two operators `<<` and `>>` that allow adding workers to the group and assigning the group as dependency of another worker.

{{% panel theme="default" header="Example" %}}
```cpp
void module::build(const misa_dispatcher::blueprint_builder &builder) {
    group parallelized;
    for (int i = 0; i < 3; ++i) {
        parallelized << builder.build<A>("A");
    }
    parallelized >> builder.build<B>("B");
}
```
{{% /panel %}}

The example builds a group of 3 tasks that have no specific order (**they can be run in parallel**) and another task that waits until all 3 tasks are finished.
This is the dependency graph that is created:
{{<mermaid align="center">}}
graph TD
A0["A"]-->B
A1["A"]-->B
A2["A"]-->B
{{< /mermaid >}}

## Chains

A chain allows easy creation of chain-dependencies. It only has a `>>` operator, but can be initialized with a set of dependencies.

{{% panel theme="default" header="Example" %}}
```cpp
void module::build(const misa_dispatcher::blueprint_builder &builder) {
    group parallelized;
    for (int i = 0; i < 3; ++i) {
        parallelized << builder.build<A>("A");
    }

    chain pipe {{ parallelized }};
    pipe >> builder.build<B>("B") >> builder.build<C>("C") >> builder.build<D>("D");
}
```
{{% /panel %}}

The example builds a group of 3 tasks that have no specific order (**they can be run in parallel**) and a chain of other tasks that are run strictly one-after-another.
This is the dependency graph that is created:

{{<mermaid align="center">}}
graph TD;
A0["A"]-->B;
A1["A"]-->B;
A2["A"]-->B;
B-->C;
C-->D;
{{< /mermaid >}}

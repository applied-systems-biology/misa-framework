+++
title = "Creating tasks"
weight = 40
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

The MISA++ framework manages workloads by parallelized execution of atomic *tasks*. Additionally, *dispatchers* can be created that acts as inner node within a tree of workers. Tasks on the other hand are always leaves.

The module dispatcher (`./include/<module-name>/module.h`) acts as root of this tree and as entry point to define the tasks that process data and their dependencies.

Tasks contain atomic workloads that should be able to run in parallel. All tasks inherit from `misaxx::misa_task` and require you to inherit following methods:

```cpp
void create_parameters(misaxx::misa_parameter_builder &parameters);
void work();
```

The `create_parameters()` function comes with a parameter builder and allows you to create `misaxx::misa_parameter<T>` instances. The `work()` function is called when the work should be executed.

# Creating parameters

To be able to automatically generate the parameter schema, the MISA++ framework requires you to initialize  parameters within the `create_parameters(...)` function.

{{% panel theme="default" header="Example" %}}
```cpp
struct my_task : public misa_task {
    using misa_task::misa_task;

    parameter<int> m_radius;

    void create_parameters(misaxx::misa_parameter_builder &parameters) {
        m_radius = parameters.create_algorithm_parameter<int>("radius", 10); // Will be independent of the current sample
    }
}
```
{{% /panel %}}

There are 3 types of parameters that are available:

| Type      | Parameter.json path         | Description                                                                 |
| --------- | --------------------------- | --------------------------------------------------------------------------- |
| Algorithm | /algorithm/../..*           | Independent from current sample. Depends on name and path of current worker |
| Sample    | /samples/(current sample)/* | Only depends on sample name.                                                |
| Runtime   | /runtime/*                  | Global                                                                      |

# Accessing data

Data is accessed via the module interface, or alternatively via cache accessor member variables.
Access to the current module interface is available via the `get_module()` and `get_module_as<T>()` functions. This applies to all workers (tasks and dispatchers).

{{% panel theme="default" header="Example" %}}
```cpp
struct my_task : public misa_task {
    using misa_task::misa_task;

    void work() {
        misaxx::ome::misa_ome_tiff tiff = get_module_as<my_module_interface>()->m_input_data;
        // Do something with tiff
    }
}
```
{{% /panel %}}

+++
title = "Dispatching submodules"
weight = 60
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

You reduce the amount of code in your current project by off-loading work
to another MISA++ module. The sub-module will be integrated into the
current worker tree and benefit from better parallelization and less post-processing
compared to running MISA++ applications in a pipeline.

# Prerequisites

Modify the `CMakeLists.txt` and the `cmake/*-config.in` to add the submodule as dependency.

{{% panel theme="default" header="Example" %}}
### In CMakeLists.txt
```cmake
find_package(misaxx-core REQUIRED)
find_package(misaxx-tissue-segmentation REQUIRED)

# Later:
target_link_libraries(my-module PUBLIC misaxx::misaxx-tissue-segmentation)
```

### In cmake/\*-config.in

```cmake
find_package(misaxx-core REQUIRED)
find_package(misaxx-tissue-segmentation REQUIRED)
```
{{% /panel %}}

# Submodules within the filesystem

A submodule is created on a subfolder of the current virtual filesystem and therefore are handled similar
to data. The name of the folder is set during the declaration of the submodule in a dispatcher's `create_blueprints` function.

{{<mermaid align="center">}}
graph TD
Main["main() root module"]
Main --> S1["Current module (Sample 1)"]
Main --> S2["Current module (Sample 2)"]
S1 --> D1("Data 1")
S1 --> D2("Data 2")
S2 --> D3("Data 1")
S2 --> D4("Data 2")
S1 ==> M1["Submodule"]
S2 ==> M2["Submodule"]
{{< /mermaid >}}

{{% notice warning %}}
Please do not name sub-modules like existing data.
This also means that the name of a submodule must be **unique** across the whole module.

This does not apply to submodules of the submodules, as each sub-module gets its own virtual filesystem.
{{% /notice %}}
{{% notice info %}}
Submodules are independent of the dispatcher where they are instantiated. They only depend on the filesystem of the instantiating module.
{{% /notice %}}

# Dispatching a submodule

It is recommended to add submodules to the module interface. To do this, create an empty shared pointer to the submodule interface in the current module interface.

```cpp
struct module_interface : public misaxx::misa_module_interface {
  std::shared_ptr<my_submodule::module_interface> m_submodule;
}
```

Instantiate the pointer within the `setup()` function (see [Creating dispatchers](../creating-dispatchers)). Here you can also pass input data from the current module to the submodule.

```cpp
void setup() {
  m_submodule = std::make_shared<my_submodule::module_interface>();

  // You have the option to directly set the input data of the submodule
  m_submodule.m_input_images = this->m_input_images;
}
```

Like all dispatchers, submodules need to be added to the list of blueprints. It is recommended to do this in the module dispatcher. Use `create_submodule_blueprint(...)` instead of `create_blueprint(...)`.

The `create_submodule_blueprint(...)` function requires you to give a name to the submodule and allows you to pass the submodule interface that was created within the module interface. The shared pointer will be automatically updated to follow the submodule dispatcher instead.

```cpp
module::create_blueprints(misa_dispatcher::blueprint_list &blueprints,
                          misa_dispatcher::parameter_list &parameters) {
  blueprints.add(create_submodule_blueprint<my_submodule::module>("my-submodule", get_module_as<module_interface>()->m_submodule));
}
```

Dispatch the submodule dispatcher just like any other dispatcher (see [Creating dispatchers](../creating-dispatchers)).

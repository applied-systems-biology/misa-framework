+++
title = "Declaring data"
weight = 30
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

MISA++ organizes input and output data in caches.
Caches provide functionality to access large datasets in a memory-efficient way.
To be able to achieve this, they are *linked* to a location within the filesystem (a folder).
The cache-implementation loads data into the memory on-demand and frees space if there is no demand left.

Caches are exclusively created within the module interface that is also responsible
for triggering the linking process between a folder and the cache implementation.

# Declaring and linking caches

To define that a cache is part of the interface, declare a variable of `misa::misa_cached_data<T>`. This instance is called an *accessor*, which is a shared pointer to the actual cache. Accessors are designed to include additional helper functions to make working with the specific cache type easier.

Linking of a cache is done within the `setup()` method of the module interface. In most cases, it is most reasonable to use the `suggest_import_location(...)` or `suggest_export_location(...)` functions to link a cache.

{{% notice info %}}
The paths provided to `suggest_import_location(...)` and `suggest_export_location(...)` are relative to the
virtual filesystem of the current module. The framework will automatically handle the organization of the files within
the physical filesystem.
{{% /notice %}}

## Input data

To declare input data, use the `suggest_import_location(...)` function. It loads a location from the filesystem if the cache was not already set from a parent module. Its first parameter, `filesystem` is the filesystem that is directly available from within the module interface. The second parameter describes the location within the input folder.

> The filesystem of a module interface is always relative. You do not have to account for your module being instantiated as a submodule.

## Output data

`suggest_export_location(...)` creates a new location within the output folder that will contain the generated data. It has a third parameter that contains all necessary information for the cache to create an output. The type of this *description* differs from cache to cache.

{{<mermaid align="center">}}
graph TD;
Description-->|export|Cache
Cache-->|describe|Description
Filesystem---|link|Cache
{{< /mermaid >}}

{{% panel theme="default" header="Example" %}}
```cpp
struct module_interface : public misa_module_interface {
    misaxx::ome::misa_ome_tiff m_input;
    misaxx::ome::misa_ome_tiff m_output;
    void setup();
}

module_interface::setup() {
    m_input.suggest_import_location(filesystem, "input"); // Link to <input folder>/input
    m_output.suggest_export_location(filesystem, "output", m_input.describe()); // Link to <output folder>/output. The output cache will have the same properties as the input
}
```
{{% /panel %}}

# Accessing cached data

Caches load data from hard disk on demand and are implemented in a way that minimizes read and write operations. Any cache accessor comes with 3 methods:

```cpp
readonly_access <value_type> access_readonly();
write_access <value_type> access_write();
readwrite_access <value_type> access_readwrite();
```

Those methods create proxy objects that ensure the correct state of the cache.

{{% notice warning %}}
Make sure to not destroy the proxy objects prematurely. The cache
might invalidate the data.
{{% /notice %}}

Access via proxy-objects or accessor-specific helper methods is thread-safe and uses a shared mutex to allow multiple threads to read the data, but only one thread to write it.

{{% notice warning %}}
Please be careful to not circumvent cache access, as it can lead to lost data or errors.
{{% /notice %}}

## Example

```cpp
void work() {
    auto access = m_input.access_readonly();
    cv::countNonZero(access.get());
    // Proxy object will be destroyed
}
```

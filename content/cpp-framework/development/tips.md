+++
title = "Tips and tricks"
weight = 90
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

# Interacting with the runtime

The MISA++ runtime is responsible for managing parameters, generation of the parameter schema,
managing caches and executing workers.

There are multiple interface functions avilable that allow extracting information from and manipulation of the runtime.

## misaxx::runtime_properties

Contains functions about the current set of workers, the number of threads, the root filesystem
and information about the root module.

The most important function is `misaxx::runtime_properties::is_simulating()` that indicates
if actual work should be done or a parameter schema is currently being generated.

## misaxx::cache_registry

Allows manual registration and de-registration of caches.

## misaxx::parameter_registry

Allows manual query of parameters, access to the parameter JSON data and parameter schema builder.

# Working on streaming data

MISA++ can be used to process that that is not (yet) fully present on the hard disk, but generated during the runtime. This is for example helpful for online analyses.

A possible way is to implement a cache that acts as data source and offers functionality to query data that should be processed and a way to know if the analysis should be finished.

Any MISA++ worker (tasks, dispatchers and module dispatchers) allows repetition of the workload. This does not include the parameter and blueprint generation. This functionality can be used to create new workers whenever new data is available.

```cpp
void create_blueprints(blueprint_list &blueprints, parameter_list &parameters) {
    blueprints.add(create_blueprint<online_analysis_dispatcher>("online-analysis"));
}

void build(const blueprint_builder &builder) {
    while(m_input_stream.has_new_data()) {
        online_analysis_dispatcher &worker = builder.build<online_analysis_dispatcher>("online-analysis");
        worker.m_input = m_input_stream.dequeue();
    }

    if(!m_input_stream.is_finished()) {
        this->repeat_work();
    }
}
```

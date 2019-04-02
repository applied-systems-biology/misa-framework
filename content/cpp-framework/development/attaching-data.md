+++
title = "Attaching data"
weight = 70
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

The attachment system allows easy attachment of any serializable data to a cache. It can be used to communicate between different tasks and generate quantification output. If the attachment data type supports adding information about its location (e.g. which image plane), the cache will automatically assign the most available information.

# Attaching data to a cache

To attach data, just call the `void attach<T>(...)` member function of a cache accessor. To access an attachment, you can use the `T &get<T>()` member function, which will *throw an exception* if the type does not exist. Use the `T &access<T>()` function to either return an existing attachment or create a new one.

Attachments are caches and therefore thread-safe.

## Example

```cpp
void my_task::work() {
    auto module = get_module_as<module_interface>();
    module->m_output.attach(my_attachment("important"));

    module->m_output.get<not_in_output>(); // Throws an exception

    module->m_output.access<not_in_output>().important_stuff = true; // Safe access
}
```

# Creating a new attachment type

Attachments inherit either from `misaxx::misa_serializable` or `misaxx::misa_locatable`. If you want a plain attachment, use `misaxx::misa_serializable` as base. The `misaxx::misa_locatable` interface additionally allows adding a location to the attachment. This location allows easy assignment of the data to the data its attached to.

Any MISA++ serializable must override following virtual methods:

```cpp
void from_json(const nlohmann::json &j);
void to_json(nlohmann::json &j) const;
void to_json_schema(misaxx::misa_json_schema_property &t_schema) const;
void build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const;
```

You additionally need to create following inline functions that are needed for JSON serialization.

```cpp
inline void to_json(nlohmann::json &j, const my_attachment &p) {
    p.to_json(j);
}

inline void from_json(const nlohmann::json &j, my_attachment &p) {
    p.from_json(j);
}
```

The `from_json` and `to_json` members should be used to serialize or deserialize the object. The `to_json_schema` describes the JSON that is created/expected by the `from_json` and `to_json` members and is used to generate the parameter schema.

Each serializable is assigned an unique *serialization id*. The `build_serialization_id_hierarchy` is used to create this ID.

## From and to JSON

Just take a look at the [JSON for Modern C++ documentation](https://nlohmann.github.io/json/).

## Describing the input and output JSON

To be able to create the parameter schema, each serialzable object must describe the expected structure of the serialized JSON. The `misaxx::misa_json_schema` provides methods to do this.

It offers following methods:

* `declare<T>(...)` declares the *current path* as JSON of type `T`. The default value is the `T()`
* `declare_optional<T>(...)` allows providing a default value
* `declare_required<T>(...)` marks the property as a **required** property
* `resolve()` returns a sub-path of the current JSON schema path. It will create a child property if needed

Please keep in mind that the provided `misaxx::misa_json_schema_property` represents the "root" of the current JSON object. This means that you have to call the declare function on `resolve(...)` to register member variables.

## Portable serialization

Each serializable type has an unique serialization id of type `misaxx::misa_serialization_id`. This id is part of a hierarchy of serialization ids that mirrors the inheritance hierarchy of the C++ class. This allows external programs to serialize even unsupported types with minimal loss of information.

The `build_serialization_id_hierarchy` is used to build this hierarchy. In most cases, it is sufficient to just add the serialization ID of the current class to the list of IDs provided in the parameter.

> Please do not forget to call `build_serialization_id_hierarchy` function of the parent class.

Serialization IDs have two components:

* The module
* A path that describes the serialized object

The serialization ID should be consistent with the location in the public interface, which means that it should for example be `misa_serialization_id("my-module", "attachments/my-attachment"`.

{{% panel theme="default" header="Example" %}}

```cpp
/**
* We want this attachment to be assignable to a cache after the program has run.
* Alternatively use misa_serializable
*/
struct my_attachment : public misaxx::misa_locatable {
    int count = 0;
    std::vector<double> variances;
    bool some_flag = false;

    void from_json(const nlohmann::json &j) {
        // As usual with nlohmann::json
        count = j["count"];
        variances = j["variances"].get<std::vector<double>>();

        // An optional parameter:
        if(j.find("some-flag") != j.end())
            some_flag = j["some-flag"];
    }

    void to_json(nlohmann::json &j) const {
        // As usual with nlohmann::json
        j["count"] = count;
        j["variances"] = variances;
        j["some-flag"] = some_flag;
    }

    void to_json_schema(const misaxx::misa_json_schema &t_schema) const {
        // The schema is at the "root" of the object.
        // This is for example helpful if you want to wrap a single value
        // e.g. t_schema.declare_required<int>();
        // if we read/write like this: this->value = j; in from_json

        // But we have member functions, so we need to use "resolve"

        // Short and efficient for "count":
        t_schema.resolve("count").declare_required<int>(); // No additional info. This is sufficient

        // some_flag is optional, so use the other method
        t_schema.resolve("some-flag").declare_optional<bool>(false); // Default is false

        // Use misa_json_property for extra fun
        t_schema.resolve("variances").declare(misa_json_property<std::vector<double>>()
        .make_optional()
        .with_title("Variances")
        .with_description("Optional variances"));
    }

    void build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const {
      // Please do not forget to call the parent build_serialization_id_hierarchy() method!
      misaxx::misa_locatable::build_serialization_id_hierarchy(result);

      // Add our own ID now
      result.emplace_back(misaxx::misa_serialization_id("my-module", "attachments/my-attachment"));
    }
}

// Important for nlohmann:json serialization
// Do not forget them
inline void to_json(nlohmann::json &j, const my_attachment &p) {
    p.to_json(j);
}

inline void from_json(const nlohmann::json &j, my_attachment &p) {
    p.from_json(j);
}
```
{{% /panel %}}

+++
title = "Creating caches"
weight = 80
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

This guide will show you how to create your own cache types. A cache data type consists of four different classes:

* The cache implementation
* The cache accessor
* A pattern
* A description

The cache implementation is responsible for dynamically loading and unloading data from/to a folder that is located within the filesystem. Such a folder on the other hand can contain many different files with different names, while an image file cache would for example look for a specific image file within this folder.

Instead of cache-specific functions like a `set_file(path)` function, a *description*
is used as input of a cache. It uses this description to set all internal parameters to function correctly. Descriptions are serializable, making them portable. The `describe()` function of a cache allows access to this description and implementation-independent modification of its values. This enables easy derivation of exported data from imported data.

A description on the other hand just contains already defined parameters (like for example the filename), which does not solve the issue of caches containing vastly different files. *Patterns* are objects with the purpose of creating a description from its a folder in a filesystem.

A cache *accessor* is a shared-pointer-like object with the purpose of allowing easy access to the functionality of a cache. You can use it to add convenient methods that make it easier to work with the specific cache type.

{{% notice info %}}
The module interface and workers access a cache only via a cache accessor.
{{% /notice %}}

{{<mermaid align="center">}}
graph TD;
Pattern-->|Produce|Description
Description-->|Link|Cache
Cache-->|Describe|Description
Filesystem-->|Produce|Description
Accessor-->|Access|Cache
Accessor-.-I["Module interface"]
{{< /mermaid >}}

The base types of the necessary cache components are:

* `misaxx::misa_cache` and any of `misaxx::utils::access::cache` for caches
* `misaxx::misa_cached_data` for accessors
* `misaxx::misa_data_pattern` for patterns
* `misaxx::misa_data_description` for desciptions

# Creating the cache implementation

We first show you to use our `misaxx::misa_default_cache` helper that automates many functions that you would need to define manually.

The `misaxx::misa_default_cache` helper requires three template arguments:

* A mutexed cache of type `misaxx::utils::access::cache` or `misaxx::utils::access::memory_cache`
* The pattern type
* The description type

> Please make sure to create a cache-specific description type. This is needed to later identify the cache type. Optional for the pattern type.

The *mutexed cache* is a utility type that models the actual read and write access of the cache implementation. If you create a cache with a small enough footprint that allows it to be stored within memory, you can use `misaxx::utils::access::memory_cache` instead.

## Linking, Simulating and Postprocessing

The `misaxx::misa_default_cache` requires you to override following functions:

```cpp
Description produce_description(const boost::filesystem::path &, const Pattern &);
void do_link(const <Description> &t_description);
void simulate_link(); // Optional
void postprocess() // Optional
```

The `produce_description()` function is responsible for creating applying the pattern and generating a description that allows the link function to find all necessary data within the folder.

The main link function is `do_link()`. It takes the description type of the cache and allows you to setup all internal functionality to make reading and writing work. It is only called if the runtime is not creating a parameter schema.

If a parameter schema is created, `simulate_link()` is called instead. By default, it will ensure that `describe()` returns a storage that contains both the pattern and description type. If you want to add additional information, just override this method.

### Example

```cpp
// The parameter schema will have the information that the pattern looks for JSON files
void my_cache::simulate_link() {
    describe()->set(misa_file_pattern({ ".json" }));
    describe()->access<misa_file_description>();
}
```

### Postprocessing

The `postprocess()` method will be called after the runtime has finished all tasks you can use it to clean up the cache or finalize the output.

## Location and unique location

A cache always has a *location* that corresponds to the location provided in the link function. It is automatically set by the default cache.

Additionally, there is an *unique location* that should always point to some file within the cache's location folder (use `get_location()` to obtain it). **Please do not forget to set this unique location using set_unique_location() somewhere in do_link()**

## Sub-caches

You can define sub-caches that are manually linked during `do_link()`. Just make sure that their location is equal to the location of the main cache and that their **unique** location is unique.

## Thread-safe data access

A cache implementation always inherits from a mutexed cache of type `misaxx::utils::access::cache`. It will require you to override following methods as well:

```cpp
T &get() override;
const T &get() const override;
void set(T value) override;
bool has() const override;
bool can_pull() const override;
void pull() override;
void stash() override;
void push() override;
```

The get and set functions do what you would expect. `pull()` is responsible for loading data into memory, while `push()` is responsible for writing the current data to files on the harddrive. `push()` should **not** clear the data. `stash()` will be called to do only this if necessary. `can_pull()` should return if loading is possible (e.g. if the file exists).

All methods are called from a thread-safe environment using a shared mutex:

| Function | Access    |
| -------- | --------- |
| get      | shared    |
| has      | shared    |
| can_pull | shared    |
| pull     | exclusive |
| stash    | exclusive |
| push     | exclusive |

{{% notice warning %}}
Do not call `get()`, `has()`, etc. manually unless you know exactly what you do.
Use `readonly_access()`, `write_access()` and `readwrite_access()` that are provided by the accessor!
{{% /notice %}}

## The cache accessor

A cache accessor is a shared pointer to a cache implementation. It contains additional functions to interact with the cache.

Just inherit from `misa_cached_data<CacheImplementation>`. You can also inherit from `misa_description_accessors_from_cache<CacheImplementation, SELF>` to automatically add convenient access to caches that are based on `misa_default_cache`.

{{% panel theme="default" header="Example" %}}

We will create a cache that provides access to an integer. It will be stored in a file, so it is sufficient to just use a file pattern and derive our description from misa_file_description.

### The pattern

```cpp
/**
patterns/integer_pattern.h
*/

struct integer_pattern : public misaxx::misa_file_pattern {
  // We only look for .integer files
  integer_description() : misaxx::misa_file_pattern({ ".integer" }) {

  }
protected:
      void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
}

inline void to_json(nlohmann::json& j, const integer_pattern& p);
inline void from_json(const nlohmann::json& j, integer_pattern& p);
```

### The description

```cpp
/**
descriptions/integer_description.h
*/

struct integer_description : public misaxx::misa_file_description {
  using misa_file_description::misa_file_description;
protected:
      void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
}

inline void to_json(nlohmann::json& j, const integer_description& p);
inline void from_json(const nlohmann::json& j, integer_description& p);

```

### The cache implementation

```cpp
/**
caches/integer_cache.h
*/

struct integer_cache : public misaxx::misa_default_cache<
  misaxx::utils::access::cache<int>,
  integer_pattern,
  integer_description> {

    // Inherited from mutexed cache. Just plain I/O
    int &get() { return m_value.get(); }

    const int &get() const { return m_value.get(); }

    void set(int v) { m_value = v; }

    bool has() const { return m_value.has_value(); }

    bool can_pull() const { return boost::filesystem::exists(m_path); }

    void stash() { m_value = std::nullopt; }

    void pull() {
      std::ifstream r;
      r.open(m_path.string());
      int v;
      r >> v;
      m_value = v;
    }

    void push() {
      std::ofstream w;
      w.open(m_path.string())
      w << m_value.get();
    }

    // Linkage, simulation & postprocessing
protected:

    integer_description produce_description(const boost::filesystem::path &path, const integer_pattern &pattern) override {
      integer_description description;
      pattern.apply(description, path); // Functionality of misa_file_pattern
      return description;
    }

public:

    void do_link(const integer_description &description) override {
      // Just take the filename in this case
      m_path = get_location() / description.filename;

      // VERY IMPORTANT
      set_unique_location(m_path);
    }

    void postprocess() override {
      std::cout << "Nothing" << "\n";
    }

private:
    boost::filesystem::path m_path;
    std::optional<int> m_value;
}
```

### Cache accessor

```cpp
/**
accessors/integer.h
*/

struct integer : public misaxx::misa_cached_data<integer_cache> {
  using misaxx::misa_cached_data<integer_cache>::misa_cached_data;

  // We add some additional functions for convenience
  int get() const {
    auto access = readonly_access();
    return access.get();
  }

  void set(int value) {
    auto access = write_access();
    access.set(value);
  }

}
```

{{% /panel %}}

# Sub-caches

You can create sub-caches that allow access to individual parts of the data. They live within the same filesystem location and are linked in the `do_link()` method of the main cache.

To make it easier, we provide the `misaxx::manual_cache` type that can be used as base instead of `misaxx::default_cache`. It provides the same functionality, but does not require a pattern. Instead, the main cache should provide a valid description.

In the `do_link()` method, create an uninitialized accessor type of the sub-cache and set its `data` attribute manually. Then run `force_link` with the appropriate arguments.
This will ensure that the sub-cache is registered correctly.

{{% panel theme="default" header="Example" %}}

```cpp
void do_link(integer_description &description) override {
  decimals accessor;
  accessor.data = std::make_shared<decimals_cache>();
  // Manual setup etc.

  // Important:
  decimal_description subcache_description;
  accessor.force_link(this->get_location(),
    misa_description_storage::with(subcache_description));

  // Store for later usage
  this->decimals = std::move(accessor);
}
```

{{% /panel %}}

# Without using misa_default_cache

A cache implementation should inherit from `misaxx::misa_cache` **and** a mutexed cache (`misaxx::utils::access::cache`). It should override following methods:

```cpp

// Main function that applies the linkage process:
void link(const boost::filesystem::path &t_location, const std::shared_ptr<misa_description_storage> &t_description);

// Returns the pattern & description:
std::shared_ptr<misa_description_storage> describe() const;

// The absolute folder path where the data is located
boost::filesystem::path get_location() const;

// A filename or any other unique file (for an explanation see below)
boost::filesystem::path get_unique_location() const;

// Returns true if the cache has currently data
bool has_data();

// Returns a "self-description" of the cache location
std::shared_ptr<const misa_location> get_location_interface() const

// Optionally: Run after all work is done
void postprocess()

// Additional methods might be necessary depending on the mutexed cache implementation
```

The `link()` method is provided with an absolute path within the filesystem and a description. The `link()` function is called independent of the **simulation mode**. This means you have to use `misaxx:runtime_properties::is_simulating()` to check if the runtime is actually processing data.

{{% notice warning %}}
Please make sure that `describe()` **always** returns a non-empty description storage with **both a pattern and description**. This also applies for parameter schema generation.
{{% /notice %}}

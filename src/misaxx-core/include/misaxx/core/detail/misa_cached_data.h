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

#include <iostream>

namespace misaxx {
    template<class Cache>
    misa_cached_data<Cache>::misa_cached_data(Cache t_cache) : data(std::make_shared<Cache>(std::move(t_cache))) {
    }

    template<class Cache>
    bool misa_cached_data<Cache>::is_externally_set() const {
        return data && !data.unique();
    }

    template<class Cache>
    bool misa_cached_data<Cache>::has_data() const {
        return static_cast<bool>(data);
    }

    template<class Cache>
    std::shared_ptr<misa_cache> misa_cached_data<Cache>::get_cache_base() const {
        return data;
    }

    template<class Cache>
    void misa_cached_data<Cache>::force_link(const boost::filesystem::path &t_internal_location,
                                             const boost::filesystem::path &t_location,
                                             const std::shared_ptr<misa_description_storage> &t_description) {
        if (!data)
            data = std::make_shared<Cache>();
        misaxx::cache_registry::register_cache(data);
        std::cout << "[Cache] Linking " << t_location << " (" << t_internal_location << ") " << " into cache of type " << typeid(Cache).name()
                  << "\n";
        data->link(t_internal_location, t_location, t_description);
    }

    template<class Cache>
    void misa_cached_data<Cache>::force_link(const filesystem::const_entry &t_location) {
        if (!data)
            data = std::make_shared<Cache>();
        misaxx::cache_registry::register_cache(data);

        // Special case simulation mode
        if (misaxx::runtime_properties::is_simulating()) {
            std::cout << "[Cache] Linking " << t_location->internal_path() << " into cache of type "
                      << typeid(Cache).name() << "\n";
            data->link(t_location->internal_path() ,"", t_location->metadata);
            return;
        }

        std::cout << "[Cache] Linking " << t_location->internal_path() << " [" << t_location->external_path()
                  << "] into cache of type " << typeid(Cache).name() << "\n";
        data->link(t_location->internal_path(), t_location->external_path(), t_location->metadata);
    }

    template<class Cache>
    void misa_cached_data<Cache>::suggest_link(const boost::filesystem::path &t_internal_location,
                                                const boost::filesystem::path &t_location,
                                               const std::shared_ptr<misa_description_storage> &t_description) {
        if (!data) {
            data = std::make_shared<Cache>();
            force_link(t_internal_location, t_location, t_description);
        }
    }

    template<class Cache>
    void misa_cached_data<Cache>::suggest_link(const filesystem::const_entry &t_location) {
        if (!data) {
            data = std::make_shared<Cache>();
            force_link(t_location);
        }
    }

    template<class Cache>
    void misa_cached_data<Cache>::suggest_create(const filesystem::entry &t_location,
                                                 const std::shared_ptr<misa_description_storage> &t_description) {
        if (!data) {
            data = std::make_shared<Cache>();
            misaxx::cache_registry::register_cache(data);

            if (misaxx::runtime_properties::is_simulating()) {
                std::cout << "[Cache] Creating " << t_location->internal_path() << " as cache of type "
                          << typeid(Cache).name() << "\n";
                // Metadata is copied into the export location
                if (t_description.unique()) {
                    t_location->metadata = t_description;
                } else {
                    t_location->metadata = std::make_shared<misa_description_storage>(*t_description);
                }

                data->link(t_location->internal_path(), "", t_location->metadata);
                return;
            }

            std::cout << "[Cache] Creating " << t_location->internal_path() << " [" << t_location->external_path()
                      << "] as cache of type " << typeid(Cache).name() << "\n";

            // Create the directory if necessary
            if (!boost::filesystem::exists(t_location->external_path())) {
                std::cout << "[Cache] Creating directory " << t_location->external_path() << "\n";
                boost::filesystem::create_directories(t_location->external_path());
            }

            // Metadata is copied into the export location
            if (t_description.unique()) {
                t_location->metadata = t_description;
            } else {
                t_location->metadata = std::make_shared<misa_description_storage>(*t_description);
            }

            data->link(t_location->internal_path(), t_location->external_path(), t_location->metadata);
        }
    }

    template<class Cache>
    void misa_cached_data<Cache>::suggest_import_location(const misa_filesystem &t_filesystem,
                                                          const boost::filesystem::path &t_path) {
        if (!data) {
            suggest_link(t_filesystem.imported->resolve(t_path));
        }
    }

    template<class Cache>
    void misa_cached_data<Cache>::suggest_export_location(const misa_filesystem &t_filesystem,
                                                          const boost::filesystem::path &t_path,
                                                          const std::shared_ptr<misa_description_storage> &t_description) {
        if (!data) {
//                if (t_filesystem.exported->has_subpath(t_path))
//                    throw std::runtime_error(
//                            "Suggested export location exported/" + t_path.string() + " is already used!");
            suggest_create(t_filesystem.exported->resolve(t_path), t_description);
        }
    }

    template<class Cache>
    std::shared_ptr<misa_description_storage> misa_cached_data<Cache>::describe() const {
        return data->describe();
    }

    template<class Cache>
    boost::filesystem::path misa_cached_data<Cache>::get_internal_location() const {
        return data->get_internal_location();
    }

    template<class Cache>
    boost::filesystem::path misa_cached_data<Cache>::get_location() const {
        return data->get_location();
    }

    template<class Cache>
    boost::filesystem::path misa_cached_data<Cache>::get_unique_location() const {
        return data->get_unique_location();
    }

    template<class Cache>
    std::shared_ptr<const misa_location> misa_cached_data<Cache>::get_location_interface() const {
        return data->get_location_interface();
    }

    template<class Cache>
    void
    misa_cached_data<Cache>::force_link(const boost::filesystem::path &t_internal_location, const boost::filesystem::path &t_location, std::shared_ptr<misa_data_description> t_description) {
        force_link(t_internal_location, t_location, std::make_shared<misa_description_storage>(std::shared_ptr<misa_data_pattern>(), std::move(t_description)));
    }

    template<class Cache>
    void misa_cached_data<Cache>::suggest_link(const boost::filesystem::path &t_internal_location,
                                               const boost::filesystem::path &t_location,
                                               std::shared_ptr<misa_data_description> t_description) {
        suggest_link(t_internal_location, t_location, std::make_shared<misa_description_storage>(std::shared_ptr<misa_data_pattern>(), std::move(t_description)));
    }

    template<class Cache>
    void
    misa_cached_data<Cache>::suggest_create(const filesystem::entry &t_location, std::shared_ptr<misa_data_description> t_description) {
        suggest_create(t_location, std::make_shared<misa_description_storage>(std::shared_ptr<misa_data_pattern>(), std::move(t_description)));
    }

    template<class Cache>
    void misa_cached_data<Cache>::suggest_export_location(const misa_filesystem &t_filesystem,
                                                          const boost::filesystem::path &t_path,
                                                          std::shared_ptr<misa_data_description> t_description) {
        suggest_export_location(t_filesystem, t_path, std::make_shared<misa_description_storage>(std::shared_ptr<misa_data_pattern>(), std::move(t_description)));
    }


}
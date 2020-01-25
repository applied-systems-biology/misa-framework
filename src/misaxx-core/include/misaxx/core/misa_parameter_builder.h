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

#include <misaxx/core/misa_parameter.h>

namespace misaxx {

    class misa_worker;

    /**
     * Allows creating parameters for misa_workers and read-only access to them
     */
    struct misa_parameter_builder {

        explicit misa_parameter_builder(misa_worker &t_worker);

        /**
         * Creates a parameter that is equal across all samples
         * @tparam T
         * @param t_name
         * @param t_default
         * @param t_metadata
         * @return
         */
        template<typename T> misa_parameter<T> create_algorithm_parameter(std::string t_name, const T &t_default) {
            auto path = get_algorithm_path();
            path.emplace_back(std::move(t_name));
            auto schema = misaxx::parameter_registry::register_parameter(path);
            schema->declare_optional<T>(t_default);
            return misa_parameter<T>(std::move(path), std::move(schema));
        }

        /**
         * Creates a parameter that is equal across all samples
         * @tparam T
         * @param t_name
         * @param t_metadata
         * @return
         */
        template<typename T> misa_parameter<T> create_algorithm_parameter(std::string t_name) {
            auto path = get_algorithm_path();
            path.emplace_back(std::move(t_name));
            auto schema = misaxx::parameter_registry::register_parameter(path);
            schema->declare_required<T>();
            return misa_parameter<T>(std::move(path), std::move(schema));
        }

        /**
         * Creates a parameter that is global within the sample
         * @tparam T
         * @param t_name
         * @param t_default
         * @param t_metadata
         * @return
         */
        template<typename T> misa_parameter<T> create_sample_parameter(std::string t_name, const T &t_default) {
            auto path = get_sample_path();
            path.emplace_back(std::move(t_name));
            auto schema = misaxx::parameter_registry::register_parameter(path);
            schema->declare_optional<T>(t_default);
            return misa_parameter<T>(std::move(path), std::move(schema));
        }

        /**
         * Creates a parameter that is global within the sample
         * @tparam T
         * @param t_name
         * @param t_metadata
         * @return
         */
        template<typename T> misa_parameter<T> create_sample_parameter(std::string t_name) {
            auto path = get_sample_path();
            path.emplace_back(std::move(t_name));
            auto schema = misaxx::parameter_registry::register_parameter(path);
            schema->declare_required<T>();
            return misa_parameter<T>(std::move(path), std::move(schema));
        }

        /**
       * Creates a global parameter
       * @tparam T
       * @param t_name
       * @param t_default
       * @param t_metadata
       * @return
       */
        template<typename T> misa_parameter<T> create_runtime_parameter(std::string t_name, const T &t_default) {
            auto path = get_runtime_path();
            path.emplace_back(std::move(t_name));
            auto schema = misaxx::parameter_registry::register_parameter(path);
            schema->declare_optional<T>(t_default);
            return misa_parameter<T>(std::move(path), std::move(schema));
        }

        /**
         * Creates a parameter that is global within the sample
         * @tparam T
         * @param t_name
         * @param t_metadata
         * @return
         */
        template<typename T> misa_parameter<T> create_runtime_parameter(std::string t_name) {
            auto path = get_runtime_path();
            path.emplace_back(std::move(t_name));
            auto schema = misaxx::parameter_registry::register_parameter(path);
            schema->declare_required<T>();
            return misa_parameter<T>(std::move(path), std::move(schema));
        }

    private:

        std::weak_ptr<misa_worker> m_worker;

        std::vector<std::string> get_algorithm_path();

        std::vector<std::string> get_sample_path();

        std::vector<std::string> get_node_path();

        std::vector<std::string> get_runtime_path();
    };
}




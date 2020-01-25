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

#include <misaxx/core/misa_serializable.h>

namespace misaxx {
    /**
     * A wrapper around a primitive value that supports dynamic serialization (misa_serializable)
     * @tparam T Must be compile-type serializable by nlohmann::json
     */
    template<typename T> struct misa_primitive : public misa_serializable {

        using value_type = T;

        T value = T();

        misa_primitive() = default;

        misa_primitive(T v) : value(std::move(v)) {

        }

        operator T() {
            return value;
        }

        void from_json(const nlohmann::json &t_json) override {
            value = misa_serializable::deserialize_wrapped<T>(t_json);
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializable::to_json(t_json);
            misa_serializable::serialize_wrapped(value, t_json);
        }

        void to_json_schema(misa_json_schema_property &t_schema) const override {
            misa_serializable::to_json_schema(t_schema);
            t_schema = T();
        }

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override {
            misa_serializable::build_serialization_id_hierarchy(result);
            if constexpr (std::is_same<int, T>::value) {
                result.emplace_back(misa_serialization_id("misa", "primitive/int"));
            }
            else if constexpr (std::is_same<char, T>::value) {
                result.emplace_back(misa_serialization_id("misa", "primitive/char"));
            }
            else if constexpr (std::is_same<short, T>::value) {
                result.emplace_back(misa_serialization_id("misa", "primitive/short"));
            }
            else if constexpr (std::is_same<long, T>::value) {
                result.emplace_back(misa_serialization_id("misa", "primitive/long"));
            }
            else if constexpr (std::is_same<double, T>::value) {
                result.emplace_back(misa_serialization_id("misa", "primitive/double"));
            }
            else if constexpr (std::is_same<float, T>::value) {
                result.emplace_back(misa_serialization_id("misa", "primitive/float"));
            }
            else if constexpr (std::is_same<std::string, T>::value) {
                result.emplace_back(misa_serialization_id("misa", "primitive/string"));
            }
            else if constexpr (std::is_same<bool, T>::value) {
                result.emplace_back(misa_serialization_id("misa", "primitive/bool"));
            }
            else {
                result.emplace_back(misa_serialization_id("misa", std::string("primitive/") + typeid(T).name()));
            }
        }
    };

    /**
     * Returns a misa_primitive<T> if needed to wrap the value
     */
    template<typename T> using misa_serializable_value = typename std::conditional<std::is_base_of<misa_serializable, T>::value, T, misa_primitive<T>>::type;
}

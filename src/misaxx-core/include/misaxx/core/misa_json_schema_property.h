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

#include <nlohmann/json.hpp>
#include <misaxx/core/misa_serialization_id.h>
#include <misaxx/core/misa_serializable.h>
#include <map>
#include <unordered_map>

namespace misaxx {

    struct misa_json_schema_property : public std::enable_shared_from_this<misa_json_schema_property> {

        /**
         * Needed to override buggy behavior of nlohmann::json's object detector
         * @tparam Type
         */
        template<typename Type>
        struct is_map : public std::false_type {

        };

        template<typename V>
        struct is_map<std::map<std::string, V>> : public std::true_type {

        };

        template<typename V>
        struct is_map<std::unordered_map<std::string, V>> : public std::true_type {

        };

        /**
         * Subtree-properties are supposed to have no default value and no child template.
         * Leaf properties can have a child template and default value, but no child properties.
         */
        enum class property_type {
            subtree,
            leaf
        };

        /**
         * Type of this JSON schema property
         */
        property_type property_type = property_type::subtree;

        /**
         * Type of the JSON entry descibed by this schema
         */
        nlohmann::json::value_t value_type = nlohmann::json::value_t::object;

        /**
        * A short descriptive title of the property
        */
        std::string title = "";

        /**
         * A short explanation what the property represents
         */
        std::string description = "";

        /**
         * A short descriptive name of the property type
         */
        std::string type_title = "";

        /**
         * A short explanation what the type does
         */
        std::string type_description = "";

        /**
         * True if the value of this schema object must be set by the user
         */
        bool required = false;

        /**
         * Optional default value of this property
         */
        std::optional<nlohmann::json> default_value = std::nullopt;

        /**
         * If contains values, the property is an enum
         */
        std::vector<nlohmann::json> allowed_values;

        /**
         * Optional description of child entries if this schema describes an object or an array
         */
        std::shared_ptr<misa_json_schema_property> children_template;

        /**
         * Properties if this schema describes an object
         * Only valid if the property_type is of type "object"
         */
        std::unordered_map<std::string, std::shared_ptr<misa_json_schema_property>> properties;

        /**
         * Information about the current object if its is a misaxx::misa_serializable
         */
        std::optional<misa_serialization_id> serialization_id;

        /**
         * Information about the current object if its is a misaxx::misa_serializable
         */
        std::optional<std::vector<misa_serialization_id>> serialization_hierarchy;

        /**
         * Additional properties that are merged into the schema
         */
        nlohmann::json annotation;

        /**
         * Converts this JSON schema property to a proper JSON JSON
         * @return
         */
        void to_json(nlohmann::json &json) const;

        /**
         * Declares this JSON schema property as the given type
         * Supports misaxx::misa_serializable
         * @tparam T
         * @return
         */
        template<typename T>
        misa_json_schema_property &declare() {
            if constexpr (std::is_base_of<misa_serializable, T>::value) {
                property_type = property_type::subtree;
                value_type = nlohmann::json::value_t::object;
                dynamic_cast<const misa_serializable &>(T()).to_json_schema(*this);
            } else if constexpr (is_map<T>::value) {
                property_type = property_type::leaf;
                value_type = nlohmann::json::value_t::object;
                // Extract the "additionalProperties" property from the type as children_template
                children_template = std::make_shared<misa_json_schema_property>();
                children_template->declare<typename T::mapped_type>();
            } else if constexpr (nlohmann::detail::is_compatible_string_type<nlohmann::json, T>::value) {
                property_type = property_type::leaf;
                value_type = nlohmann::json::value_t::string;
            } else if constexpr (nlohmann::detail::is_compatible_integer_type<nlohmann::json, T>::value) {
                property_type = property_type::leaf;
                value_type = nlohmann::json::value_t::number_integer;
            } else if constexpr (nlohmann::detail::is_compatible_array_type<nlohmann::json, T>::value) {
                property_type = property_type::leaf;
                value_type = nlohmann::json::value_t::array;
                // Extract the "items" property from the type as children_template
                children_template = std::make_shared<misa_json_schema_property>();
                children_template->declare<typename T::value_type>();
            } else if constexpr (nlohmann::detail::is_compatible_object_type<nlohmann::json, T>::value) {
                property_type = property_type::leaf;
                value_type = nlohmann::json::value_t::object;
                // Extract the "additionalProperties" property from the type as children_template
                children_template = std::make_shared<misa_json_schema_property>();
                children_template->declare<typename T::mapped_type>();
            } else {
                property_type = property_type::leaf;
                value_type = nlohmann::json(T()).type();
            }
            return *this;
        }

        /**
         * Adds a name to this property
         * @param title
         * @return
         */
        misa_json_schema_property &document_title(std::string title) {
            this->title = std::move(title);
            return *this;
        }

        /**
         * Adds a description to this property
         * @param title
         * @return
         */
        misa_json_schema_property &document_description(std::string description) {
            this->description = std::move(description);
            return *this;
        }

        /**
          * Adds a name to this property type
          * @param title
          * @return
          */
        misa_json_schema_property &document_type_title(std::string title) {
            this->type_title = std::move(title);
            return *this;
        }

        /**
         * Adds a description to this property type
         * @param title
         * @return
         */
        misa_json_schema_property &document_type_description(std::string description) {
            this->type_description = std::move(description);
            return *this;
        }

        /**
         * Indicates that this property is required
         * @return
         */
        misa_json_schema_property &make_required() {
            required = true;
            return *this;
        }

        /**
         * Indicates that this property is required
         * @return
         */
        template<typename T>
        misa_json_schema_property &make_optional(const T &t_default_value) {
            required = false;
            if constexpr (std::is_same<nlohmann::json, T>::value)
                default_value = t_default_value;
            else
                default_value = nlohmann::json(t_default_value);
            return *this;
        }

        /**
         * Converts a subtree property with one child into a leaf property with a child template
         * @return
         */
        misa_json_schema_property &make_template() {
            if (property_type == property_type::leaf)
                return *this;
            if (properties.size() != 1)
                throw std::runtime_error(
                        "Exactly 1 child property is needed to make this subtree property a leaf property");
            children_template = properties.begin()->second;
            properties.clear();
            property_type = property_type::leaf;
            return *this;
        }

        /**
         * Sets the allowed_values property
         * @tparam V
         * @param values
         * @return
         */
        template<typename V>
        misa_json_schema_property &make_enum(const std::vector<V> &values) {
            for (const auto &v : values) {
                allowed_values.emplace_back(nlohmann::json(v));
            }
            return *this;
        }

        /**
         * declare().make_optional()
         * @tparam T
         * @param t_value
         * @return
         */
        template<typename T>
        misa_json_schema_property &declare_optional(const T &t_value = T()) {
            return declare<T>().make_optional(t_value);
        }

        /**
       * declare().make_required()
       * @tparam T
       * @param t_value
       * @return
       */
        template<typename T>
        misa_json_schema_property &declare_required() {
            return declare<T>().make_required();
        }

        /**
         * Equivalent to declare().make_required() and setting the default value
         * @tparam T
         * @param t_value
         * @return
         */
        template<typename T>
        misa_json_schema_property &define(const T &t_value) {
            return declare_optional<T>(t_value);
        }

        /**
         * Creates and returns a sub-property with given key if this schema is a subtree-object
         * @param key
         * @return
         */
        std::shared_ptr<misa_json_schema_property> resolve(const std::string &key);

        /**
         * Creates and returns a sub-property with given key if this schema is a subtree-object
         * @param key
         * @return
         */
        std::shared_ptr<misa_json_schema_property> resolve(const std::vector<std::string> &keys);

        /**
         * Shortcut for *resolve()
         * @param key
         * @return
         */
        misa_json_schema_property &operator[](const std::string &key) {
            return *resolve(key);
        }

        /**
         * Shortcut for declare<T>() with T being the argument if T is not a misa_json_schema_property
         * @tparam T
         * @param value
         * @return
         */
        template<typename T, typename = typename std::enable_if<!std::is_same<T, misa_json_schema_property>::value>::type>
        misa_json_schema_property &operator=(const T &) {
            return declare<T>();
        }

        std::shared_ptr<misa_json_schema_property> self();

        std::shared_ptr<const misa_json_schema_property> self() const;

    };
}



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

namespace misaxx {

    struct misa_json_schema_property;

    /**
     * An object that can be serialized and deserialized from/to JSON.
     */
    struct misa_serializable {
        /**
         * Deserializes the JSON data
         * @param t_json
         */
        virtual void from_json(const nlohmann::json &t_json);

        /**
         * Serializes the JSON data
         * @param t_json
         */
        virtual void to_json(nlohmann::json &t_json) const;

        /**
         * Describes the structure of the data as JSON schema
         * Must be consistent with the JSON structure expected by from_json and to_json
         * @param schema
         */
        virtual void to_json_schema(misa_json_schema_property &schema) const;

        /**
         * Gets the serialization ID of the object
         * @return
         */
        misa_serialization_id get_serialization_id() const;

        /**
         * Gets a hierarchy of serialization IDs that is consistent with the inheritance hierarchy.
         * The hierarchy should include the current serialization id and should be ordered such as the
         * current id is the last entry
         * @return
         */
        std::vector<misa_serialization_id> get_serialization_id_hierarchy() const;

        /**
         * Override this method to change the automatically generated documentation name of this object
         * It is used during to_json_schema()
         * @return
         */
        virtual std::string get_documentation_name() const;

        /**
         * Override this method to change the automatically generatated documentation name of this object
         * It is used during to_json_schema()
         * @return
         */
        virtual std::string get_documentation_description() const;

        /**
         * Deserializes a wrapped value from the JSON
         * @tparam T
         * @param t_json
         * @return
         */
        template<typename T> static T deserialize_wrapped(const nlohmann::json &t_json) {
            if(t_json.is_object() && t_json.find("misa:serializable/value") != t_json.end()) {
                return t_json["misa:serializable/value"].get<T>();
            }
            else {
                return t_json.get<T>();
            }
        }

        /**
         * Serializes objects as-is, but other JSON types wrapped into an misa:serializable/value attibute.
         * This is required, so misa_serializable can attach type information
         * @tparam T
         * @param value
         * @param t_json
         */
        template<typename T> static void serialize_wrapped(const T &value, nlohmann::json &t_json) {
            nlohmann::json v = value;
            if(v.is_object()) {
                for(auto it = v.begin(); it != v.end(); ++it) {
                    t_json[it.key()] = it.value();
                }
            }
            else {
                t_json["misa:serializable/value"] = std::move(v);
            }
        }

        /**
         * Returns the serialization ID from a JSON object
         * @param t_json
         * @return
         */
        static misa_serialization_id get_serialization_id_from_json(const nlohmann::json &t_json);

        /**
         * Returns the serialization ID from a JSON object or an alternative
         * @param t_json
         * @return
         */
        static misa_serialization_id get_serialization_id_from_json_or(const nlohmann::json &t_json, misa_serialization_id t_default);

        /**
         * Returns the serialization ID hierarchy from a JSON object
         * @param t_json
         * @return
         */
        static std::vector<misa_serialization_id> get_serialization_hierarchy_from_json(const nlohmann::json &t_json);

        /**
         * Returns the serialization ID hierarchy from a JSON object or an alternative
         * @param t_json
         * @return
         */
        static std::vector<misa_serialization_id> get_serialization_hierarchy_from_json_or(const nlohmann::json &t_json, std::vector<misa_serialization_id> t_default);

        /**
         * Returns true if the JSON object indicates via JSON hierarchy that it can be deserialized into the target id
         * @param t_json
         * @return
         */
        static bool is_deserializable_from_json(const misa_serialization_id &target_id, const nlohmann::json &t_json);

        /**
         * Returns true if the JSON object indicates via JSON hierarchy that it can be deserialized into the target type
         * @tparam Target
         * @param t_json
         * @return
         */
        template<class Target> static bool type_is_deserializable_from_json(const nlohmann::json &t_json) {
            return is_deserializable_from_json(Target().get_serialization_id(), t_json);
        }

    protected:

        /**
         * Override this function and add the serialization ID of the current instance as last element
         * @param result
         */
        virtual void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
            result.emplace_back(misa_serialization_id("misa", "serializable"));
        }
    };
}

// For convenience
#include <misaxx/core/misa_json_schema_property.h>

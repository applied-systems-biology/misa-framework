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

#include <string>
#include <nlohmann/json.hpp>
#include <misaxx/core/misa_data_pattern.h>
#include <misaxx/core/misa_data_description.h>
#include <misaxx/core/misa_serializable.h>
#include <misaxx/core/utils/type_traits.h>
#include <misaxx/core/attachments/misa_locatable.h>

namespace misaxx {
    /**
     * Holds a misaxx::misa_data_pattern and/or a misaxx:misa_data_description
     * This storage also holds JSON data, allowing dynamic deserialization if needed
     */
    struct misa_description_storage : public misa_locatable {

        /**
         * Name of this cache
         * This is put into the schema and serialized JSON
         */
        std::string documentation_title;

        /**
         * Description of this cache
         * This is put into the schema and serialized JSON
         */
        std::string documentation_description;

        misa_description_storage() = default;

        explicit misa_description_storage(std::shared_ptr<misa_data_pattern> t_pattern, std::shared_ptr<misa_data_description> t_description);

        misa_description_storage(const misa_description_storage &t_source);

        misa_description_storage(misa_description_storage && t_source) = default;

        misa_description_storage &operator =(const misa_description_storage &t_source);

        misa_description_storage &operator =(misa_description_storage &&t_source) = default;

        /**
         * Clones this metadata
         * @return
         */
        std::shared_ptr<misa_description_storage> clone() const;

        /**
         * Loads from an imported JSON where all the contents in the JSON can be pattern information.
         * If a pattern or description are existing, the standard deserialization is used.
         * @param t_json
         */
        void from_imported_json(const nlohmann::json &t_json);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;

    public:

        /**
         * Gets a description from this metadata instance. It is deserialized from the
         * internal JSON if needed.
         * @tparam Metadata
         * @return
         */
        template <class Metadata> const Metadata &get() const {
            if constexpr (std::is_base_of<misa_data_pattern, Metadata>::value) {
                if (static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_pattern))) {
                    return *std::dynamic_pointer_cast<Metadata>(m_pattern);
                } else if (misa_serializable::type_is_deserializable_from_json<Metadata>(m_raw_pattern_json)) {
                    Metadata m;
                    m.from_json(m_raw_pattern_json);
                    m_pattern = std::make_shared<Metadata>(std::move(m));
                    return *std::dynamic_pointer_cast<Metadata>(m_pattern);
                }
                else {
                    throw std::runtime_error(std::string("The description storage does not contain ") + typeid(Metadata).name());
                }
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                if (static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_description))) {
                    return *std::dynamic_pointer_cast<Metadata>(m_description);
                } else if (misa_serializable::type_is_deserializable_from_json<Metadata>(m_raw_description_json)) {
                    Metadata m;
                    m.from_json(m_raw_description_json);
                    m_description = std::make_shared<Metadata>(std::move(m));
                    return *std::dynamic_pointer_cast<Metadata>(m_description);
                }
                else {
                    throw std::runtime_error(std::string("The description storage does not contain ") + typeid(Metadata).name());
                }
            }
            else {
                static_assert(misaxx::utils::always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }
        }

        /**
        * Gets a description from this metadata instance. It is deserialized from the
        * internal JSON if needed.
        * @tparam Metadata
        * @return
        */
        template <class Metadata> Metadata &get() {
            if(has<Metadata>()) {
                return access<Metadata>();
            }
            else {
                throw std::runtime_error(std::string("The description storage does not contain ") + typeid(Metadata).name());
            };
        }

        /**
         * Sets a description. Existing descriptions are overwritten
         * @tparam Metadata
         * @return
         */
        template <class Metadata> Metadata &set(Metadata t_description) {
            if constexpr (std::is_base_of<misa_data_pattern, Metadata>::value) {
                m_pattern = std::make_shared<Metadata>(std::move(t_description));
                return *std::dynamic_pointer_cast<Metadata>(m_pattern);
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                m_description = std::make_shared<Metadata>(std::move(t_description));
                return *std::dynamic_pointer_cast<Metadata>(m_description);
            }
            else {
                static_assert(misaxx::utils::always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }
        }

        /**
        * Returns the pattern or description. If necessary, create it.
        * If there is already an existing pattern or description with a differnt type hierarchy, the stored value is overwritten
        * @tparam Metadata
        * @return
        */
        template <class Metadata> Metadata &access() {

            std::vector<misa_serialization_id> serialization_hierarchy = Metadata().get_serialization_id_hierarchy();

            if constexpr (std::is_base_of<misa_data_pattern, Metadata>::value) {
                if (static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_pattern))) {
                    return *std::dynamic_pointer_cast<Metadata>(m_pattern);
                } else {
                    Metadata m;
                    if (misa_serializable::type_is_deserializable_from_json<Metadata>(m_raw_pattern_json)) {
                        m.from_json(m_raw_pattern_json);
                    }
                    m_pattern = std::make_shared<Metadata>(std::move(m));
                    return *std::dynamic_pointer_cast<Metadata>(m_pattern);
                }
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                if (static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_description))) {
                    return *std::dynamic_pointer_cast<Metadata>(m_description);
                } else {
                    Metadata m;
                    if (misa_serializable::type_is_deserializable_from_json<Metadata>(m_raw_description_json)) {
                        m.from_json(m_raw_description_json);
                    }
                    m_description = std::make_shared<Metadata>(std::move(m));
                    return *std::dynamic_pointer_cast<Metadata>(m_description);
                }
            }
            else {
                static_assert(misaxx::utils::always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }
        }

        /**
        * Returns true if the storage or the underlying JSON data contains a specific pattern or description
        * @tparam Metadata
        * @return
        */
        template <class Metadata> bool has() const {
            if constexpr (std::is_base_of<misa_data_pattern, Metadata>::value) {
                if(static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_pattern))) {
                    return true;
                }
                else {
                    return misa_serializable::type_is_deserializable_from_json<Metadata>(m_raw_pattern_json);
                }
            }
            else if constexpr (std::is_base_of<misa_data_description, Metadata>::value) {
                if(static_cast<bool>(std::dynamic_pointer_cast<Metadata>(m_description))) {
                    return true;
                }
                else {
                    return misa_serializable::type_is_deserializable_from_json<Metadata>(m_raw_description_json);
                }
            }
            else {
                static_assert(misaxx::utils::always_false<Metadata>::value, "Only patterns and descriptions are supported!");
            }
        }

        /**
         * Returns true if the description storage has a pattern
         * @return
         */
        bool has_pattern() const;

        /**
         * Returns true if the description storage has a description
         * @return
         */
        bool has_description() const;

    private:

        /**
         * Raw JSON carried along from the settings to allow flexible importing (upgrading) of information during processing
         */
        nlohmann::json m_raw_pattern_json;

        /**
        * Raw JSON carried along from the settings to allow flexible importing (upgrading) of information during processing
        */
        nlohmann::json m_raw_description_json;

        mutable std::shared_ptr<misa_data_pattern> m_pattern;

        mutable std::shared_ptr<misa_data_description> m_description;

    public:

       /**
        * Convenience function that creates a description storage initialized with the provided values
        * @tparam Arg
        * @tparam Args
        * @param arg
        * @param args
        * @return
        */
        template<typename Arg, typename... Args> static std::shared_ptr<misa_description_storage> with(Arg &&arg, Args&&... args) {
            using arg_type = typename std::remove_reference<Arg>::type;
            static_assert(std::is_base_of<misa_data_description, arg_type>::value || std::is_base_of<misa_data_pattern, arg_type>::value,
                          "Only patterns and descriptions can be attached!");
            if constexpr (sizeof...(Args) > 0) {
                auto storage = misa_description_storage::with(std::forward<Args>(args)...);
                storage->set(std::forward<Arg>(arg));
                return storage;
            }
            else {
                auto storage = std::make_shared<misa_description_storage>();
                storage->set(std::forward<Arg>(arg));
                return storage;
            }
        }

    };

    inline void to_json(nlohmann::json& j, const misa_description_storage& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_description_storage& p) {
        p.from_json(j);
    }
}

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

#include <typeindex>
#include <utility>
#include <type_traits>
#include <memory>
#include <unordered_map>

namespace misaxx::utils {
    /**
     * Map from type to value of this type.
     * @tparam BaseType
     */
    template<class BaseType>
    class dynamic_singleton_map {
    public:
        using storage_t = std::unordered_map<std::type_index, std::unique_ptr<BaseType>>;
        using iterator = typename storage_t ::iterator ;
        using const_iterator = typename storage_t ::const_iterator ;

        template<class Type>
        const Type &at() const {
            return *dynamic_cast<const Type *>(m_storage.at(typeid(Type)).get());
        }

        template<class Type>
        Type &at() {
            return *dynamic_cast<Type *>(m_storage.at(typeid(Type)).get());
        }

        template<class Type>
        void insert(Type &&t_src) {
            using src_type = typename std::remove_cv<typename std::remove_reference<Type>::type>::type;
            m_storage.emplace(std::piecewise_construct,
                              std::forward_as_tuple<std::type_index>(typeid(Type)),
                              std::forward_as_tuple(std::unique_ptr<BaseType>(new src_type(std::forward<Type>(t_src)))));
        }

        template<class Type>
        const_iterator find() const {
            return m_storage.find(typeid(Type));
        }

        template<class Type>
        iterator find() {
            return m_storage.find(typeid(Type));
        }

        iterator begin() {
            return m_storage.begin();
        }

        const_iterator begin() const {
            return m_storage.begin();
        }

        iterator end() {
            return m_storage.end();
        }

        const_iterator end() const {
            return m_storage.end();
        }

        /**
         * Equivalent of the [] operator in a map
         * @tparam Type
         * @return
         */
        template<class Type>
        Type &access() {
            const auto it = m_storage.find(typeid(Type));
            if (it != m_storage.end()) {
                return *dynamic_cast<Type *>(it->second.get());
            } else {
                insert(Type());
                return at<Type>();
            }
        }

        template<class Type> bool has() const {
            return find<Type>() != end();
        }

        bool empty() const {
            return m_storage.empty();
        }

    private:

        storage_t m_storage;
    };
}
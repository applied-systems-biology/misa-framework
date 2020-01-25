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
#include <exception>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>
#include <boost/operators.hpp>
#include <string>

namespace misaxx {

    /**
     * Contains an unique id that identifies a serialized class
     */
    struct misa_serialization_id : public boost::equality_comparable<misa_serialization_id> {

        /**
         * Creates an empty serialization ID
         */
        explicit misa_serialization_id();

        /**
         * Creates a serialization ID from a module ID and a path
         * @param t_module
         * @param t_path
         */
        explicit misa_serialization_id(const std::string &t_module, const boost::filesystem::path &t_path);

        /**
         * Creates a serialization ID
         * @param t_id must have the form <module>:<path>
         */
        explicit misa_serialization_id(std::string t_id);

        /**
         * Returns the operator as string
         * @return
         */
        explicit operator std::string() const {
            return id;
        }

        /**
         * Returns the module string
         * @return
         */
        std::string get_module() const;

        /**
         * Returns the path within the module
         * @return
         */
        boost::filesystem::path get_path() const;

        /**
         * Sets the module of the serialization ID
         * @param module
         */
        void set_module(const std::string &module);

        /**
         * Sets the path of the serialization ID
         * @param t_path
         */
        void set_path(const boost::filesystem::path &t_path);

        /**
         * Returns the ID as string
         * @return
         */
        const std::string &get_id() const;

        void set_id(std::string t_id);

        bool empty() const;

        bool operator==(const misa_serialization_id &rhs) const {
            return id == rhs.id;
        }

    private:

        std::string id;
    };

    inline void to_json(nlohmann::json& j, const misa_serialization_id& p) {
        j = p.get_id();
    }

    inline void from_json(const nlohmann::json& j, misa_serialization_id& p) {
        p = misa_serialization_id(j.get<std::string>());
    }

}

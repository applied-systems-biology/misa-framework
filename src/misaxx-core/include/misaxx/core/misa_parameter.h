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


#include <misaxx/core/runtime/misa_parameter_registry.h>
#include <misaxx/core/misa_parameter_base.h>

namespace misaxx {
    /**
     * Wrapper around a parameter, including metadata
     * @tparam T
     */
    template<typename T> struct misa_parameter : public misa_parameter_base {

        misa_parameter() = default;

        explicit misa_parameter(const path &t_location) : misa_parameter(t_location, parameter_registry::register_parameter(t_location)) {

        }

        explicit misa_parameter(path t_location, std::shared_ptr<misa_json_schema_property> t_schema) :
                misa_parameter_base(std::move(t_location), std::move(t_schema)) {

        }


        /**
         * Name of the parameter
         * @return
         */
        const std::string &get_name() const override {
            return location[location.size() - 1];
        }

        /**
         * Location of the parameter within the parameter JSON
         * @return
         */
        const path &get_location() const override {
            return location;
        }

        /**
         * Gets the value of this parameter from the parameter file
         * @return
         */
        T query() const {
            return misaxx::parameter_registry::get_json<T>(get_location());
        }

        /**
         * Documents this parameter with a description
         * @param title
         * @return
         */
        misa_parameter<T> &document_title(std::string title) {
            schema->document_title(std::move(title));
            return *this;
        }

        /**
        * Documents this parameter with a name
        * @param title
        * @return
        */
        misa_parameter<T> &document_description(std::string description) {
            schema->document_description(std::move(description));
            return *this;
        }
    };
}

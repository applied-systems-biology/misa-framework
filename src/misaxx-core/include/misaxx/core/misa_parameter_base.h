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
#include <misaxx/core/misa_json_schema_property.h>

namespace misaxx {
    /**
     * Base class of a parameter
     */
    struct misa_parameter_base {

        /**
         * Path type that is used for parameters
         */
        using path = std::vector<std::string>;

        /**
        * Location of the parameter within the parameter JSON
        */
        path location;

        /**
         * Parameter schema that belongs to the parameter
         */
        std::shared_ptr<misa_json_schema_property> schema;

        misa_parameter_base() = default;

        explicit misa_parameter_base(path t_location, std::shared_ptr<misa_json_schema_property> t_schema);

        /**
         * Returns the name of this parameter
         * @return
         */
        virtual const std::string &get_name() const = 0;

        /**
         * Returns the path of this parameter within the parameter file
         * @return
         */
        virtual const path &get_location() const = 0;

    };
}

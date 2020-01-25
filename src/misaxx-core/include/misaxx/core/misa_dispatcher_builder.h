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


#include <misaxx/core/misa_dispatcher_blueprint_list.h>

namespace misaxx {
    /**
     * Allows instantiation of a specific subtree supported by a misa_dispatcher_blueprint
     */
    struct misa_dispatcher_builder : public misa_dispatcher_blueprint_list {
        using misa_dispatcher_blueprint_list::misa_dispatcher_blueprint_list;

        /**
         * Instantiates a blueprint
         * @tparam Instance
         * @param t_key
         * @return
         */
        template<class Instance> Instance &build(const std::string &t_key) const {
            return dynamic_cast<Instance&>(this->get(t_key)->dispatch(this->get_worker()));
        }
    };
}


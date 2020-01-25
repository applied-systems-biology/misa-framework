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
#include <unordered_map>
#include <typeindex>
#include <misaxx/core/misa_dispatch_blueprint.h>
#include <memory>

namespace misaxx {

    class misa_dispatcher;

    /**
     * Describes how the maximum subtree of a misa_dispatcher looks like
     */
    class misa_dispatcher_blueprint_list {
    public:

        using blueprint = std::shared_ptr<misa_dispatch_blueprint_base>;

        explicit misa_dispatcher_blueprint_list(misa_dispatcher &t_worker);

        /**
         * Adds an entry to this blueprint list
         * @param t_dispatched
         */
        void add(blueprint t_blueprint);

        /**
         * Adds multiple entries to the blueprint list
         * @param t_blueprints
         */
        void add(std::vector<blueprint> t_blueprints);

        /**
         * Returns a blueprint by its name
         * @param t_key
         * @return
         */
        blueprint get(const std::string &t_key) const;

        /**
         * Returns the stored blueprints
         * @return
         */
        const std::unordered_map<std::string, blueprint> &get_entries() const;

        /**
         * Returns the worker this blueprint list is attached to
         * @return
         */
        misa_dispatcher &get_worker() const;

    private:

        std::weak_ptr<misa_dispatcher> m_worker;

        /**
         * Allows checking if the the code initializes the correct types for child nodes
         */
        std::unordered_map<std::string, blueprint> m_blueprints;

    };
}





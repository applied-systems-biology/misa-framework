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

#include <misaxx/core/workers/misa_work_node.h>
#include <unordered_set>

namespace misaxx {

    /**
     * Dependencies that are passed around
     */
    using depencencies_t = std::unordered_set<std::shared_ptr<misa_work_node>>;

    /**
     * Represents an item in a misaxx::depencencies::chain
     */
    struct misa_work_dependency_segment {

        /**
        * Dependencies of this specific segment
        * @return
        */
        virtual depencencies_t dependencies() const = 0;

       /**
        * If another segment should depend from this one, the dependencies in the return value must be statisfied.
        * This is used for interaction between segments
        * @return
        */
        virtual depencencies_t to_dependencies() = 0;
    };

}
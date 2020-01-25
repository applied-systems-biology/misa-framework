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

namespace misaxx {

    /**
     * Status of the node's subtree
     */
    enum misa_work_subtree_status : int {
        /**
         * Not all children are instantiated.
         * The runtime must keep track of this node and update it constantly until the node reports completion
         */
                incomplete,
        /**
         * All children are instantiated.
         * The runtime is not required anymore to check for changes in children
         */
                complete
    };

}
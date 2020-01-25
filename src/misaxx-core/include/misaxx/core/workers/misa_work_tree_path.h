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

#include <memory>
#include <vector>
#include <optional>
#include <ostream>
#include <boost/algorithm/string/join.hpp>

namespace misaxx {

    class misa_work_node;

    /**
     * Base class for worker tree node paths
     */
    struct misa_work_tree_path {

        /**
         * Returns the hierarchy of nodes
         * @return
         */
        virtual const std::vector<std::weak_ptr<const misa_work_node>> &get_node_path() const = 0;

        /**
         * Returns the node names
         * @return
         */
        virtual const std::vector<std::string> &get_path() const = 0;

        friend std::ostream &operator<<(std::ostream &os, const misa_work_tree_path &path) {
            os << boost::algorithm::join(path.get_path(), "/");
            return os;
        }
    };

}

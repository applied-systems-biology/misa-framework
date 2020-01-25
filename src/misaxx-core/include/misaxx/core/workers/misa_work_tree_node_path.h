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

#include <misaxx/core/workers/misa_work_tree_path.h>

namespace misaxx {
    /**
     * Full path of a node
     * Should not be used for JSON
     */
    class misa_work_tree_node_path : public misa_work_tree_path {
    public:
        misa_work_tree_node_path() = default;

        explicit misa_work_tree_node_path(const std::shared_ptr<const misa_work_node> &t_node);

        const std::vector<std::weak_ptr<const misa_work_node>> &get_node_path() const override;

        const std::vector<std::string> &get_path() const override;

        friend std::ostream &operator<<(std::ostream &os, const misa_work_tree_node_path &path) {
            os << boost::algorithm::join(path.get_path(), "/");
            return os;
        }

    private:

        std::vector<std::weak_ptr<const misa_work_node>> m_node_path;
        std::vector<std::string> m_path;
    };
}

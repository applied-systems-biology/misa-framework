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

#include <misaxx/core/misa_task.h>
#include <misaxx/core/accessors/misa_json.h>

namespace misaxx_analyzer {

    struct attachment_indexer_discover_result {
        int database_id = -1;
//        std::string title;
//        std::string description;
    };

    struct attachment_indexer_task : public misaxx::misa_task {

        using misaxx::misa_task::misa_task;

        void work() override;

        void create_parameters(misaxx::misa_parameter_builder &t_parameters) override;

        attachment_indexer_discover_result discover(nlohmann::json &json,
                const std::vector<std::string> &path, misaxx::readwrite_access<attachment_index_database> &db,
                const std::string &sample,
                const std::string &cache);
    };
}





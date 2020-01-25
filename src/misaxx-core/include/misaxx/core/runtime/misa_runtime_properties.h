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
#include <nlohmann/json.hpp>
#include <misaxx/core/misa_module_info.h>

namespace misaxx {
    struct misa_cache;
    struct misa_json_schema_builder;
    struct misa_work_node;
    struct misa_filesystem;
}

/**
 * Helper namespace that allows access to some properties of the current runtime
 */
namespace misaxx::runtime_properties {

    /**
     * Returns true if the runtime is set to request skipping as much work as possible
     * @return
     */
    extern bool requested_skipping();

    /**
     * Returns the number of threads
     * @return
     */
    extern int get_num_threads();

    /**
     * If true, the runtime is in simulation mode and no actual work should be done
     * @return
     */
    extern bool is_simulating();

    /**
     * Returns true if the runtime is currently working
     * @return
     */
    extern bool is_running();

    /**
     * Returns the root node of the worker tree
     * @return
     */
    extern std::shared_ptr<misa_work_node> get_root_node();

    /**
     * Returns the filesystem of the root module
     * Please note that the root module is usually misa_multiobject_root.
     * @return
     */
    extern misa_filesystem get_root_filesystem();

    /**
     * Returns the module information that was attached to the runtime
     * @return
     */
    extern misa_module_info get_module_info();
}
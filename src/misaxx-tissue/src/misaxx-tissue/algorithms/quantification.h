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
#include <src/misaxx-tissue/algorithms/segmentation3d/segmentation3d_base.h>
#include <misaxx-tissue/module_interface.h>

namespace misaxx_tissue {
    struct quantification : public misaxx::misa_task {
        using misaxx::misa_task::misa_task;
        void work() override;
    };
}

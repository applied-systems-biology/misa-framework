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

namespace misaxx_kidney_glomeruli {
    struct glomeruli_filtering : public misaxx::misa_task{

        parameter<bool> m_enable_label_filtering;
        parameter<bool> m_enable_quantification_filtering;
        parameter<bool> m_invert;

        using misaxx::misa_task::misa_task;

        void work() override;

        void create_parameters(misaxx::misa_parameter_builder &t_parameters) override;
    };
}





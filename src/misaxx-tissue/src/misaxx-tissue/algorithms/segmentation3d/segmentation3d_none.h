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

#include "segmentation3d_base.h"

namespace misaxx_tissue {

    /**
     * No 3d segmentation. Copies the data from 2d to 3d
     */
    struct segmentation3d_none : public segmentation3d_base {

        using segmentation3d_base::segmentation3d_base;

        void work() override;
    };
}
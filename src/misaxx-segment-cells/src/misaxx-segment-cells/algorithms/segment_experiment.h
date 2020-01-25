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
#include <misaxx/imaging/accessors/misa_image_file.h>

namespace misaxx_segment_cells {
    struct segment_experiment : public misaxx::misa_task {
        using misaxx::misa_task::misa_task;

        misaxx::imaging::misa_image_file m_inputImage;
        misaxx::imaging::misa_image_file m_outputImage;

        void work() override;
    };
}

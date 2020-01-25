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

#include <misaxx/core/misa_module_interface.h>
#include <misaxx/imaging/accessors/misa_image_stack.h>

namespace misaxx_segment_cells {
    struct module_interface : public misaxx::misa_module_interface {

        misaxx::imaging::misa_image_stack m_inputImages;
        misaxx::imaging::misa_image_stack m_outputSegmented;

        void setup() override;
    };
}

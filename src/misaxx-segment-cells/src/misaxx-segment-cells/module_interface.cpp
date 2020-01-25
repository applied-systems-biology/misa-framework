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
#include <misaxx-segment-cells/module_interface.h>

using namespace misaxx_segment_cells;

void module_interface::setup() {
    m_inputImages.suggest_import_location(filesystem, "");
    m_outputSegmented.suggest_export_location(filesystem, "", m_inputImages.describe());
}

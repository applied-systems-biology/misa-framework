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
#include <misaxx-deconvolve/module_interface.h>

using namespace misaxx_deconvolve;

void module_interface::setup() {
    m_input_image.suggest_import_location(filesystem, "in");
    m_input_psf.suggest_import_location(filesystem, "psf");
    m_output_convolved.suggest_export_location(filesystem, "convolved", m_input_image.describe());
    m_output_deconvolved.suggest_export_location(filesystem, "deconvolved", m_input_image.describe());
}

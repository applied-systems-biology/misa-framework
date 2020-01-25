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
#include <misaxx-microbench/module_interface.h>

using namespace misaxx_microbench;

void module_interface::setup() {
    m_input_image.suggest_import_location(filesystem, "in");
    m_output_median.suggest_export_location(filesystem, "median_filtered", m_input_image.describe());
    m_output_morphology.suggest_export_location(filesystem, "dilated", m_input_image.describe());
    m_output_fft_ifft.suggest_export_location(filesystem, "fft_ifft", m_input_image.describe());
    m_output_otsu.suggest_export_location(filesystem, "otsu", m_input_image.describe());
    m_output_percentile.suggest_export_location(filesystem, "percentile", m_input_image.describe());
    m_output_canny.suggest_export_location(filesystem, "canny_edges", m_input_image.describe());
    m_output_wiener.suggest_export_location(filesystem, "wiener2", m_input_image.describe());
    m_runtimes.suggest_export_location(filesystem, "runtimes/runtimes.json");
}

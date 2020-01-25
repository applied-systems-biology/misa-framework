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

#include <misaxx-ome-visualizer/module_info.h>
#include <misaxx-ome-visualizer/module_interface.h>


using namespace misaxx;
using namespace misaxx_ome_visualizer;

void misaxx_ome_visualizer::module_interface::setup() {
    m_input.suggest_import_location(filesystem, "/");
    m_output.suggest_export_location(filesystem, "/", m_input.derive().of_opencv(CV_8UC3));
}

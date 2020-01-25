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

#include <misaxx-tissue/module_interface.h>

using namespace misaxx;
using namespace misaxx::ome;
using namespace misaxx_tissue;

void module_interface::setup() {
    m_input_autofluorescence.suggest_import_location(filesystem, "/");
    m_input_autofluorescence.suggest_document_title("Raw images");
    m_input_autofluorescence.suggest_document_description("Grayscale OME TIFF stack of raw images");

    m_output_segmented.suggest_export_location(filesystem, "segmented", m_input_autofluorescence.derive().of_opencv(CV_8U));
    m_output_segmented.suggest_document_title("Segmented tissue");
    m_output_segmented.suggest_document_description("8-bit binary image of segmented tissue");

    m_output_quantification.suggest_export_location(filesystem, "quantified/quantified.json");
    m_output_quantification.suggest_document_title("Quantification results");
}

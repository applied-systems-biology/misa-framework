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

#include <misaxx-kidney-glomeruli/module_interface.h>

using namespace misaxx;
using namespace misaxx_kidney_glomeruli;

glomeruli module_interface::get_glomeruli() {
    return m_output_quantification.get_attachment<glomeruli>();
}

void module_interface::setup() {
    m_input_autofluorescence.suggest_import_location(filesystem, "/");
    m_input_autofluorescence.suggest_document_title("Raw images");
    m_input_autofluorescence.suggest_document_description("Grayscale OME TIFF file containg the raw images");

    m_output_segmented2d.suggest_export_location(filesystem, "glomeruli2d", m_input_autofluorescence.derive().of_opencv(CV_8U));
    m_output_segmented2d.suggest_document_title("2D segmented glomeruli");
    m_output_segmented2d.suggest_document_description("Unfiltered glomeruli detected via 2D segmentation");

    m_output_segmented3d.suggest_export_location(filesystem, "glomeruli3d", m_output_segmented2d.derive().of_opencv(CV_32S));
    m_output_segmented3d.suggest_document_title("3D segmented glomeruli");
    m_output_segmented3d.suggest_document_description("2D detected glomeruli filtered by the 3D segmentation algorithm");

    m_output_quantification.suggest_export_location(filesystem, "quantified/quantified.json");
    m_output_quantification.suggest_document_title("Quantification results");

    // Init the submodule
    m_tissue = std::make_shared<misaxx_tissue::module_interface>();
    m_tissue->m_input_autofluorescence = m_input_autofluorescence;
}

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

#include <misaxx/core/misa_module_interface.h>
#include <misaxx/core/accessors/misa_exported_attachments.h>
#include <misaxx/ome/accessors/misa_ome_tiff.h>
#include <misaxx-kidney-glomeruli/attachments/glomeruli.h>
#include <misaxx-tissue/module_interface.h>
#include <misaxx/ome/attachments/misa_ome_voxel_size.h>

namespace misaxx_kidney_glomeruli {
    struct module_interface : public misaxx::misa_module_interface {

        misaxx::ome::misa_ome_tiff m_input_autofluorescence;
        misaxx::ome::misa_ome_tiff m_output_segmented2d;
        misaxx::ome::misa_ome_tiff m_output_segmented3d;
        misaxx::misa_exported_attachments m_output_quantification;

        /**
         * Stores the result of the tissue detection
         */
        std::shared_ptr <misaxx_tissue::module_interface> m_tissue;

        /**
         * Voxel size obtained from input image
         */
        misaxx::ome::misa_ome_voxel_size m_voxel_size;

        void setup() override;

        glomeruli get_glomeruli();
    };
}
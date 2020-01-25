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

#include <misaxx/core/misa_cached_data.h>
#include <misaxx/core/accessors/misa_exported_attachments.h>
#include <misaxx/ome/accessors/misa_ome_tiff.h>
#include <misaxx/ome/attachments/misa_ome_unit.h>
#include <misaxx/core/attachments/misa_matrix.h>
#include <misaxx/ome/attachments/misa_ome_voxel_size.h>
#include <misaxx/core/misa_module_interface.h>

namespace misaxx_tissue {
    struct module_interface : public misaxx::misa_module_interface {
        misaxx::ome::misa_ome_tiff m_input_autofluorescence;
        misaxx::ome::misa_ome_tiff m_output_segmented;
        misaxx::misa_exported_attachments m_output_quantification;

        misaxx::ome::misa_ome_voxel_size m_voxel_size;

        void setup() override;
    };
}
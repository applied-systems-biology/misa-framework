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

#include <ome/xml/model/enums/PixelType.h>
#include <ome/files/Types.h>
#include <ome/files/MetadataTools.h>

namespace misaxx::ome::helpers {

    extern std::shared_ptr<::ome::files::CoreMetadata>
    create_ome_core_metadata(const ::ome::xml::meta::OMEXMLMetadata &t_metadata, size_t series);

    extern std::shared_ptr<::ome::files::CoreMetadata>
    create_ome_core_metadata(size_t size_X, size_t size_Y, size_t size_Z, size_t size_T, std::vector<size_t> size_C, ::ome::xml::model::enums::PixelType pixel_type);

    extern std::shared_ptr<::ome::xml::meta::OMEXMLMetadata>
    create_ome_xml_metadata(size_t size_X, size_t size_Y, size_t size_Z, size_t size_T, std::vector<size_t> size_C, ::ome::xml::model::enums::PixelType pixel_type);
}
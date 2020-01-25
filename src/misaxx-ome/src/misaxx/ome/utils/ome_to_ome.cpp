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

#include "ome_to_ome.h"

using namespace misaxx::ome;

void misaxx::ome::ome_to_ome(const ::ome::files::FormatReader &ome_reader, const misa_ome_plane_description &input_index,
                ::ome::files::out::OMETIFFWriter &ome_writer, const misa_ome_plane_description &output_index) {
    using namespace ::ome::xml::model::enums;
    switch(ome_reader.getPixelType()) {
        case PixelType::UINT8: {
            ome_to_ome_detail<PixelType::UINT8>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        case PixelType::INT8: {
            ome_to_ome_detail<PixelType::INT8>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        case PixelType::UINT16: {
            ome_to_ome_detail<PixelType::UINT16>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        case PixelType::INT16: {
            ome_to_ome_detail<PixelType::INT16>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        case PixelType::UINT32: {
            ome_to_ome_detail<PixelType::UINT32>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        case PixelType::INT32: {
            ome_to_ome_detail<PixelType::INT32>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        case PixelType::FLOAT: {
            ome_to_ome_detail<PixelType::FLOAT>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        case PixelType::DOUBLE: {
            ome_to_ome_detail<PixelType::DOUBLE>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        case PixelType::COMPLEXFLOAT: {
            ome_to_ome_detail<PixelType::COMPLEXFLOAT>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        case PixelType::COMPLEXDOUBLE: {
            ome_to_ome_detail<PixelType::COMPLEXDOUBLE>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        case PixelType::BIT: {
            ome_to_ome_detail<PixelType::BIT>(ome_reader, input_index, ome_writer, output_index);
            return;
        }
        default:
            throw std::runtime_error("Unsupported pixel type!");
    }
}

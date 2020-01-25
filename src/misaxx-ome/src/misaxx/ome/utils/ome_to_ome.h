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

#include <ome/files/out/OMETIFFWriter.h>
#include <misaxx/ome/descriptions/misa_ome_plane_description.h>
#include <ome/files/VariantPixelBuffer.h>

namespace misaxx::ome {

    template<int OMEPixelType> inline void ome_to_ome_detail (const ::ome::files::FormatReader &ome_reader,
                                                                        const misa_ome_plane_description &input_index,
                                                                        ::ome::files::out::OMETIFFWriter &ome_writer,
                                                                        const misa_ome_plane_description &output_index) {
        using namespace ::ome::files;
        using namespace ::ome::xml::model::enums;

        const auto size_x = ome_reader.getSizeX();
        const auto size_y = ome_reader.getSizeY();
        const auto channels = ome_reader.getRGBChannelCount(input_index.c);
        auto write_buffer = std::make_shared<PixelBuffer<typename PixelProperties<OMEPixelType>::std_type>> (boost::extents[size_x][size_y][1][1][1][channels][1][1][1],
                                                                                                       ome_reader.getPixelType(),
                                                                                                       ::ome::files::ENDIAN_NATIVE,
                                                                                                       PixelBufferBase::make_storage_order(DimensionOrder::XYZTC, false));

        VariantPixelBuffer read_buffer;
        ome_reader.openBytes(input_index.index_within(ome_reader), read_buffer);
        const auto &src_array = read_buffer.array<typename PixelProperties<OMEPixelType>::std_type>();

        PixelBufferBase::indices_type idx;
        std::fill(idx.begin(), idx.end(), 0);

        for(size_t y = 0; y < size_y; ++y) {
            for(size_t x = 0; x < size_x; ++x) {
                idx[DIM_SPATIAL_X] = x;
                idx[DIM_SPATIAL_Y] = y;
                for(size_t c = 0; c < channels; ++c) {
                    idx[DIM_SUBCHANNEL] = c;
                    write_buffer->at(idx) = src_array(idx);
                }
            }
        }

        VariantPixelBuffer write_vbuffer(write_buffer);
        const auto plane_index = output_index.index_within(ome_writer);
        ome_writer.saveBytes(plane_index, write_vbuffer);
    }

    /**
     * Writes from an OME Format reader to an OME TIFF
     * @param ome_reader
     * @param input_index
     * @param ome_writer
     * @param output_index
     */
    extern void ome_to_ome(const ::ome::files::FormatReader &ome_reader,
            const misa_ome_plane_description &input_index,
            ::ome::files::out::OMETIFFWriter &ome_writer,
            const misa_ome_plane_description &output_index);
}
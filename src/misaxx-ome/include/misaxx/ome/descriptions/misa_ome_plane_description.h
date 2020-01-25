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

#include <ome/files/Types.h>
#include <ome/files/detail/FormatReader.h>
#include <ome/files/FormatWriter.h>
#include <ome/files/out/OMETIFFWriter.h>
#include <misaxx/core/misa_data_description.h>
#include <ostream>
#include <boost/operators.hpp>
#include <misaxx/core/misa_json_schema_property.h>

namespace misaxx::ome {

    /**
     * Describes the location of an image plane (2D image) within an OME TIFF
     */
    struct misa_ome_plane_description
            : public misaxx::misa_data_description,
              private boost::equality_comparable<misa_ome_plane_description, boost::partially_ordered<misa_ome_plane_description>> {

        /**
         * The series of images the target image belongs to.
         * All planes (2D images) within a series have the same X-Y size.
         * There are |Z| * |C| * |T| planes within a series.
         */
        ::ome::files::dimension_size_type series = 0;
        /**
         * The Z location (depth) the target plane (2D image) is assigned to
         */
        ::ome::files::dimension_size_type z = 0;
        /**
         * The Channel the target plane (2D image) is assigned to
         */
        ::ome::files::dimension_size_type c = 0;
        /**
         * The time location the target plane (2D image) is assigned to
         */
        ::ome::files::dimension_size_type t = 0;

        misa_ome_plane_description() = default;

        explicit misa_ome_plane_description(::ome::files::dimension_size_type t_series);

        explicit misa_ome_plane_description(::ome::files::dimension_size_type t_series,
                                            ::ome::files::dimension_size_type t_z,
                                            ::ome::files::dimension_size_type t_c,
                                            ::ome::files::dimension_size_type t_t);

        /**
         * Converts the data into a OME native ZCT coordinate.
         * The information about the series is not used!
         * @return
         */
        std::array<::ome::files::dimension_size_type, 3> as_zct() const;

        /**
         * Converts the data into a plane index within an OME Format reader
         * @param reader
         * @return
         */
        ::ome::files::dimension_size_type index_within(const ::ome::files::FormatReader &reader) const;

        ::ome::files::dimension_size_type index_within(const ::ome::files::out::OMETIFFWriter &writer) const;

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misaxx::misa_json_schema_property &t_schema) const override;

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const override;

    public:

        bool operator==(const misa_ome_plane_description &rhs) const;

        bool operator<(const misa_ome_plane_description &rhs) const;

        friend std::ostream &operator<<(std::ostream &os, const misa_ome_plane_description &description) {
            os << "S" << description.series << "_Z" << description.z << "_C" << description.c << "_T" << description.t;
            return os;
        }
    };

    inline void to_json(nlohmann::json &j, const misa_ome_plane_description &p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json &j, misa_ome_plane_description &p) {
        p.from_json(j);
    }
}

namespace std {
    template<>
    struct hash<misaxx::ome::misa_ome_plane_description> {
        size_t operator()(const misaxx::ome::misa_ome_plane_description &x) const {
            const auto H = hash<::ome::files::dimension_size_type>();
            return H(x.series) + H(x.z) + H(x.c) + H(x.t);
        }
    };
}

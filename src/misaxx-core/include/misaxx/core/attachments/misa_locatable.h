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

#include <type_traits>
#include <misaxx/core/attachments/misa_location.h>

namespace misaxx {

    /**
     * Base type for all attachments that can be located.
     * It has a method get_location() that returns a location instance of given type if the data is present
     * @tparam Location
     */
    struct misa_locatable : public misa_serializable {

        misa_locatable() = default;

        explicit misa_locatable(std::shared_ptr<const misa_location> t_location);

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        /**
         * Returns the location as the specified class
         * @tparam Location
         * @return
         */
        template<class Location>
        std::shared_ptr<const Location> get_location();

        /**
         * Returns true if this locatable has the location
         * @tparam Location
         * @return
         */
        template<class Location>
        bool has_location();

        /**
         * Sets the location of this locatable
         * @param location
         */
        void set_location(std::shared_ptr<const misa_location> location);

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    private:
        /**
        * Pointer to the location.
        * The location is obtained from a cache (or created manually)
        */
        std::shared_ptr<const misa_location> m_location;
        /**
         * If deserialized from JSON
         */
        nlohmann::json m_location_json;
    };

    inline void to_json(nlohmann::json& j, const misa_locatable& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_locatable& p) {
        p.from_json(j);
    }
}

#include "detail/misa_locatable.h"


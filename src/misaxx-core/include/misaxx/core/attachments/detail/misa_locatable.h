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

namespace misaxx {

    /**
     * Returns the location from the misa_locatable instance.
     * Returns an empty pointer if the location is not set or existing JSON data is not deserializable into the requested type
     * @tparam Location
     * @return
     */
    template<class Location>
    std::shared_ptr<const Location> misa_locatable::get_location() {
        if(static_cast<bool>(m_location)) {
            auto as = std::dynamic_pointer_cast<const Location>(m_location);
            if(static_cast<bool>(as))
                return as;
        }
        // Try to load from JSON
        if(!m_location_json.empty() && misa_serializable::type_is_deserializable_from_json<Location>(m_location_json)) {
            auto loc = std::make_shared<Location>();
            loc->from_json(m_location_json);
            m_location = loc;
            return m_location;
        }
        return std::shared_ptr<const Location>();
    }

    /**
     * Returns if this locatable has a location of given type
     * @tparam Location
     * @return
     */
    template<class Location>
    bool misa_locatable::has_location() {
        if(static_cast<bool>(m_location) && dynamic_cast<const Location*>(m_location.get()))
            return true;
        else
            return !m_location_json.empty() && misa_serializable::type_is_deserializable_from_json<Location>(m_location_json);
    }
}
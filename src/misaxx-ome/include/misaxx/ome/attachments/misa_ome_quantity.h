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

#include <misaxx/core/attachments/misa_quantity.h>
#include <misaxx/ome/attachments/misa_ome_unit.h>

namespace misaxx::ome {
    // Convenience type wrappers
    template<typename Value, size_t Order = 1> using misa_ome_length = misaxx::misa_quantity<Value, misa_ome_unit_length<Order>>;
    template<typename Value, size_t Order = 1> using misa_ome_frequency = misaxx::misa_quantity<Value, misa_ome_unit_frequency<Order>>;
    template<typename Value, size_t Order = 1> using misa_ome_power = misaxx::misa_quantity<Value, misa_ome_unit_power<Order>>;
    template<typename Value, size_t Order = 1> using misa_ome_pressure = misaxx::misa_quantity<Value, misa_ome_unit_pressure<Order>>;
    template<typename Value, size_t Order = 1> using misa_ome_temperature = misaxx::misa_quantity<Value, misa_ome_unit_temperature<Order>>;
    template<typename Value, size_t Order = 1> using misa_ome_time = misaxx::misa_quantity<Value, misa_ome_unit_time<Order>>;

    // More convenience wrappers
    template<typename Value> using misa_ome_area = misa_ome_length<Value, 2>;
    template<typename Value> using misa_ome_volume = misa_ome_length<Value, 3>;
    
}
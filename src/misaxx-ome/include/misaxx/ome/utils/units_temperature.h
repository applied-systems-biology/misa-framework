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

#include <misaxx/ome/attachments/misa_ome_unit.h>

namespace misaxx::ome::units {
    template<int Order = 1> misa_ome_unit_temperature<Order> celsius() {
        return misa_ome_unit_temperature<Order>(::ome::xml::model::enums::UnitsTemperature::CELSIUS);
    }

    template<int Order = 1> misa_ome_unit_temperature<Order> fahrenheit() {
        return misa_ome_unit_temperature<Order>(::ome::xml::model::enums::UnitsTemperature::FAHRENHEIT);
    }

    template<int Order = 1> misa_ome_unit_temperature<Order> kelvin() {
        return misa_ome_unit_temperature<Order>(::ome::xml::model::enums::UnitsTemperature::KELVIN);
    }

    template<int Order = 1> misa_ome_unit_temperature<Order> rankine() {
        return misa_ome_unit_temperature<Order>(::ome::xml::model::enums::UnitsTemperature::RANKINE);
    }
}
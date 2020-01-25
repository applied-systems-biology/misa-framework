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

    template<int Order = 1> misa_ome_unit_electric_potential<Order> yottavolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::YOTTAVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> zettavolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::ZETTAVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> exavolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::EXAVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> petavolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::PETAVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> teravolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::TERAVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> gigavolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::GIGAVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> megavolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::MEGAVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> kilovolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::KILOVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> hectovolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::HECTOVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> decavolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::DECAVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> volt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::VOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> decivolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::DECIVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> centivolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::CENTIVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> millivolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::MILLIVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> microvolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::MICROVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> nanovolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::NANOVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> picovolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::PICOVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> femtovolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::FEMTOVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> attovolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::ATTOVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> zeptovolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::ZEPTOVOLT);
    }

    template<int Order = 1> misa_ome_unit_electric_potential<Order> yoctovolt() {
        return misa_ome_unit_electric_potential<Order>(::ome::xml::model::enums::UnitsElectricPotential::YOCTOVOLT);
    }
}
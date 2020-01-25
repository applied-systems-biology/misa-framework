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
    template<int Order = 1> misa_ome_unit_power<Order> yottawatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::YOTTAWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> zettawatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::ZETTAWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> exawatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::EXAWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> petawatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::PETAWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> terawatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::TERAWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> gigawatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::GIGAWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> megawatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::MEGAWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> kilowatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::KILOWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> hectowatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::HECTOWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> decawatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::DECAWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> watt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::WATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> deciwatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::DECIWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> centiwatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::CENTIWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> milliwatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::MILLIWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> microwatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::MICROWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> nanowatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::NANOWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> picowatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::PICOWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> femtowatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::FEMTOWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> attowatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::ATTOWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> zeptowatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::ZEPTOWATT);
    }

    template<int Order = 1> misa_ome_unit_power<Order> yoctowatt() {
        return misa_ome_unit_power<Order>(::ome::xml::model::enums::UnitsPower::YOCTOWATT);
    }
}
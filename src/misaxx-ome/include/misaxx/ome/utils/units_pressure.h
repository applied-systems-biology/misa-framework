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
    template<int Order = 1> misa_ome_unit_pressure<Order> yottapascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::YOTTAPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> zettapascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::ZETTAPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> exapascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::EXAPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> petapascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::PETAPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> terapascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::TERAPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> gigapascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::GIGAPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> megapascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::MEGAPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> kilopascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::KILOPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> hectopascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::HECTOPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> decapascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::DECAPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> pascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::PASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> decipascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::DECIPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> centipascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::CENTIPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> millipascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::MILLIPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> micropascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::MICROPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> nanopascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::NANOPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> picopascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::PICOPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> femtopascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::FEMTOPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> attopascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::ATTOPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> zeptopascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::ZEPTOPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> yoctopascal() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::YOCTOPASCAL);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> bar() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::BAR);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> megabar() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::MEGABAR);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> kilobar() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::KILOBAR);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> decibar() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::DECIBAR);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> centibar() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::CENTIBAR);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> millibar() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::MILLIBAR);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> atmosphere() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::ATMOSPHERE);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> psi() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::PSI);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> torr() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::TORR);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> millitorr() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::MILLITORR);
    }

    template<int Order = 1> misa_ome_unit_pressure<Order> mmhg() {
        return misa_ome_unit_pressure<Order>(::ome::xml::model::enums::UnitsPressure::MMHG);
    }
}
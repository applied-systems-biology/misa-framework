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
    template<int Order = 1> misa_ome_unit_frequency<Order> yottahertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::YOTTAHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> zettahertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::ZETTAHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> exahertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::EXAHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> petahertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::PETAHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> terahertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::TERAHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> gigahertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::GIGAHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> megahertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::MEGAHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> kilohertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::KILOHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> hectohertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::HECTOHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> decahertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::DECAHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> hertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::HERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> decihertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::DECIHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> centihertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::CENTIHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> millihertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::MILLIHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> microhertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::MICROHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> nanohertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::NANOHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> picohertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::PICOHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> femtohertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::FEMTOHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> attohertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::ATTOHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> zeptohertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::ZEPTOHERTZ);
    }

    template<int Order = 1> misa_ome_unit_frequency<Order> yoctohertz() {
        return misa_ome_unit_frequency<Order>(::ome::xml::model::enums::UnitsFrequency::YOCTOHERTZ);
    }
}
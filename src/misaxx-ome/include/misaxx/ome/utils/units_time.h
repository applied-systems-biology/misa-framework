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
    template<int Order = 1> misa_ome_unit_time<Order> yottasecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::YOTTASECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> zettasecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::ZETTASECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> exasecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::EXASECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> petasecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::PETASECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> terasecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::TERASECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> gigasecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::GIGASECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> megasecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::MEGASECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> kilosecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::KILOSECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> hectosecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::HECTOSECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> decasecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::DECASECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> second() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::SECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> decisecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::DECISECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> centisecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::CENTISECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> millisecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::MILLISECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> microsecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::MICROSECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> nanosecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::NANOSECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> picosecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::PICOSECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> femtosecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::FEMTOSECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> attosecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::ATTOSECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> zeptosecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::ZEPTOSECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> yoctosecond() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::YOCTOSECOND);
    }

    template<int Order = 1> misa_ome_unit_time<Order> minute() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::MINUTE);
    }

    template<int Order = 1> misa_ome_unit_time<Order> hour() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::HOUR);
    }

    template<int Order = 1> misa_ome_unit_time<Order> day() {
        return misa_ome_unit_time<Order>(::ome::xml::model::enums::UnitsTime::DAY);
    }
}
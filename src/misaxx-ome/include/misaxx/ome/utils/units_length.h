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

/**
 * Contains ready-to-use misa_ome_unit instances
 */
namespace misaxx::ome::units {

    template<int Order = 1> misa_ome_unit_length<Order> yottameter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::YOTTAMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> zettameter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::ZETTAMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> exameter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::EXAMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> petameter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::PETAMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> terameter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::TERAMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> gigameter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::GIGAMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> megameter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::MEGAMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> kilometer() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::KILOMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> hectometer() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::HECTOMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> decameter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::DECAMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> meter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::METER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> decimeter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::DECIMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> centimeter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::CENTIMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> millimeter() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::MILLIMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> micrometer() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::MICROMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> nanometer() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::NANOMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> picometer() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::PICOMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> femtometer() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::FEMTOMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> attometer() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::ATTOMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> zeptometer() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::ZEPTOMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> yoctometer() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::YOCTOMETER);
    }

    template<int Order = 1> misa_ome_unit_length<Order> angstrom() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::ANGSTROM);
    }

    template<int Order = 1> misa_ome_unit_length<Order> thou() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::THOU);
    }

    template<int Order = 1> misa_ome_unit_length<Order> line() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::LINE);
    }

    template<int Order = 1> misa_ome_unit_length<Order> inch() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::INCH);
    }

    template<int Order = 1> misa_ome_unit_length<Order> foot() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::FOOT);
    }

    template<int Order = 1> misa_ome_unit_length<Order> yard() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::YARD);
    }

    template<int Order = 1> misa_ome_unit_length<Order> mile() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::MILE);
    }

    template<int Order = 1> misa_ome_unit_length<Order> astronomicalunit() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::ASTRONOMICALUNIT);
    }

    template<int Order = 1> misa_ome_unit_length<Order> lightyear() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::LIGHTYEAR);
    }

    template<int Order = 1> misa_ome_unit_length<Order> parsec() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::PARSEC);
    }

    template<int Order = 1> misa_ome_unit_length<Order> point() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::POINT);
    }

    template<int Order = 1> misa_ome_unit_length<Order> pixel() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::PIXEL);
    }

    template<int Order = 1> misa_ome_unit_length<Order> referenceframe() {
        return misa_ome_unit_length<Order>(::ome::xml::model::enums::UnitsLength::REFERENCEFRAME);
    }

}
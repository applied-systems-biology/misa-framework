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

#include <misaxx/core/misa_serializable.h>

namespace misaxx {

    /**
     * Base type for units
     */
    struct misa_unit_base {

    };

    /**
     * Base class for a unit supported by the MISA++ math types.
     * A unit can be one specific unit (e.g. unit_millimeters) or a group of
     * related and convertible units (e.g. unit_length) where the mode is stored inside of
     * the object.
     *
     * A unit must have a typedef 'select_order_type<size_t Order>' that returns the unit for given order
     * order 0 or lower should be misa_unit_numeric
     *
     * Additionally, a unit must have a static function convert(T src, unit src_unit, unit dst_unit)
     * that converts between the internal unit modes
     *
     * @tparam Order Order of the unit. Must be at least 1
     */
    template<size_t Order> struct misa_unit : public misa_serializable, public misa_unit_base {
        static constexpr size_t order = Order;
        static_assert(Order >= 1, "The order must be at least 1");

        /**
         * Returns a literal that represents the unit (e.g. mm^2)
         * @return
         */
        virtual std::string get_literal() const = 0;

        std::string get_documentation_name() const override {
            return "Unit";
        }

        std::string get_documentation_description() const override {
            return "Unit of a quantity";
        }
    };

    /**
     * Selects the higher order of given unit
     */
    template<class Unit> using misa_unit_higher_order = typename Unit::template select_order_type<Unit::order + 1>;

    /**
     * Selects the higher order of given unit
     * If the order of Unit is 1, it will decay to misa_unit_numeric
     */
    template<class Unit> using misa_unit_lower_order = typename Unit::template select_order_type<Unit::order - 1>;
}





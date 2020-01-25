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

#include <misaxx/core/attachments/misa_locatable.h>
#include <misaxx/core/attachments/misa_unit_numeric.h>
#include <boost/operators.hpp>
#include <ostream>
#include <misaxx/core/misa_json_schema_property.h>

namespace misaxx {

    /**
     * Operators that apply to quantities
     */
    template<class Quantity, typename Value> using misa_quantity_operators_t =
    boost::partially_ordered<Quantity,
    boost::equality_comparable<Quantity,
    boost::addable<Quantity,
    boost::subtractable<Quantity,
    boost::dividable2<Quantity, Value,
    boost::multipliable2<Quantity, Value,
    boost::incrementable<Quantity,
    boost::decrementable<Quantity>>>>>>>>;

    struct misa_quantity_base {
    };

    /**
     * A quantity is a value with a unit
     * @tparam Value
     * @tparam Unit
     */
    template<typename Value, class Unit>
    struct misa_quantity :
            public misa_quantity_base,
            public misa_locatable,
            private misa_quantity_operators_t <misa_quantity<Value, Unit>, Value>{

        using value_type = Value;
        using unit_type = Unit;

        misa_quantity() = default;

        explicit misa_quantity(value_type t_value, unit_type t_unit = unit_type()) :
        m_value(std::move(t_value)), m_unit(std::move(t_unit)) {

        }

        explicit misa_quantity(unit_type t_unit) :
                m_value(Value()), m_unit(std::move(t_unit)) {

        }

        /**
         * Factory function that creates a new quantity with a different value, but the same unit
         * @param t_value
         * @return
         */
        misa_quantity<Value, Unit> make(value_type t_value) const {
            return misa_quantity<Value, Unit>(std::move(t_value), m_unit);
        }

        /**
         * Casts the quantity to the numeric value
         * @return
         */
        explicit operator value_type() const {
            return get_value();
        }

        /**
         * Returns a new quantity with the same unit, but casts the numeric value
         * @tparam T
         * @return
         */
        template<typename T> explicit operator misa_quantity<T, Unit>() {
            return cast_value<T>();
        }

        /**
         * Returns the raw value
         * @return
         */
        value_type get_value() const {
            return m_value;
        }

        /**
         * Returns the raw value
         * @return
         */
        value_type &get_value() {
            return m_value;
        }

        /**
         * Returns the unit
         * @return
         */
        unit_type get_unit() const {
            return m_unit;
        }

        /**
         * Returns a new quantity with the same unit, but casts the numeric value
         * @tparam T
         * @return
         */
        template<typename T> misa_quantity<T, Unit> cast_value() const {
            return misa_quantity<T, Unit>(static_cast<T>(m_value), m_unit);
        }

        /**
         * Casts this quantity to a new unit
         * @param unit
         * @return
         */
        misa_quantity<Value, Unit> cast_unit(Unit unit) const {
            Value v = Unit::convert(m_value, m_unit, unit);
            return misa_quantity<Value, Unit>(std::move(v), std::move(unit));
        }

        /**
         * Casts this quantity to a numeric quantity
         * @return
         */
        misa_quantity<Value, misa_unit_numeric> to_numeric() const {
            return misa_quantity<Value, misa_unit_numeric>(m_value);
        }

        void from_json(const nlohmann::json &t_json) override {
            m_value = t_json["value"].get<Value>();
            m_unit.from_json(t_json["unit"]);
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_locatable::to_json(t_json);
            t_json["value"] = m_value;
            m_unit.to_json(t_json["unit"]);
        }

        void to_json_schema(misa_json_schema_property &t_schema) const override {
            misa_locatable::to_json_schema(t_schema);
            t_schema.resolve("value")->declare_required<Value>().document_title("Value");
            t_schema.resolve("unit")->declare_required<Unit>().document_title("Unit").document_description("Unit of this quantity");
        }

        std::string get_documentation_name() const override {
            return "Quantity";
        }

        std::string get_documentation_description() const override {
            return "Serializable value with an unit";
        }

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override {
            misa_locatable::build_serialization_id_hierarchy(result);
            misa_serialization_id self = m_unit.get_serialization_id();
            self.set_path(self.get_path() / "quantity");
            result.emplace_back(std::move(self));
        }

    public:

        /**
         * Returns true if the value and the unit are the same
         * No conversion is applied
         * @param rhs
         * @return
         */
        bool operator==(const misa_quantity<Value, Unit> &rhs) const {
            return m_value == rhs.m_value && m_unit == rhs.m_unit;
        }

        bool operator<(const misa_quantity<Value, Unit> &rhs) const {
            if (rhs.m_unit == m_unit) {
                return m_value < rhs.m_value;
            } else {
                const auto conv = rhs.cast_unit(m_unit);
                return conv < rhs;
            }
        }

        misa_quantity<Value, Unit> &operator+=(const misa_quantity<Value, Unit> &rhs) {
            if (rhs.m_unit == m_unit) {
                m_value += rhs.m_value;
            } else {
                const auto conv = rhs.cast_unit(m_unit);
                *this += conv;
            }
            return *this;
        }

        misa_quantity<Value, Unit> &operator-=(const misa_quantity<Value, Unit> &rhs) {
            if (rhs.m_unit == m_unit) {
                m_value -= rhs.m_value;
            } else {
                const auto conv = rhs.cast_unit(m_unit);
                *this -= conv;
            }
            return *this;
        }

        misa_quantity<Value, Unit> &operator*=(const value_type &value) {
            this->m_value *= value;
            return *this;
        }

        misa_quantity<Value, Unit> &operator/=(const value_type &value) {
            this->m_value /= value;
            return *this;
        }

        misa_quantity<Value, Unit> &operator%=(const value_type &value) {
            this->m_value %= value;
            return *this;
        }

        misa_quantity<Value, Unit> &operator++() {
            ++this->m_value;
            return *this;
        }

        misa_quantity<Value, Unit> &operator--() {
            --this->m_value;
            return *this;
        }

        /**
         * Multiplication that will increase the order of the result
         * @param lhs
         * @param rhs
         * @return
         */
        misa_quantity<Value, misa_unit_higher_order<Unit> > operator *(const misa_quantity<Value, Unit> &rhs) {
            Value r = Unit::convert(rhs.m_value, rhs.m_unit, m_unit); // Convert rhs to the unit of lhs
            return misa_quantity<Value, misa_unit_higher_order<Unit>>(get_value() * r, misa_unit_higher_order<Unit>(get_unit()));
        }

        /**
         * Division that will decrease the order of the result
         * @param lhs
         * @param rhs
         * @return
         */
        misa_quantity<Value, misa_unit_lower_order<Unit>> operator /(const misa_quantity<Value, Unit> &rhs) {
            Value r = Unit::convert(rhs.m_value, rhs.m_unit, m_unit); // Convert rhs to the unit of lhs
            return misa_quantity<Value, misa_unit_higher_order<Unit>>(get_value() / r, misa_unit_higher_order<Unit>(get_unit()));
        }

        friend std::ostream &operator<<(std::ostream &os, const misa_quantity &quantity) {
            os << misaxx::utils::to_string(quantity.m_value) << quantity.m_unit.get_literal();
            return os;
        }

    private:
        Value m_value{};
        Unit m_unit{};
    };

    template<typename Value, class Unit>
    inline std::vector<misa_quantity<Value, Unit>> misa_quantities_to_same_unit(std::vector<misa_quantity<Value, Unit>> quantities) {
        if(quantities.size() > 1) {
            for(size_t i = 1; i < quantities.size(); ++i) {
                quantities[i] = quantities[i].cast_unit(quantities[0].get_unit());
            }
        }
        return quantities;
    }
}

namespace nlohmann {
    template<typename Value, class Unit>
    struct adl_serializer<misaxx::misa_quantity<Value, Unit>> {
        static void to_json(json &j, const misaxx::misa_quantity<Value, Unit> &opt) {
            opt.to_json(j);
        }

        static void from_json(const json &j, misaxx::misa_quantity<Value, Unit> &opt) {
            opt.from_json(j);
        }
    };
}



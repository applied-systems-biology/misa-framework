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

namespace misaxx {
    template<typename Value, class Unit>
    struct misa_quantity_range : public misaxx::misa_serializable {

        using value_type = Value;
        using unit_type = Unit;
        using quantity_type = misa_quantity<Value, Unit>;

        misa_quantity_range() = default;

        explicit misa_quantity_range(value_type t_from, value_type t_to, unit_type t_unit = unit_type()) : m_from(t_from), m_to(t_to), m_unit(t_unit) {

        }

        quantity_type get_from() const {
            return misa_quantity<Value, Unit> { m_from, m_unit };
        }

        quantity_type get_to() const {
            return misa_quantity<Value, Unit> { m_to, m_unit };
        }

        quantity_type get_length() const {
            return quantity_type { m_to - m_from, m_unit };
        }

        void set_from(value_type value) {
            m_from = value;
        }

        void set_to(value_type value) {
            m_to = value;
        }

        void set_from(const quantity_type &t_value) {
            if(t_value.get_unit() != m_unit) {
                quantity_type converted = t_value.cast_unit(m_unit);
                m_from = converted.get_value();
            }
            else {
                m_from = t_value.get_value();
            }
        }

        void set_to(const quantity_type &t_value) {
            if(t_value.get_unit() != m_unit) {
                quantity_type converted = t_value.cast_unit(m_unit);
                m_to = converted.get_value();
            }
            else {
                m_to = t_value.get_value();
            }
        }

        misa_quantity_range<Value, Unit> cast_unit(const Unit &t_unit) const {
            if(t_unit == m_unit) {
                return *this;
            }
            else {
                auto from = get_from().cast_unit(t_unit);
                auto to = get_to().cast_unit(t_unit);
                return misa_quantity_range<Value, Unit> { from.get_value(), to.get_value(), t_unit };
            }
        }

        void from_json(const nlohmann::json &t_json) override {
            misa_serializable::from_json(t_json);
            m_from = t_json["from"].get<Value>();
            m_to = t_json["to"].get<Value>();
            m_unit = t_json["unit"].get<Unit>();
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializable::to_json(t_json);
            t_json["from"] = m_from;
            t_json["to"] = m_to;
            t_json["unit"] = m_unit;
        }

        void to_json_schema(misa_json_schema_property &schema) const override {
            misa_serializable::to_json_schema(schema);
            schema.resolve("from")->declare_required<Value>().document_title("From").document_description("Inclusive start value of this range");
            schema.resolve("to")->declare_required<Value>().document_title("To").document_description("Exclusive end value of this range");
            m_unit.to_json_schema(*schema.resolve("unit"));
        }

        std::string get_documentation_name() const override {
            return "Range";
        }

        std::string get_documentation_description() const override {
            return "Range of values";
        }

        /**
         * Includes the quanity into this range
         * @param t_value
         */
        void include(const quantity_type &t_value) {
            Value conv_value = t_value.get_value();
            if(t_value.get_unit() != m_unit) {
                quantity_type converted = t_value.cast_unit(m_unit);
                conv_value = converted.get_value();
            }

            if(conv_value < m_from) {
               m_from = conv_value;
            }
            if(conv_value >= m_to) {
                m_to = conv_value;
            }
        }

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override {
            misa_serializable::build_serialization_id_hierarchy(result);
        }

    private:

        value_type m_from {};
        value_type m_to {};
        unit_type  m_unit {};
    };
}

namespace nlohmann {
    template<typename Value, class Unit>
    struct adl_serializer<misaxx::misa_quantity_range<Value, Unit>> {
        static void to_json(json &j, const misaxx::misa_quantity_range<Value, Unit> &opt) {
            opt.to_json(j);
        }

        static void from_json(const json &j, misaxx::misa_quantity_range<Value, Unit> &opt) {
            opt.from_json(j);
        }
    };
}

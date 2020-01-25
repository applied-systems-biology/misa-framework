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

#include <misaxx/core/utils/string.h>
#include <misaxx/core/attachments/misa_unit.h>

namespace misaxx {

    /**
     * Special unit that multiplies and divides into itself
     */
    struct misa_unit_numeric : public misa_unit<1> {

        /**
         * This will always return misa_unit_numeric
         */
        template<size_t O> using select_order_type = misa_unit_numeric;

        misa_unit_numeric() = default;

        /**
         * Any unit can be converted into misa_unit_numeric
         * @tparam O
         * @param src
         */
        template<class Unit, typename = typename std::enable_if<std::is_base_of<misa_unit_base, Unit>::value>::type>
        explicit misa_unit_numeric(const Unit &src) {

        }

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        std::string get_literal() const override;

        bool operator==(const misa_unit_numeric &rhs) const;

        bool operator!=(const misa_unit_numeric &rhs) const;

        /**
         * Allows intra-unit conversion
         * Automatically called by misa_quantity
         * @tparam T
         * @param t_value
         * @param t_src
         * @param t_dst
         * @return
         */
        template<typename T> static T convert(T t_value, const misa_unit_numeric &t_src, const misa_unit_numeric &t_dst) {
            return t_value;
        }

        std::string get_documentation_name() const override;

        std::string get_documentation_description() const override;

    protected:
        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;
    };

    inline void to_json(nlohmann::json& j, const misa_unit_numeric& p) {
        p.to_json(j);
    }

    inline void from_json(const nlohmann::json& j, misa_unit_numeric& p) {
        p.from_json(j);
    }
}




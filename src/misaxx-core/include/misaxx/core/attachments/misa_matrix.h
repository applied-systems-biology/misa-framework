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
#include <array>
#include <boost/operators.hpp>
#include <misaxx/core/attachments/misa_quantity.h>

namespace misaxx {

    /**
    * Operators that apply to quantities
    */
    template<class Quantity, typename Value> using misa_matrix_operators_t =
    boost::equality_comparable<Quantity,
    boost::addable<Quantity,
    boost::addable2<Quantity, Value,
    boost::subtractable<Quantity,
    boost::subtractable2<Quantity, Value,
    boost::dividable2<Quantity, Value,
    boost::multipliable2<Quantity, Value,
    boost::incrementable<Quantity,
    boost::decrementable<Quantity>>>>>>>>>;

    struct misa_matrix_base {

    };

    /**
     * A matrix of misa_quantity
     * @tparam Value
     * @tparam Unit
     * @tparam Rows
     * @tparam Cols
     */
    template<typename Value, class Unit, size_t Rows, size_t Cols> struct misa_matrix :
            public misa_matrix_base,
            public misa_serializable,
            private misa_matrix_operators_t <misa_matrix<Value, misa_unit_numeric, Rows, Cols>, Value> {

        static_assert(Rows > 0 && Cols > 0, "The matrix must have a positive number of rows and columns!");

        using value_type = Value;
        using unit_type = Unit;
        static constexpr size_t rows = Rows;
        static constexpr size_t cols = Cols;

        /**
         * Initializes a default matrix (filled with 0)
         */
        misa_matrix() {
            std::fill(m_values.begin(), m_values.end(), Value());
        }

        /**
         * Initializes the matrix from an array
         * @param t_values
         * @param t_unit
         */
        explicit misa_matrix(std::array<value_type, Rows * Cols> t_values, unit_type t_unit = unit_type()) : m_values(std::move(t_values)),
        m_unit(std::move(t_unit)) {

        }

        /**
         * Initializes the matrix with an unit
         * @param t_unit
         */
        explicit misa_matrix(unit_type t_unit) : m_unit(std::move(t_unit)) {
            std::fill(m_values.begin(), m_values.end(), Value());
        }

        /**
       * Returns the number of elements within this matrix
       * @return
       */
        constexpr size_t size() const {
            return Rows * Cols;
        }


        void from_json(const nlohmann::json &t_json) override {
            for(size_t i = 0; i < size(); ++i) {
                m_values[i] = t_json["values"][i].get<Value>();
            }
            m_unit.from_json(t_json["unit"]);
        }

        /**
         * Reads matrix values from value names instead from the value array and from a "unit" property
         * @param t_json
         * @param t_value_names
         */
        void values_from_json(const nlohmann::json &t_json, const std::array<std::string, Rows * Cols> &t_value_names) {
            for(size_t i = 0; i < size(); ++i) {
                m_values[i] = t_json[t_value_names[i]].template get<Value>();
            }
            m_unit.from_json(t_json["unit"]);
        }

        void to_json(nlohmann::json &t_json) const override {
            misa_serializable::to_json(t_json);
            t_json["values"] = m_values;
            m_unit.to_json(t_json["unit"]);
        }

        /**
         * Writes matrix values to variable names instead of the value array and ther unit to a "unit" property
         * @param t_json
         */
        void values_to_json(nlohmann::json &t_json, const std::array<std::string, Rows * Cols> &t_value_names) const {
            for(size_t i = 0; i < size(); ++i) {
                t_json[t_value_names[i]] = m_values[i];
            }
            m_unit.to_json(t_json["unit"]);
        }

        void to_json_schema(misa_json_schema_property &t_schema) const override {
            misa_serializable::to_json_schema(t_schema);
            t_schema.resolve("values")->declare_required<std::vector<Value>>().document_title("Values").document_description("List of matrix values row-major indexed");
            t_schema.resolve("unit")->declare_required<Unit>().document_title("Unit");
        }

        void values_to_json_schema(misa_json_schema_property &t_schema,
                const std::array<std::string, Rows * Cols> &t_value_names) const {
            for(size_t i = 0; i < size(); ++i) {
                t_schema.resolve(t_value_names[i])->template declare_required<Value>();
            }
            t_schema.resolve("unit")->declare_required<Unit>().document_title("Unit");
        }

        std::string get_documentation_name() const override {
            return "Matrix";
        }

        std::string get_documentation_description() const override {
            return "Serializable matrix of quantities";
        }

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override {
            misa_serializable::build_serialization_id_hierarchy(result);
            misa_serialization_id self = m_unit.get_serialization_id();
            self.set_path(self.get_path() / (std::string("matrix") + misaxx::utils::to_string(Rows) + "x" + misaxx::utils::to_string(Cols)));
            result.emplace_back(std::move(self));
        }

    public:

        /**
         * Returns one element
         * @param row
         * @param col
         * @return
         */
        misa_quantity<value_type, unit_type> get(size_t row, size_t col) const {
            return misa_quantity<value_type, unit_type>(get_value(row, col), m_unit);
        }

        /**
         * Sets the value of one element.
         * Automatically converts the value into the unit of this matrix
         * @param row
         * @param col
         * @param v
         */
        void set(size_t row, size_t col, const misa_quantity<value_type, unit_type> &v) {
            if(v.get_unit() == get_unit()) {
                get_value(row, col) = v.get_value();
            }
            else {
                auto c = v.cast_unit(get_unit());
                set(row, col, c);
            }
        }

        /**
         * Returns the raw value of the element of index i
         * @param i
         * @return
         */
        value_type value_at(size_t i) const {
            return m_values.at(i);
        }

        /**
         * Returns the raw value of the element of index i
         * @param i
         * @return
         */
        value_type &value_at(size_t i) {
            return m_values.at(i);
        }

        /**
         * Returns the raw value of an element
         * @param row
         * @param col
         * @return
         */
        value_type get_value(size_t row, size_t col) const {
            if(row >= Rows || col >= Cols)
                throw std::out_of_range("Matrix position out of range");
            return m_values.at(col + row * cols);
        }

        /**
         * Returns the raw value of an element
         * @param row
         * @param col
         * @return
         */
        value_type &get_value(size_t row, size_t col) {
            if(row >= Rows || col >= Cols)
                throw std::out_of_range("Matrix position out of range");
            return m_values.at(col + row * cols);
        }

        /**
         * Returns the unit of this matrix
         * @return
         */
        unit_type get_unit() const {
            return m_unit;
        }

        /**
         * Returns if the matrix is a square matrix
         * @return
         */
        constexpr bool is_square() const {
            return Rows == Cols;
        }

        /**
         * Returns a new matrix with the same unit, but casts the numeric values
         * @tparam T
         * @return
         */
        template<typename T> misa_matrix<T, Unit, Rows, Cols> cast_values() const {
            std::array<T, Rows * Cols> arr;
            for(size_t i = 0; i < arr.size(); ++i) {
                arr[i] = static_cast<T>(m_values[i]);
            }
            return misa_matrix<T, Unit, Rows, Cols>(std::move(arr), m_unit);
        }

        /**
        * Casts this matrix to a new unit
        * @param unit
        * @return
        */
        misa_matrix<Value, Unit, Rows, Cols> cast_unit(Unit unit) const {
            std::array<Value, Rows * Cols> arr;
            for(size_t i = 0; i < arr.size(); ++i) {
                arr[i] = Unit::convert(m_values[i], m_unit, unit);
            }
            return misa_matrix<Value, Unit, Rows, Cols>(std::move(arr), std::move(unit));
        }

        /**
       * Casts this matrix to a matrix without unit (numeric)
       * @param unit
       * @return
       */
        misa_matrix<Value, misa_unit_numeric, Rows, Cols> to_numeric() const {
            return misa_matrix<Value, misa_unit_numeric, Rows, Cols>(m_values);
        }

        /**
         * Returns the transposed matrix of this matrix
         * @return
         */
        misa_matrix<Value, Unit, Cols, Rows> transpose() const {
            std::array<Value, Rows * Cols> arr;
            for(size_t i = 0; i < arr.size(); ++i) {
                size_t y = i / Cols;
                size_t x = i - Cols * y;
                size_t j = y + x * Rows;
                arr[j] = m_values[i];
            }
            return misa_matrix<Value, Unit, Rows, Cols>(std::move(arr), m_unit);
        }

        /**
         * Returns the product of all values in this matrix as quantity
         * The order of the output quantity is order * rows * columns
         * @return
         */
        auto get_element_product() const {
            using output_unit = typename Unit::template select_order_type<Unit::order * Rows * Cols>;
            return misa_quantity<Value, output_unit>(std::accumulate(m_values.begin(), m_values.end(), 1, std::multiplies<Value>()), output_unit(m_unit));
        }

        /**
         * Returns the sum of all values in this matrix as quantity
         * @return
         */
        auto get_element_sum() const {
            return misa_quantity<Value, Unit>(std::accumulate(m_values.begin(), m_values.end(), 0), m_unit);
        }

        /**
         * Returns true if oth matrices are the same
         * No conversion of units is done
         * @param rhs
         * @return
         */
        bool operator==(const misa_matrix<Value, Unit, Rows, Cols> &rhs) const {
            return m_values == rhs.m_values && m_unit == rhs.m_unit;
        }

        /**
         * Element-wise addition operation
         * Automatically converts the unit
         * @param rhs
         * @return
         */
        misa_matrix<Value, Unit, Rows, Cols> &operator+=(const misa_matrix<Value, Unit, Rows, Cols> &rhs) {
            if (rhs.m_unit == m_unit) {
                for(size_t i = 0; i < size(); ++i) {
                    m_values[i] += rhs.m_values[i];
                }
            } else {
                const auto conv = rhs.cast_unit(m_unit);
                *this += conv;
            }
            return *this;
        }

        /**
         * Element-wise subtraction operation
         * Automatically converts the unit
         * @param rhs
         * @return
         */
        misa_matrix<Value, Unit, Rows, Cols> &operator-=(const misa_matrix<Value, Unit, Rows, Cols> &rhs) {
            if (rhs.m_unit == m_unit) {
                for(size_t i = 0; i < size(); ++i) {
                    m_values[i] -= rhs.m_values[i];
                }
            } else {
                const auto conv = rhs.cast_unit(m_unit);
                *this -= conv;
            }
            return *this;
        }

        /**
         * Element-wise addition with scalar
         * @param value
         * @return
         */
        misa_matrix<Value, Unit, Rows, Cols> &operator+=(const value_type &value) {
            for(auto& v : m_values) {
                v += value;
            }
            return *this;
        }

        /**
         * Element-wise subtraction with scalar
         * @param value
         * @return
         */
        misa_matrix<Value, Unit, Rows, Cols> &operator-=(const value_type &value) {
            for(auto& v : m_values) {
                v -= value;
            }
            return *this;
        }

        /**
         * Element-wise multiplication with scalar
         * @param value
         * @return
         */
        misa_matrix<Value, Unit, Rows, Cols> &operator*=(const value_type &value) {
            for(auto& v : m_values) {
                v *= value;
            }
            return *this;
        }

        /**
         * Element-wise division with scalar
         * @param value
         * @return
         */
        misa_matrix<Value, Unit, Rows, Cols> &operator/=(const value_type &value) {
            for(auto& v : m_values) {
                v /= value;
            }
            return *this;
        }

        /**
         * Element-wise modulo with scalar
         * @param value
         * @return
         */
        misa_matrix<Value, Unit, Rows, Cols> &operator%=(const value_type &value) {
            for(auto& v : m_values) {
                v %= value;
            }
            return *this;
        }

        /**
         * Element-wise addition by 1
         * @return
         */
        misa_matrix<Value, Unit, Rows, Cols> &operator++() {
            for(auto& v : m_values) {
                ++v;
            }
            return *this;
        }

        /**
         * Element-wise subtraction by 1
         * @return
         */
        misa_matrix<Value, Unit, Rows, Cols> &operator--() {
            for(auto& v : m_values) {
                --v;
            }
            return *this;
        }

        /**
        * Element-wise multiplication. Use the % operator for a matrix multiplication
        * Increases the order of the unit
        * @param lhs
        * @param rhs
        * @return
        */
        misa_matrix<Value, misa_unit_higher_order<Unit>, Rows, Cols > operator *(const misa_matrix<Value, Unit, Rows, Cols> &rhs) {
            if(rhs.get_unit() == m_unit) {
                misa_matrix<Value, misa_unit_higher_order<Unit>, Rows, Cols > result { misa_unit_higher_order<Unit>(m_unit) };
                for(size_t i = 0; i < size(); ++i) {
                    result.value_at(i) = value_at(i) * rhs.value_at(i);
                }
                return result;
            }
            else {
                return *this * rhs.cast_unit(m_unit);
            }
        }

        /**
       * Element-wise division
       * Increases the order of the unit
       * @param lhs
       * @param rhs
       * @return
       */
        misa_matrix<Value, misa_unit_higher_order<Unit>, Rows, Cols > operator /(const misa_matrix<Value, Unit, Rows, Cols> &rhs) {
            if(rhs.get_unit() == m_unit) {
                misa_matrix<Value, misa_unit_higher_order<Unit>, Rows, Cols > result { misa_unit_higher_order<Unit>(m_unit) };
                for(size_t i = 0; i < size(); ++i) {
                    result.value_at(i) = value_at(i) / rhs.value_at(i);
                }
                return result;
            }
            else {
                return *this * rhs.cast_unit(m_unit);
            }
        }

        /**
         * Matrix multiplication
         * @tparam Matrix
         * @param rhs
         * @return
         */
        template<class Matrix> auto operator %(const Matrix &rhs) {
            static_assert(Rows == Matrix::cols, "Incompatible matrices!");
            if(rhs.get_unit() == m_unit) {
                misa_matrix<Value, misa_unit_higher_order<Unit>, Matrix::cols, Rows > result { misa_unit_higher_order<Unit>(m_unit) };
                for(size_t i = 0; i < result.rows; ++i) {
                    for(size_t j = 0; j < result.cols; ++j) {
                        for(size_t k = 0; k < Cols; ++k) {
                            result.get_value(i, j) += get_value(i, k) * rhs.get_value(k, j);
                        }
                    }
                }
                return result;
            }
            else {
                return *this % rhs.cast_unit(m_unit);
            }
        }

    private:
        std::array<Value, Rows * Cols> m_values;
        Unit m_unit;
    };

    /**
     * Convenience type for a matrix with 1 column
     */
    template<typename Value, class Unit, size_t Rows> using misa_vector = misa_matrix<Value, Unit, Rows, 1>;
}

namespace nlohmann {
    template<typename Value, class Unit, size_t Rows, size_t Cols>
    struct adl_serializer<misaxx::misa_matrix<Value, Unit, Rows, Cols>> {
        static void to_json(json &j, const misaxx::misa_matrix<Value, Unit, Rows, Cols> &opt) {
            opt.to_json(j);
        }

        static void from_json(const json &j, misaxx::misa_matrix<Value, Unit, Rows, Cols> &opt) {
            opt.from_json(j);
        }
    };
}



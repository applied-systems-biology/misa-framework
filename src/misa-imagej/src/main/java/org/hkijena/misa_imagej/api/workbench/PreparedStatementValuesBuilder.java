/*
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

package org.hkijena.misa_imagej.api.workbench;

import java.math.BigDecimal;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class PreparedStatementValuesBuilder {
    private PreparedStatement preparedStatement;
    private int index = 1;

    public PreparedStatementValuesBuilder(PreparedStatement preparedStatement) {
        this.preparedStatement = preparedStatement;
    }

    public PreparedStatement getPreparedStatement() {
        return preparedStatement;
    }

    public void addNull(int sqlType) throws SQLException {
        preparedStatement.setNull(index++, sqlType);
    }


    public void addBoolean(boolean x) throws SQLException {
        preparedStatement.setBoolean(index++, x);
    }


    public void addByte(byte x) throws SQLException {
        preparedStatement.setByte(index++, x);
    }


    public void addShort(short x) throws SQLException {
        preparedStatement.setShort(index++, x);
    }


    public void addInt(int x) throws SQLException {
        preparedStatement.setInt(index++, x);
    }


    public void addLong(long x) throws SQLException {
        preparedStatement.setLong(index++, x);
    }


    public void addFloat(float x) throws SQLException {
        preparedStatement.setFloat(index++, x);
    }


    public void addDouble(double x) throws SQLException {
        preparedStatement.setDouble(index++, x);
    }


    public void addBigDecimal(BigDecimal x) throws SQLException {
        preparedStatement.setBigDecimal(index++, x);
    }


    public void addString(String x) throws SQLException {
        preparedStatement.setString(index++, x);
    }


    public void addBytes(byte x[]) throws SQLException {
        preparedStatement.setBytes(index++, x);
    }


    public void addDate(java.sql.Date x)
            throws SQLException {
        preparedStatement.setDate(index++, x);
    }


    public void addTime(java.sql.Time x)
            throws SQLException {
        preparedStatement.setTime(index++, x);
    }


    public void addTimestamp(java.sql.Timestamp x)
            throws SQLException {
        preparedStatement.setTimestamp(index++, x);
    }


    public void addAsciiStream(java.io.InputStream x, int length)
            throws SQLException {
        preparedStatement.setAsciiStream(index++, x, length);
    }


    public void addBinaryStream(java.io.InputStream x,
                         int length) throws SQLException {
        preparedStatement.setBinaryStream(index++, x, length);
    }


    public void addObject(Object x, int targetSqlType)
            throws SQLException {
        preparedStatement.setObject(index++, x, targetSqlType);
    }


    public void addObject(Object x) throws SQLException {
        preparedStatement.setObject(index++, x);
    }


}

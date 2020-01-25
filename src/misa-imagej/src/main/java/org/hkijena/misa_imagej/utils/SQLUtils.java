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

package org.hkijena.misa_imagej.utils;

import java.util.List;

public class SQLUtils {

    private SQLUtils() {

    }

    public static String concatFilters(List<String> filters, String operator) {
        StringBuilder result = new StringBuilder();
        for(String filter : filters) {
            if(result.length() > 0) {
                result.append(" ").append(operator).append(" ");
            }
            result.append("(").append(filter).append(")");
        }
        return result.toString();
    }

    public static String column(String s) {
        if(s.contains(" ") || s.contains("-")) {
            return "\"" + s.replace("\\", "\\\\")
                    .replace("\b","\\b")
                    .replace("\n","\\n")
                    .replace("\r", "\\r")
                    .replace("\t", "\\t")
                    .replace("\\x1A", "\\Z")
                    .replace("\\x00", "\\0")
                    .replace("'", "\\'")
                    .replace("\"", "\\\"") + "\"";
        }
        else {
            return s;
        }
    }

    public static String value(String s) {
        return "'" + s.replace("\\", "\\\\")
                .replace("\b","\\b")
                .replace("\n","\\n")
                .replace("\r", "\\r")
                .replace("\t", "\\t")
                .replace("\\x1A", "\\Z")
                .replace("\\x00", "\\0")
                .replace("'", "\\'") + "'";
    }

    public static String escapeWildcardsForSQLite(String s) {
        return s.replace("\\", "\\\\")
                .replace("\b","\\b")
                .replace("\n","\\n")
                .replace("\r", "\\r")
                .replace("\t", "\\t")
                .replace("\\x1A", "\\Z")
                .replace("\\x00", "\\0")
                .replace("'", "\\'")
                .replace("\"", "\\\"")
                .replace("%", "\\%")
                .replace("_","\\_");
    }


}

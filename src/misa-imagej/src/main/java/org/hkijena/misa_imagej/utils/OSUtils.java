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

import java.util.Locale;

public class OSUtils {

    public static OperatingSystem detectOperatingSystem() {
        boolean x64 = System.getProperty("os.arch").endsWith("64");
        String os = System.getProperty("os.name", "generic").toLowerCase(Locale.ENGLISH);
        if(os.contains("windows")) {
            return OperatingSystem.Windows;
        }
        else if(os.contains("linux")) {
            return OperatingSystem.Linux;
        }
        else {
            return OperatingSystem.Unknown;
        }
    }

    public static OperatingSystemArchitecture detectArchitecture() {
        if(System.getProperty("os.arch").endsWith("64"))
            return OperatingSystemArchitecture.x64;
        else
            return OperatingSystemArchitecture.x32;
    }

    public static boolean isCompatible(OperatingSystem system, OperatingSystemArchitecture architecture, OperatingSystem targetSystem, OperatingSystemArchitecture targetArchitecture) {
        if(system != targetSystem)
            return false;
        return architecture == targetArchitecture || (architecture == OperatingSystemArchitecture.x64 && targetArchitecture == OperatingSystemArchitecture.x32);
    }
}

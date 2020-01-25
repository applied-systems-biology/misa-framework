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

package org.hkijena.misa_imagej.ui.components;

import javax.swing.*;

public class MemoryStatusUI extends JProgressBar {

    private Timer timer;
    private static final long MEGABYTES = 1024 * 1024;

    public MemoryStatusUI() {
        initialize();
    }

    private void initialize() {
        setStringPainted(true);
        setString("- / -");
        timer = new Timer(1000, e -> {
            setMaximum((int)(Runtime.getRuntime().maxMemory() / MEGABYTES));
            setValue((int)(Runtime.getRuntime().totalMemory() / MEGABYTES));
            setString((Runtime.getRuntime().totalMemory() / MEGABYTES) + "MB / " + (Runtime.getRuntime().maxMemory() / MEGABYTES) + "MB");
        });
        timer.setRepeats(true);
        timer.start();
    }
}

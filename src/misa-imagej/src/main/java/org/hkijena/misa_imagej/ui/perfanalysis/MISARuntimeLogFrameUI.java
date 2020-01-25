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

package org.hkijena.misa_imagej.ui.perfanalysis;

import com.google.common.eventbus.Subscribe;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.*;

public class MISARuntimeLogFrameUI extends JFrame {

    public MISARuntimeLogFrameUI() {
        initialize();
    }

    private void initialize() {
        setSize(800, 600);
        setIconImage(UIUtils.getIconFromResources("misaxx.png").getImage());
        getContentPane().setLayout(new BorderLayout(8, 8));
        setTitle("MISA++ runtime analysis");
        setLayout(new BorderLayout());

        MISARuntimeLogUI ui = new MISARuntimeLogUI();
        ui.setHideOpenButton(false);
        add(ui, BorderLayout.CENTER);
        ui.getEventBus().register(this);
    }

    @Subscribe
    public void handleRuntimeLogChangedEvent(MISARuntimeLogUI.RuntimeLogChangedEvent event) {
        if(event.getRuntimeLog() == null || event.getPath() == null) {
            setTitle("MISA++ runtime analysis");
        }
        else {
            setTitle(event.getPath().toString() + " - MISA++ runtime analysis");
        }
    }

}

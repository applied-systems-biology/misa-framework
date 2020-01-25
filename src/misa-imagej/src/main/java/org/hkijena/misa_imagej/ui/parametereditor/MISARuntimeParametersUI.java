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

package org.hkijena.misa_imagej.ui.parametereditor;

import org.hkijena.misa_imagej.api.MISAModuleInstance;
import org.hkijena.misa_imagej.ui.json.JSONSchemaEditorUI;

import javax.swing.*;
import java.awt.*;

public class MISARuntimeParametersUI extends JPanel {

    private MISAModuleInstance parameterSchema;

    public MISARuntimeParametersUI(MISAModuleInstance app) {
        this.parameterSchema = app;
        initialize();
    }

    private void initialize() {
        JSONSchemaEditorUI jsonSchemaEditorUI = new JSONSchemaEditorUI();

        setLayout(new BorderLayout());
        add(jsonSchemaEditorUI, BorderLayout.CENTER);

        jsonSchemaEditorUI.setSchema(parameterSchema.getRuntimeParameters());
    }

}

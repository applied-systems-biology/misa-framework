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

import org.hkijena.misa_imagej.utils.ResourceUtils;
import org.icepdf.ri.common.ComponentKeyBinding;
import org.icepdf.ri.common.SwingController;
import org.icepdf.ri.common.SwingViewBuilder;
import org.icepdf.ri.common.views.DocumentViewControllerImpl;

import javax.swing.*;
import java.awt.*;
import java.io.InputStream;

public class PDFReader extends JPanel {

    private SwingController controller;

    public PDFReader() {
        initialize();
    }

    private void initialize() {
        setLayout(new BorderLayout());

        controller = new SwingController();
        SwingViewBuilder factory = new SwingViewBuilder(controller);

        // Use the factory to build a JPanel that is pre-configured
        //with a complete, active Viewer UI.
        JPanel viewerComponentPanel = factory.buildViewerPanel();

        // add copy keyboard command
        ComponentKeyBinding.install(controller, viewerComponentPanel);
        add(viewerComponentPanel, BorderLayout.CENTER);

        controller.getDocumentViewController().setViewType(DocumentViewControllerImpl.ONE_COLUMN_VIEW);
    }

    public void openDocument(String path) {
        controller.openDocument(path);
    }

    public void openDocument(InputStream stream, String name) {
        controller.openDocument(stream, name, null);
    }

    public static PDFReader fromResource(String resourcePath) {
        PDFReader reader = new PDFReader();
        reader.openDocument(ResourceUtils.getPluginResourceAsStream(resourcePath), resourcePath);
        return reader;
    }
}

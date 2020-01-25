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
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.*;

public class AddSamplesDialog extends JDialog {

    private MISAModuleInstance moduleInstance;
    private JTextArea samplesInput;

    public AddSamplesDialog(Window parent, MISAModuleInstance moduleInstance) {
        super(parent);
        this.moduleInstance = moduleInstance;
        initialize();
    }

    private void initialize() {
        setSize(400, 300);
        getContentPane().setLayout(new BorderLayout(8, 8));
        setTitle("Add samples");
        setIconImage(UIUtils.getIconFromResources("misaxx.png").getImage());

        JTextArea infoArea = new JTextArea("Please insert the name of the sample. You can also add multiple samples at once by writing multiple lines. Each line represents one sample.");
        infoArea.setEditable(false);
        infoArea.setOpaque(false);
        infoArea.setBorder(null);
        infoArea.setWrapStyleWord(true);
        infoArea.setLineWrap(true);
        add(infoArea, BorderLayout.NORTH);

        samplesInput = new JTextArea();
        add(new JScrollPane(samplesInput), BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.LINE_AXIS));

        buttonPanel.add(Box.createHorizontalGlue());

        JButton cancelButton = new JButton("Cancel");
        cancelButton.addActionListener(e -> setVisible(false));
        buttonPanel.add(cancelButton);

        JButton addButton = new JButton("Add");
        addButton.addActionListener(e -> addFromInput());
        buttonPanel.add(addButton);

        add(buttonPanel, BorderLayout.SOUTH);
    }

    private void addFromInput() {
        if(samplesInput.getText() != null && !samplesInput.getText().isEmpty()) {
            for(String line : samplesInput.getText().split("\n")) {
                String modified = line.trim();
                if(!modified.isEmpty()) {
                    if(!moduleInstance.getSamples().containsKey(modified)) {
                        moduleInstance.addSample(modified);
                    }
                }
            }
        }
        setVisible(false);
    }
}

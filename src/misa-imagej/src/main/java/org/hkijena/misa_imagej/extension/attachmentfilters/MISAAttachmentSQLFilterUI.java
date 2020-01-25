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

package org.hkijena.misa_imagej.extension.attachmentfilters;

import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilter;
import org.hkijena.misa_imagej.ui.workbench.objectbrowser.MISAAttachmentFilterUI;
import org.hkijena.misa_imagej.utils.ui.DocumentChangeListener;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import java.awt.*;

public class MISAAttachmentSQLFilterUI extends MISAAttachmentFilterUI {
    public MISAAttachmentSQLFilterUI(MISAAttachmentFilter filter) {
        super(filter);
        initialize();
    }

    private void initialize() {
        JTextArea sqlArea = new JTextArea();
        sqlArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 12));
        sqlArea.setMinimumSize(new Dimension(100, 100));
        sqlArea.setWrapStyleWord(true);
        sqlArea.setLineWrap(true);
        sqlArea.setText(getFilter().toSQLStatement());
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(sqlArea, BorderLayout.CENTER);
        sqlArea.getDocument().addDocumentListener(new DocumentChangeListener() {
            @Override
            public void changed(DocumentEvent documentEvent) {
                ((MISAAttachmentSQLFilter)getFilter()).setSql(sqlArea.getText().replace('\n', ' '));
            }
        });
    }
}

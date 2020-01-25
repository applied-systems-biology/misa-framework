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

package org.hkijena.misa_imagej.ui.workbench.tableanalyzer;

import org.hkijena.misa_imagej.MISAImageJRegistryService;

import javax.swing.*;
import java.awt.*;

public class MISATableVectorOperationDialogUI extends JDialog {

    private MISATableVectorOperation operation;
    private boolean needsOpenDialog;
    private boolean userAccepts;

    private MISATableVectorOperationDialogUI(MISATableVectorOperation operation) {
        this.operation = operation;

        MISATableVectorOperationUI ui = MISAImageJRegistryService.getInstance().getTableAnalyzerUIOperationRegistry().createUIForVectorOperation(operation);
        if(ui == null) {
            needsOpenDialog = false;
        }
        else {
            setLayout(new BorderLayout());
            add(ui, BorderLayout.CENTER);
        }
    }

    public MISATableVectorOperation getOperation() {
        return operation;
    }

    public boolean isNeedsOpenDialog() {
        return needsOpenDialog;
    }

    public boolean isUserAccepts() {
        return userAccepts;
    }
}

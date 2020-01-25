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

package org.hkijena.misa_imagej.extension.outputcaches;

import ij.WindowManager;
import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.workbench.MISAOutput;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;

public class GenericImageOutputCacheUI extends DefaultMISAOutputCacheUI {
    public GenericImageOutputCacheUI(MISAOutput misaOutput, MISACache cache) {
        super(misaOutput, cache);
    }

    @Override
    protected void initialize() {

        if(getFilesystemPath() == null)
            return;

        AbstractButton renameCurrentImageButton = createButton("Set current image name", UIUtils.getIconFromResources("imagej.png"));
        renameCurrentImageButton.addActionListener(e -> renameCurrentImage());

        super.initialize();
    }

    private void renameCurrentImage() {
        if(WindowManager.getCurrentImage() != null) {
            WindowManager.getCurrentImage().setTitle(getCache().getSample().getName() + "/" + getCache().getRelativePathName());
        }
    }
}

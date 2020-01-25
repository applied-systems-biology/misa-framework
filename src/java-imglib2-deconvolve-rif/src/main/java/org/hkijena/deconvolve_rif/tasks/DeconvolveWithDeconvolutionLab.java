/**
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

package org.hkijena.deconvolve_rif.tasks;

import deconvolution.algorithm.RegularizedInverseFilter;
import deconvolutionlab.Lab;
import org.hkijena.deconvolve_rif.DataInterface;
import signal.RealSignal;

public class DeconvolveWithDeconvolutionLab extends DAGTask {

    public DeconvolveWithDeconvolutionLab(Integer tid, DataInterface dataInterface) {
        super(tid, dataInterface);
    }

    @Override
    public void work() {
        RealSignal img = Lab.openFile(getDataInterface().getConvolvedImage().getFile().toString());
        RealSignal psf = Lab.openFile(getDataInterface().getPsfImage().getFile().toString());
        RegularizedInverseFilter rif = new RegularizedInverseFilter(0.001);
        RealSignal x = rif.run(img, psf);
        Lab.save(x, getDataInterface().getDeconvolvedImage().getFile().toString());
    }
}

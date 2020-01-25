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

import com.github.dexecutor.core.task.Task;
import org.hkijena.deconvolve_rif.DataInterface;

public abstract class DAGTask extends Task<Integer, Integer> {

    private Integer tid;
    private DataInterface dataInterface;

    protected DAGTask(Integer tid, DataInterface dataInterface) {
        this.tid = tid;
        this.dataInterface = dataInterface;
    }

    public DataInterface getDataInterface() {
        return dataInterface;
    }

    public Integer getTid() {
        return tid;
    }

    public abstract void work();

    public Integer execute() {
        work();
        return getTid();
    }
}

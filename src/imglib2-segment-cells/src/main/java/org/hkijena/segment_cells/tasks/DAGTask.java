package org.hkijena.segment_cells.tasks;

import com.github.dexecutor.core.task.Task;
import org.hkijena.segment_cells.SampleDataInterface;

public abstract class DAGTask extends Task<Integer, Integer> {

    private Integer tid;
    private SampleDataInterface sampleDataInterface;

    protected DAGTask(Integer tid, SampleDataInterface sampleDataInterface) {
        this.tid = tid;
        this.sampleDataInterface = sampleDataInterface;
    }

    public SampleDataInterface getSampleDataInterface() {
        return sampleDataInterface;
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

package org.hkijena.segment_cells.tasks;

import net.imglib2.Cursor;
import net.imglib2.img.Img;
import net.imglib2.type.numeric.integer.IntType;
import org.hkijena.segment_cells.ExperimentDataInterface;
import org.hkijena.segment_cells.SampleDataInterface;

import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class QuantifyConidia extends DAGTask {

    public QuantifyConidia(Integer tid, SampleDataInterface sampleDataInterface) {
        super(tid, sampleDataInterface);
    }

    @Override
    public void work() {
        System.out.println("Running QuantifyConidia on " + getSampleDataInterface().toString());
        for(Map.Entry<String, ExperimentDataInterface> kv : getSampleDataInterface().getExperiments().entrySet()) {
            Img<IntType> label = kv.getValue().getOutputLabel().getOrCreate();
            Set<Integer> knownLabels = new HashSet<>();

            Cursor<IntType> cursor = label.cursor();
            while(cursor.hasNext()) {
                cursor.fwd();
                int l = cursor.get().getInteger();
                if(l > 0) {
                    knownLabels.add(l);
                }
            }

            getSampleDataInterface().getQuantificationResults().put(kv.getKey(), knownLabels.size());
        }
    }
}

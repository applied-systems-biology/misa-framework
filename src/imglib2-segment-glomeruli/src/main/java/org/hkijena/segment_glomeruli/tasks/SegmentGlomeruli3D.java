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

package org.hkijena.segment_glomeruli.tasks;

import net.imglib2.Cursor;
import net.imglib2.RandomAccess;
import net.imglib2.algorithm.labeling.ConnectedComponents;
import net.imglib2.img.Img;
import net.imglib2.img.array.ArrayImgFactory;
import net.imglib2.type.numeric.integer.UnsignedIntType;
import org.hkijena.segment_glomeruli.DataInterface;

import java.util.*;

public class SegmentGlomeruli3D extends DAGTask {

    private double maxGlomerulusRadius = 65;

    public SegmentGlomeruli3D(int tid, DataInterface dataInterface) {
        super(tid, dataInterface);
    }

    @Override
    public void work() {
        System.out.println("Running SegmentGlomeruli3D on " + getDataInterface().getInputData().toString());

        List<Img<UnsignedIntType>> labels = new ArrayList<>();
        int loaded_label_count = 0;
        int first_loaded_label_index = 0;

        final long limsize = (long)maxGlomerulusRadius;

        int global_max_label = 0;

        final long xsize = getDataInterface().getInputData().getXSize();
        final long ysize = getDataInterface().getInputData().getYSize();
        final long zsize = getDataInterface().getInputData().getZSize();

        for(int i = 0; i < zsize; ++i) {
            Img<UnsignedIntType> label = (new ArrayImgFactory<>(new UnsignedIntType())).create(xsize, ysize);
            int max_label = ConnectedComponents.labelAllConnectedComponents(getDataInterface().getGlomeruli2DOutputData().getOrCreatePlane(i),
                    label,
                    ConnectedComponents.StructuringElement.FOUR_CONNECTED);
            System.out.println("Found " + max_label + " glomeruli in layer " + i);

            if(!labels.isEmpty()) {
                // All connections from this layer -> labels of last layer
                Map<Integer, Set<Integer>> connections = new HashMap<>();

                // Look for connections to the last layer if available
                {
                    Img<UnsignedIntType> last_label = labels.get(labels.size() - 1);
                    RandomAccess<UnsignedIntType> lastLabelAccess = last_label.randomAccess();
                    Cursor<UnsignedIntType> cursor = label.cursor();
                    while(cursor.hasNext()) {
                        cursor.fwd();
                        lastLabelAccess.setPosition(cursor);
                        int current = cursor.get().getInteger();
                        if(current > 0) {
                            int last = lastLabelAccess.get().getInteger();
                            connections.putIfAbsent(current, new HashSet<>()); // Declare existence
                            if(last > 0) {
                                connections.get(current).add(last);
                            }
                        }
                    }
                }

                Map<Integer, Integer> local_renaming = new HashMap<>();
                Map<Integer, Integer> global_renaming = new HashMap<>();

                for(Map.Entry<Integer, Set<Integer>> kv : connections.entrySet()) {
                    if(kv.getValue().isEmpty()) {
                        ++global_max_label;
                        local_renaming.put(kv.getKey(), global_max_label);
                    }
                    else if(kv.getValue().size() == 1) {
                        local_renaming.put(kv.getKey(), kv.getValue().iterator().next());
                    }
                    else {
                        int target = kv.getValue().iterator().next();
                        local_renaming.put(kv.getKey(), target);

                        // Find all labels that are connected to target
                        Set<Integer> target_components = new HashSet<>();
                        for(Map.Entry<Integer, Set<Integer>> kv2 : connections.entrySet()) {
                            if(kv2.getValue().contains(target)) {
                                for(int src : kv2.getValue()) {
                                    if(src != target) {
                                        target_components.add(src);
                                    }
                                }
                            }
                        }

                        // Rename globally components -> target
                        for(int src : target_components) {
                            global_renaming.put(src, target);
                        }

                        // Fix * -> component to: * -> target
                        for(Map.Entry<Integer, Integer> kv2 : global_renaming.entrySet()) {
                            if(target_components.contains(kv2.getValue())) {
                                kv2.setValue(target);
                            }
                        }

                        // Fix local renaming
                        for(Map.Entry<Integer, Integer> kv2 : local_renaming.entrySet()) {
                            if(target_components.contains(kv2.getValue())) {
                                kv2.setValue(target);
                            }
                        }

                        // Apply global renaming to all connections
                        for(Map.Entry<Integer, Set<Integer>> kv2 : connections.entrySet()) {
                            Set<Integer> new_connections = new HashSet<>();
                            for(int src : kv2.getValue()) {
                                if(global_renaming.containsKey(src)) {
                                    new_connections.add(global_renaming.get(src));
                                }
                                else {
                                    new_connections.add(src);
                                }
                            }
                            kv2.setValue(new_connections);
                        }
                    }
                }

                // Renaming according to local renaming
                {
                    Cursor<UnsignedIntType> cursor = label.cursor();
                    while(cursor.hasNext()) {
                        cursor.fwd();
                        int l = cursor.get().getInteger();
                        if(l > 0) {
                            l = local_renaming.get(l);
                            cursor.get().set(l);
                        }
                    }
                }

                // Pass-through renaming
                for(int j = first_loaded_label_index; j < labels.size(); ++j) {
                    Img<UnsignedIntType> previous_label = labels.get(j);
                    Cursor<UnsignedIntType> cursor = previous_label.cursor();
                    while(cursor.hasNext()) {
                        cursor.fwd();
                        int l = cursor.get().getInteger();
                        if(l > 0 && global_renaming.containsKey(l)) {
                            l = global_renaming.get(l);
                            cursor.get().set(l);
                        }
                    }
                }

                // Move the first pass results into the current buffer
                labels.add(label);
                ++loaded_label_count;
            }
            else {
                // Set global max label to current glomeruli count
                global_max_label = max_label;
                labels.add(label);
                ++loaded_label_count;
            }

            while(loaded_label_count > limsize) {
                getDataInterface().getGlomeruli3DOutputData().setPlane(first_loaded_label_index, labels.get(first_loaded_label_index));
                labels.set(first_loaded_label_index, null);
                --loaded_label_count;
                ++first_loaded_label_index;
            }
        }

        while(loaded_label_count > 0) {
            getDataInterface().getGlomeruli3DOutputData().setPlane(first_loaded_label_index, labels.get(first_loaded_label_index));
            labels.set(first_loaded_label_index, null);
            --loaded_label_count;
            ++first_loaded_label_index;
        }
    }
}

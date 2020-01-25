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

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import org.hkijena.misa_imagej.api.MISAModuleInstance;
import org.hkijena.misa_imagej.api.MISASample;
import org.hkijena.misa_imagej.ui.components.renderers.MISASampleListCellRenderer;

import javax.swing.*;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class MISASampleComboBox extends JComboBox<MISASample> {

    private MISAModuleInstance moduleInstance;

    private EventBus eventBus = new EventBus();

    public MISASampleComboBox(MISAModuleInstance moduleInstance) {
        this();
        setModuleInstance(moduleInstance);
    }

    public MISASampleComboBox() {
        this.setRenderer(new MISASampleListCellRenderer());
        this.addItemListener(e -> {
            eventBus.post(new SelectionChangedEvent(this.getCurrentSample()));
        });
    }

    @Subscribe
    public void handleSampleListChangedEvent(MISAModuleInstance.AddedSampleEvent event) {
        refreshItems();
    }

    @Subscribe
    public void handleSampleListChangedEvent(MISAModuleInstance.RemovedSampleEvent event) {
        refreshItems();
    }

    @Subscribe
    public void handleSampleListChangedEvent(MISAModuleInstance.RenamedSampleEvent event) {
        refreshItems();
    }

    private void refreshItems() {
        if(moduleInstance == null || moduleInstance.getSamples().isEmpty()) {
            this.setModel(new DefaultComboBoxModel<>());
            this.setEnabled(false);
            this.setSelectedItem(null);
            eventBus.post(new SelectionChangedEvent(null));
        }
        else {
            MISASample current = getSelectedItem() instanceof MISASample ? (MISASample)getSelectedItem() : null;
            DefaultComboBoxModel<MISASample> model = new DefaultComboBoxModel<>();
            List<MISASample> sampleList = new ArrayList<>(moduleInstance.getSamples().values());
            sampleList.sort(Comparator.comparing(MISASample::getName));
            for(MISASample sample : sampleList) {
                model.addElement(sample);
            }
            this.setModel(model);
            this.setEnabled(true);
            this.setSelectedItem(null);
            if(current != null && sampleList.contains(current))
                this.setSelectedItem(current);
            else if (model.getSize() > 0)
                this.setSelectedItem(model.getElementAt(0));
            else
                this.setSelectedItem(null);
            eventBus.post(new SelectionChangedEvent(this.getCurrentSample()));
        }
    }

    public MISASample getCurrentSample() {
        return getSelectedItem() instanceof MISASample ? ((MISASample)getSelectedItem()) : null;
    }

    public EventBus getEventBus() {
        return eventBus;
    }

    public void setModuleInstance(MISAModuleInstance moduleInstance) {
        if(this.moduleInstance != null)
            this.moduleInstance.getEventBus().unregister(this);
        this.moduleInstance = moduleInstance;
        this.moduleInstance.getEventBus().register(this);
        refreshItems();
    }

    public static class SelectionChangedEvent {
        private MISASample sample;

        public SelectionChangedEvent(MISASample sample) {
            this.sample = sample;
        }

        public MISASample getSample() {
            return sample;
        }
    }
}

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

package org.hkijena.misa_imagej.ui.registries;

import org.hkijena.misa_imagej.ui.workbench.plotbuilder.MISAPlot;
import org.hkijena.misa_imagej.ui.workbench.plotbuilder.MISAPlotSeriesData;
import org.hkijena.misa_imagej.ui.workbench.plotbuilder.MISAPlotSettingsUI;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.*;

public class MISAPlotBuilderRegistry {

    private Map<Class<? extends MISAPlot>, Entry> entries = new HashMap<>();

    public void register(Class<? extends MISAPlot> plotType, Class<? extends MISAPlotSettingsUI> settingsType, String name, Icon icon) {
        entries.put(plotType, new Entry(plotType, settingsType, name, icon));
    }

    public Collection<Entry> getEntries() {
        return entries.values();
    }

    public String getNameOf(MISAPlot plot) {
        return entries.get(plot.getClass()).getName();
    }

    public Icon getIconOf(MISAPlot plot) {
        return entries.get(plot.getClass()).getIcon();
    }

    public List<MISAPlot> createAllPlots(List<MISAPlotSeriesData> seriesDataList) {
        List<MISAPlot> plots = new ArrayList<>();
        for(Entry entry : entries.values()) {
            try {
                plots.add(entry.getPlotType().getConstructor(List.class).newInstance(seriesDataList));
            } catch (InstantiationException | IllegalAccessException | InvocationTargetException | NoSuchMethodException e) {
                throw new RuntimeException(e);
            }
        }
        return plots;
    }

    public MISAPlotSettingsUI createSettingsUIFor(MISAPlot plot) {
        try {
            return entries.get(plot.getClass()).getSettingsType().getConstructor(MISAPlot.class).newInstance(plot);
        } catch (InstantiationException | IllegalAccessException | InvocationTargetException | NoSuchMethodException e) {
            throw new RuntimeException(e);
        }
    }

    public static class Entry {
        private Class<? extends MISAPlot> plotType;
        private Class<? extends MISAPlotSettingsUI> settingsType;
        private String name;
        private Icon icon;

        public Entry(Class<? extends MISAPlot> plotType, Class<? extends MISAPlotSettingsUI> settingsType, String name, Icon icon) {
            this.plotType = plotType;
            this.settingsType = settingsType;
            this.name = name;
            this.icon = icon;
        }

        public Class<? extends MISAPlot> getPlotType() {
            return plotType;
        }

        public String getName() {
            return name;
        }

        public Icon getIcon() {
            return icon;
        }

        public Class<? extends MISAPlotSettingsUI> getSettingsType() {
            return settingsType;
        }
    }
}

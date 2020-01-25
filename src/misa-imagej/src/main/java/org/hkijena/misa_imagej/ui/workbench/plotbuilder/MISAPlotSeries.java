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

package org.hkijena.misa_imagej.ui.workbench.plotbuilder;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;

import java.util.*;
import java.util.stream.Collectors;

public class MISAPlotSeries {
    private Map<String, MISAPlotSeriesColumn> columns = new HashMap<>();
    private Map<String, Object> parameters = new HashMap<>();
    private Map<String, Class> parameterTypes = new HashMap<>();
    private EventBus eventBus = new EventBus();
    private boolean enabled = true;

    public MISAPlotSeries() {

    }

    public void addColumn(String name, MISAPlotSeriesColumn column) {
        columns.put(name, column);
        column.getEventBus().register(this);
    }

    @Subscribe
    public void handleColumnDataChangedEvent(MISAPlotSeriesColumn.DataChangedEvent event) {
        eventBus.post(new DataChangedEvent(this));
    }

    public Map<String, MISAPlotSeriesColumn> getColumns() {
        return Collections.unmodifiableMap(columns);
    }

    public EventBus getEventBus() {
        return eventBus;
    }

    public void addParameter(String key, Object defaultValue) {
        parameters.put(key, Objects.requireNonNull(defaultValue));
        parameterTypes.put(key, defaultValue.getClass());
    }

    public Class getParameterType(String key) {
        return parameterTypes.get(key);
    }

    public Object getParameterValue(String key) {
        return parameters.get(key);
    }

    public void setParameterValue(String key, Object value) {
        parameters.put(key, Objects.requireNonNull(value));
        getEventBus().post(new DataChangedEvent(this));
    }

    public List<String> getParameterNames() {
        return parameters.keySet().stream().sorted().collect(Collectors.toList());
    }

    public MISANumericPlotSeriesColumn getAsNumericColumn(String name) {
        return (MISANumericPlotSeriesColumn)columns.get(name);
    }

    public MISAStringPlotSeriesColumn getAsStringColumn(String name) {
        return (MISAStringPlotSeriesColumn)columns.get(name);
    }

    public int getMaximumRequiredRowCount() {
        int count = 0;
        for(MISAPlotSeriesColumn column : columns.values()) {
            count = Math.max(count, column.getRequiredRowCount());
        }
        return count;
    }

    public boolean isEnabled() {
        return enabled;
    }

    public void setEnabled(boolean enabled) {
        this.enabled = enabled;
        getEventBus().post(new DataChangedEvent(this));
    }

    public static class DataChangedEvent {
        private MISAPlotSeries series;

        public DataChangedEvent(MISAPlotSeries series) {
            this.series = series;
        }

        public MISAPlotSeries getSeries() {
            return series;
        }
    }
}

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
import org.jfree.chart.JFreeChart;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public abstract class MISAPlot {

    protected List<MISAPlotSeries> series = new ArrayList<>();
    private EventBus eventBus = new EventBus();
    private List<MISAPlotSeriesData> seriesDataList;
    private String title = "Plot";

    protected MISAPlot(List<MISAPlotSeriesData> seriesDataList) {
        this.seriesDataList = seriesDataList;
    }

    public boolean canRemoveSeries() {
        return true;
    }

    public boolean canAddSeries() {
        return true;
    }

    public void addSeries() {
        if(canAddSeries()) {
            MISAPlotSeries s = createSeries();
            s.getEventBus().register(this);
            series.add(s);
            eventBus.post(new PlotSeriesListChangedEvent(this));
            eventBus.post(new PlotChangedEvent(this));
        }
    }

    public void removeSeries(MISAPlotSeries series) {
        if(canRemoveSeries()) {
            this.series.remove(series);
            eventBus.post(new PlotSeriesListChangedEvent(this));
            eventBus.post(new PlotChangedEvent(this));
        }
    }

    public void moveSeriesUp(MISAPlotSeries series) {
        int index = this.series.indexOf(series);
        if(index > 0) {
            this.series.set(index, this.series.get(index - 1));
            this.series.set(index - 1, series);
            eventBus.post(new PlotSeriesListChangedEvent(this));
            eventBus.post(new PlotChangedEvent(this));
        }
    }

    public void moveSeriesDown(MISAPlotSeries series) {
        int index = this.series.indexOf(series);
        if(index >= 0 && index < this.series.size() - 1) {
            this.series.set(index, this.series.get(index + 1));
            this.series.set(index + 1, series);
            eventBus.post(new PlotSeriesListChangedEvent(this));
            eventBus.post(new PlotChangedEvent(this));
        }
    }

    @Subscribe
    public void handleSeriesDataChangedEvent(MISAPlotSeries.DataChangedEvent event) {
        eventBus.post(new PlotChangedEvent(this));
    }

    protected abstract MISAPlotSeries createSeries();

    public abstract JFreeChart createPlot();

    public EventBus getEventBus() {
        return eventBus;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
        eventBus.post(new PlotChangedEvent(this));
    }

    public List<MISAPlotSeries> getSeries() {
        return Collections.unmodifiableList(series);
    }

    public List<MISAPlotSeriesData> getSeriesDataList() {
        return seriesDataList;
    }

    public static class PlotChangedEvent {
        private MISAPlot plot;

        public PlotChangedEvent(MISAPlot plot) {
            this.plot = plot;
        }

        public MISAPlot getPlot() {
            return plot;
        }
    }

    public static class PlotSeriesListChangedEvent {
        private MISAPlot plot;

        public PlotSeriesListChangedEvent(MISAPlot plot) {
            this.plot = plot;
        }

        public MISAPlot getPlot() {
            return plot;
        }
    }
}

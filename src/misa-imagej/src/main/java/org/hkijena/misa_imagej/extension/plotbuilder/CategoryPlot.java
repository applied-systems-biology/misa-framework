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

package org.hkijena.misa_imagej.extension.plotbuilder;

import org.hkijena.misa_imagej.ui.workbench.plotbuilder.*;
import org.jfree.chart.JFreeChart;
import org.jfree.data.category.CategoryDataset;

import java.util.List;

public abstract class CategoryPlot extends MISAPlot {

    private String categoryAxisLabel;
    private String valueAxisLabel;

    protected CategoryPlot(List<MISAPlotSeriesData> seriesDataList) {
        super(seriesDataList);
        addSeries();
    }


    @Override
    public boolean canRemoveSeries() {
        return false;
    }

    @Override
    public boolean canAddSeries() {
        return getSeries().size() == 0;
    }

    @Override
    protected MISAPlotSeries createSeries() {
        MISAPlotSeries series = new MISAPlotSeries();
        series.addColumn("X", new MISAStringPlotSeriesColumn(getSeriesDataList(),
                new MISAPlotSeriesGenerator<>("No category", x -> "No category"),
                new MISAPlotSeriesGenerator<>("Row number", x -> "x" + x)));
        series.addColumn("Category", new MISAStringPlotSeriesColumn(getSeriesDataList(),
                new MISAPlotSeriesGenerator<>("No category", x -> "No category")));
        series.addColumn("Value", new MISANumericPlotSeriesColumn(getSeriesDataList(),
                new MISAPlotSeriesGenerator<>("Row index", x -> (double)x)));
        return series;
    }

    public abstract CategoryDataset getDataset();

    protected abstract void updateDataset();

    protected abstract JFreeChart createPlotFromDataset();

    @Override
    public final JFreeChart createPlot() {
        updateDataset();
        return createPlotFromDataset();
    }

    public String getCategoryAxisLabel() {
        return categoryAxisLabel;
    }

    public void setCategoryAxisLabel(String categoryAxisLabel) {
        this.categoryAxisLabel = categoryAxisLabel;
        getEventBus().post(new PlotChangedEvent(this));
    }

    public String getValueAxisLabel() {
        return valueAxisLabel;
    }

    public void setValueAxisLabel(String valueAxisLabel) {
        this.valueAxisLabel = valueAxisLabel;
        getEventBus().post(new PlotChangedEvent(this));
    }
}

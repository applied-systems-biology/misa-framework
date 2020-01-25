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
import org.hkijena.misa_imagej.utils.StringUtils;
import org.jfree.chart.JFreeChart;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

public abstract class XYPlot extends MISAPlot {

    private String xAxisLabel = "X";
    private String yAxisLabel = "Y";
    private XYSeriesCollection dataset = new XYSeriesCollection();

    public XYPlot(List<MISAPlotSeriesData> seriesDataList) {
        super(seriesDataList);
        addSeries();
    }

    @Override
    public boolean canRemoveSeries() {
        return series.size() > 1;
    }

    @Override
    public boolean canAddSeries() {
        return true;
    }

    @Override
    protected MISAPlotSeries createSeries() {
        MISAPlotSeries series = new MISAPlotSeries();
        series.addParameter("Name", "Series");
        series.addColumn("X", new MISANumericPlotSeriesColumn(getSeriesDataList(),
                new MISAPlotSeriesGenerator<>("Row number", x -> (double)x)));
        series.addColumn("Y", new MISANumericPlotSeriesColumn(getSeriesDataList(),
                new MISAPlotSeriesGenerator<>("Row number", x -> (double)x)));
        return series;
    }

    protected abstract JFreeChart createPlotFromDataset(XYSeriesCollection dataset);

    protected void updateDataset() {
        dataset.removeAllSeries();
        Set<String> existingSeries = new HashSet<>();
        for(MISAPlotSeries seriesEntry : series) {
            if(!seriesEntry.isEnabled())
                continue;
            String name = StringUtils.makeUniqueString(seriesEntry.getParameterValue("Name").toString(), existingSeries);
            XYSeries chartSeries = new XYSeries(name, true);

            int rowCount = seriesEntry.getMaximumRequiredRowCount();

            List<Double> xValues = ((MISANumericPlotSeriesColumn)seriesEntry.getColumns().get("X")).getValues(rowCount);
            List<Double> yValues = ((MISANumericPlotSeriesColumn)seriesEntry.getColumns().get("Y")).getValues(rowCount);
            for(int i = 0; i < xValues.size(); ++i) {
                chartSeries.add(xValues.get(i), yValues.get(i));
            }
            dataset.addSeries(chartSeries);
            existingSeries.add(name);
        }
    }

    @Override
    public final JFreeChart createPlot() {
        updateDataset();
        return createPlotFromDataset(dataset);
    }

    public String getxAxisLabel() {
        return xAxisLabel;
    }

    public void setxAxisLabel(String xAxisLabel) {
        this.xAxisLabel = xAxisLabel;
        getEventBus().post(new PlotChangedEvent(this));
    }

    public String getyAxisLabel() {
        return yAxisLabel;
    }

    public void setyAxisLabel(String yAxisLabel) {
        this.yAxisLabel = yAxisLabel;
        getEventBus().post(new PlotChangedEvent(this));
    }

    public XYSeriesCollection getDataset() {
        return dataset;
    }
}

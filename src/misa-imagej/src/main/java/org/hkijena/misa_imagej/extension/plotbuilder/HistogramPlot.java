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

import com.google.common.primitives.Doubles;
import org.hkijena.misa_imagej.ui.workbench.plotbuilder.*;
import org.hkijena.misa_imagej.utils.StringUtils;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.renderer.xy.StandardXYBarPainter;
import org.jfree.chart.renderer.xy.XYBarRenderer;
import org.jfree.data.statistics.HistogramDataset;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class HistogramPlot extends MISAPlot {

    private String xAxisLabel = "Bin";
    private String yAxisLabel = "Number";

    public HistogramPlot(List<MISAPlotSeriesData> seriesDataList) {
        super(seriesDataList);
        setTitle("Histogram");
        addSeries();
    }

    @Override
    protected MISAPlotSeries createSeries() {
        MISAPlotSeries series = new MISAPlotSeries();
        series.addParameter("Name", "Category");
        series.addParameter("Bins", 10);
        series.addColumn("Values", new MISANumericPlotSeriesColumn(getSeriesDataList(),
                new MISAPlotSeriesGenerator<>("Zero", x -> 0.0)));
        return series;
    }

    @Override
    public JFreeChart createPlot() {
        HistogramDataset dataset = new HistogramDataset();
        Set<String> existingNames = new HashSet<>();
        for(MISAPlotSeries seriesEntry : series) {
            if(!seriesEntry.isEnabled())
                continue;
            int rowCount = Math.max(1, seriesEntry.getMaximumRequiredRowCount());
            List<Double> values = seriesEntry.getAsNumericColumn("Values").getValues(rowCount);
            String name = StringUtils.makeUniqueString((String)seriesEntry.getParameterValue("Name"), existingNames);
            dataset.addSeries(name, Doubles.toArray(values), (int)seriesEntry.getParameterValue("Bins"));
        }

        JFreeChart chart = ChartFactory.createHistogram(getTitle(), getxAxisLabel(), getyAxisLabel(), dataset);
        ((XYBarRenderer)chart.getXYPlot().getRenderer()).setBarPainter(new StandardXYBarPainter());
        return chart;
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

}

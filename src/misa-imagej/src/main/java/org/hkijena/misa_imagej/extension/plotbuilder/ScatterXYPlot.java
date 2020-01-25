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

import org.hkijena.misa_imagej.ui.workbench.plotbuilder.MISAPlotSeriesData;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.JFreeChart;
import org.jfree.data.xy.XYSeriesCollection;

import java.util.List;

public class ScatterXYPlot extends XYPlot {
    public ScatterXYPlot(List<MISAPlotSeriesData> seriesDataList) {
        super(seriesDataList);
        setTitle("XY Scatter Plot");
    }

    @Override
    protected JFreeChart createPlotFromDataset(XYSeriesCollection dataset) {
        return ChartFactory.createScatterPlot(getTitle(), getxAxisLabel(), getyAxisLabel(), dataset);
    }
}

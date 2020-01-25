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

import com.google.common.base.Joiner;
import org.hkijena.misa_imagej.api.MISAValidityReport;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;

public class MISAValidityReportUI extends JFrame {

    private MISAValidityReport report;
    private JTable table;

    public MISAValidityReportUI() {
        initialize();
    }

    private void initialize() {
        setSize(800, 600);
        getContentPane().setLayout(new BorderLayout(8, 8));
        setTitle("Detailed error log - MISA++ for ImageJ");
        setIconImage(UIUtils.getIconFromResources("misaxx.png").getImage());

        table = new JTable() {
            @Override
            public boolean isCellEditable(int i, int i1) {
                return false;
            }
        };
        add(new JScrollPane(table), BorderLayout.CENTER);
        add(table.getTableHeader(), BorderLayout.NORTH);
    }

    public MISAValidityReport getReport() {
        return report;
    }

    public void setReport(MISAValidityReport report) {
        this.report = report;
        refreshUI();
    }

    private void refreshUI() {
        DefaultTableModel model = new DefaultTableModel();
        if(report != null) {
            model.addColumn("Response");
            model.addColumn("Message");
            model.addColumn("Categories");
            model.addColumn("Object");
            for(MISAValidityReport.Entry entry : report.getEntries().values()) {
                model.addRow(new Object[]{
                        entry.getEntryType().name(),
                        entry.getMessage(),
                        Joiner.on(", ").join(entry.getCategories()),
                        entry.getObject().toString()
                });
            }
        }
        table.setModel(model);
    }
}

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

import org.hkijena.misa_imagej.api.MISAValidityReport;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.util.Map;

public class MISAValidityReportStatusUI extends JButton implements ActionListener {

    private LocalTime lastUpdate = LocalTime.now();
    private MISAValidityReport report;
    private MISAValidityReportUI detailedView = new MISAValidityReportUI();

    public MISAValidityReportStatusUI() {
        this.setBackground(Color.WHITE);
        this.setOpaque(false);
        this.setBorder(null);
        this.addActionListener(this);
        refreshUI();
    }

    public MISAValidityReport getReport() {
        return report;
    }

    public void setReport(MISAValidityReport report) {
        this.report = report;
        this.lastUpdate = LocalTime.now();
        refreshUI();
    }

    private String getLastUpdateString() {
        return lastUpdate.format(DateTimeFormatter.ofPattern("HH:mm:ss"));
    }

    private void refreshUI() {
        if(report == null || report.getResult() == MISAValidityReport.Entry.Type.Info) {
            this.setIcon(null);
            this.setText(getLastUpdateString() + " - No errors found");
        }
        else {
            StringBuilder message = new StringBuilder();
            message.append(getLastUpdateString());
            message.append(" - ");
            Map<Object, MISAValidityReport.Entry> interestingEntries =
                    report.getEntriesOfAtLeast(MISAValidityReport.Entry.Type.Warning);
            if(!interestingEntries.isEmpty()) {
                MISAValidityReport.Entry e = interestingEntries
                        .values().stream().findFirst().get();
                if(!e.getCategories().isEmpty()) {
                    message.append(e.getCategories().stream().findFirst().get());
                    if(e.getCategories().size() > 1)
                        message.append("...");
                    message.append(": ");
                }
                message.append(e.getMessage());

                if(interestingEntries.size() > 1) {
                    message.append(" (");
                    message.append(interestingEntries.size() - 1);
                    message.append(" more)");
                }
            }
            else {
                message.append("Parameters are invalid!");
            }
            this.setText(message.toString());

            switch(report.getResult()) {
                case Warning:
                    this.setIcon(UIUtils.getIconFromResources("warning.png"));
                    break;
                case Error:
                    this.setIcon(UIUtils.getIconFromResources("error.png"));
                    break;
            }
        }
    }

    @Override
    public void actionPerformed(ActionEvent actionEvent) {
        detailedView.setReport(report);
        detailedView.setVisible(true);
    }
}

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

package org.hkijena.misa_imagej.extension.datasources;

import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISADataSource;
import org.hkijena.misa_imagej.api.MISASample;
import org.hkijena.misa_imagej.ui.datasources.MISADataSourceUI;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.ColorIcon;

import javax.swing.*;
import java.awt.*;

public class MISAPipelineNodeDataSourceUI extends MISADataSourceUI {

    JComboBox<MISACache> cacheList;

    public MISAPipelineNodeDataSourceUI(MISADataSource dataSource) {
        super(dataSource);
        refreshDisplay();
    }

    @Override
    protected void initialize() {
        setLayout(new GridBagLayout());

        cacheList = new JComboBox<>();
        cacheList.setRenderer(new CacheListRenderer());
        cacheList.addItemListener(itemEvent -> {
            if(cacheList.getSelectedItem() != null) {
                getNativeDataSource().setSourceCache((MISACache) cacheList.getSelectedItem());
            }
        });
        add(cacheList, new GridBagConstraints() {
            {
                gridx = 0;
                gridy = 0;
                fill = GridBagConstraints.HORIZONTAL;
                weightx = 1;
            }
        });

        JButton applyButton = new JButton(UIUtils.getIconFromResources("copy.png"));
        applyButton.addActionListener(actionEvent -> applyToAllSamples());
        applyButton.setToolTipText("Apply this setting to equivalent data in all other samples");
        add(applyButton, new GridBagConstraints() {
            {
                gridx = 1;
                gridy = 0;
                fill = GridBagConstraints.VERTICAL;
            }
        });
    }

    private void applyToAllSamples() {
        if(JOptionPane.showConfirmDialog(this, "Apply this connection to all samples?",
                "Apply to all samples", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE) == JOptionPane.YES_OPTION) {
            for(MISASample sample : getNativeDataSource ().getCache().getSample().getModuleInstance().getSamples().values()) {
                if(sample != getNativeDataSource().getCache().getSample()) {
                    for(MISACache targetCache : sample.getImportedCaches()) {
                        // Look for the cache that is equivalent
                        if(targetCache.getRelativePath().equals(getNativeDataSource().getCache().getRelativePath())) {
                            // Find the correct data source that is coming from the same node as this data source
                            MISADataSource ds = targetCache.getAvailableDataSources().stream().filter(misaDataSource -> {
                                if(misaDataSource instanceof  MISAPipelineNodeDataSource) {
                                    MISAPipelineNodeDataSource p = (MISAPipelineNodeDataSource)misaDataSource;
                                    if(p.getSourceNode() == getNativeDataSource().getSourceNode()) {
                                        return true;
                                    }
                                }
                               return false;
                            }).findFirst().get();

                            // Find the equivalent source cache
                            MISACache sourceCache = getNativeDataSource().getSourceNode().getModuleInstance().getSample(sample.getName()).getExportedCacheByRelativePath(
                                    getNativeDataSource().getSourceCache().getRelativePath());
                            ((MISAPipelineNodeDataSource)ds).setSourceCache(sourceCache);
                            targetCache.setDataSource(ds);
                            break;
                        }
                    }
                }
            }
        }
    }

    private void refreshDisplay() {
        MutableComboBoxModel<MISACache> model = new DefaultComboBoxModel<>();
        String currentSample = getDataSource().getCache().getSample().getName();
        for(MISACache cache : getNativeDataSource().getSourceNode().getModuleInstance().
                getSample(currentSample).getExportedCaches()) {
            model.addElement(cache);
        }
        model.setSelectedItem(getNativeDataSource().getSourceCache());
        cacheList.setModel(model);

        if(model.getSelectedItem() != null) {
            getNativeDataSource().setSourceCache((MISACache) model.getSelectedItem());
        }
        else {
            // Select the first one that is available
            if(model.getSize() != 0) {
                getNativeDataSource().setSourceCache(model.getElementAt(0));
                refreshDisplay();
            }
        }
    }

    private MISAPipelineNodeDataSource getNativeDataSource() {
        return (MISAPipelineNodeDataSource)getDataSource();
    }

    private static class CacheListRenderer extends JPanel implements ListCellRenderer<MISACache> {

        private ColorIcon icon = new ColorIcon(21,21);
        private JLabel cacheLabel = new JLabel();
        private JLabel locationLabel = new JLabel();

        public CacheListRenderer() {
            initialize();
        }

        private void initialize() {
            setLayout(new GridBagLayout());
            add(new JLabel(icon), new GridBagConstraints() {
                {
                    gridx = 0;
                    gridy = 0;
                    gridheight = 2;
                    insets = new Insets(4, 8, 4, 4);
                }
            });
            add(cacheLabel, new GridBagConstraints() {
                {
                    gridx = 1;
                    gridy = 0;
                    weightx = 1;
                    fill = GridBagConstraints.HORIZONTAL;
                    insets = new Insets(4, 4, 0, 4);
                }
            });
            add(locationLabel, new GridBagConstraints() {
                {
                    gridx = 1;
                    gridy = 1;
                    weightx = 1;
                    fill = GridBagConstraints.HORIZONTAL;
                    insets = new Insets(0, 4, 4, 4);
                }
            });
            locationLabel.setFont(locationLabel.getFont().deriveFont(11.0f));
        }

        @Override
        public Component getListCellRendererComponent(JList<? extends MISACache> list, MISACache value,
                                                      int index, boolean isSelected, boolean hasFocus) {
            if(value != null) {
                icon.setColor( value.toColor());
                cacheLabel.setText(value.getCacheTypeName() + ": " +
                        value.getRelativePathName());
                locationLabel.setText( value.getSample().getModuleInstance().getName());
            }

            return this;

        }
    }
}

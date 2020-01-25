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

package org.hkijena.misa_imagej.ui.parametereditor;

import com.google.common.eventbus.Subscribe;
import org.hkijena.misa_imagej.MISAImageJRegistryService;
import org.hkijena.misa_imagej.api.MISACache;
import org.hkijena.misa_imagej.api.MISACacheIOType;
import org.hkijena.misa_imagej.api.MISADataSource;
import org.hkijena.misa_imagej.ui.datasources.MISADataSourceUI;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.*;

public class MISAInputCacheUI extends JPanel {

    private MISACache cache;

    private MISADataSourceUI editor;

    private JButton selectDataSourceButton;

    public MISAInputCacheUI(MISACache cache) {
        this.cache = cache;
        initialize();
    }

    private void initialize() {
        if(cache.getIOType() == MISACacheIOType.Imported) {
            setLayout(new BorderLayout());

            selectDataSourceButton = new JButton(UIUtils.getIconFromResources("database.png"));
            selectDataSourceButton.setToolTipText("Click this button to list all available data sources for the cache.");
            JPopupMenu menu = UIUtils.addPopupMenuToComponent(selectDataSourceButton);

            for(MISADataSource source : cache.getAvailableDataSources()) {
                JMenuItem menuItem = new JMenuItem(source.getName(), UIUtils.getIconFromResources("database.png"));
                menuItem.addActionListener(actionEvent -> {
                    cache.setDataSource(source);
                });
                menu.add(menuItem);
            }

            cache.getEventBus().register(this);

            if(cache.getDataSource() == null) {
                if(cache.getPreferredDataSource() != null) {
                    cache.setDataSource(cache.getPreferredDataSource());
                }
                else {
                    cache.setDataSource(cache.getAvailableDataSources().get(0));
                }
            }

            add(selectDataSourceButton, BorderLayout.EAST);
            updateEditorUI();
        }
    }

    @Subscribe
    public void handleDataSourceChangeEvent(MISACache.DataSourceChangeEvent event) {
        updateEditorUI();
    }

    private void updateEditorUI() {
        // Change data source button text
        if(cache.getDataSource() != null)
            selectDataSourceButton.setText(cache.getDataSource().getName());
        else
            selectDataSourceButton.setText(null);

        if(editor != null)
            remove(editor);
        editor = MISAImageJRegistryService.getInstance().getDataSourceUIRegistry().getEditorFor(cache.getDataSource());
        add(editor, BorderLayout.CENTER);
        revalidate();
        repaint();
    }

    public MISACache getCache() {
        return cache;
    }
}

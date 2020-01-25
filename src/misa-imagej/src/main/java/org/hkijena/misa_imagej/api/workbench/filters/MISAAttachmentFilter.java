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

package org.hkijena.misa_imagej.api.workbench.filters;

import com.google.common.eventbus.EventBus;
import org.hkijena.misa_imagej.api.workbench.MISAAttachmentDatabase;
import org.hkijena.misa_imagej.api.workbench.PreparedStatementValuesBuilder;

import java.sql.SQLException;

/**
 * A class that builds a SQL query to filter attachments
 * If the query changes, a {@link MISAAttachmentFilterChangedEvent} is triggered
 */
public abstract class MISAAttachmentFilter {

    private MISAAttachmentDatabase database;
    private EventBus eventBus = new EventBus();
    boolean enabled = true;

    protected MISAAttachmentFilter(MISAAttachmentDatabase database) {
        this.database = database;
    }

    /**
     * Returns the database
     * @return
     */
    public MISAAttachmentDatabase getDatabase() {
        return database;
    }

    /**
     * Creates equivalent SQL query to the toSQLStatement and setSQLStatementVariables methods
     * @return
     */
    public abstract String toSQLQuery();

    /**
     * Used to build a prepared statement
     * @return
     */
    public abstract String toSQLStatement();

    /**
     * Adds the missing variables one after another
     * @param builder
     */
    public abstract void setSQLStatementVariables(PreparedStatementValuesBuilder builder) throws SQLException;

    /**
     * Gets the event bus
     * @return
     */
    public EventBus getEventBus() {
        return eventBus;
    }

    /**
     * Returns true if the filter is enabled
     * @return
     */
    public boolean isEnabled() {
        return enabled;
    }

    /**
     * Enables or disables the filter
     * @param enabled
     */
    public void setEnabled(boolean enabled) {
        this.enabled = enabled;
        getEventBus().post(new MISAAttachmentFilterChangedEvent(this));
    }
}

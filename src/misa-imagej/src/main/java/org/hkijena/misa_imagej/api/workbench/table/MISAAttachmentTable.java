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

package org.hkijena.misa_imagej.api.workbench.table;

import com.google.common.eventbus.EventBus;
import com.google.gson.Gson;
import org.hkijena.misa_imagej.api.MISAAttachment;
import org.hkijena.misa_imagej.api.workbench.MISAAttachmentDatabase;
import org.hkijena.misa_imagej.utils.GsonUtils;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class MISAAttachmentTable {
    private MISAAttachmentDatabase database;
    private List<String> databaseFilters;
    private String serializationId;
    private List<MISAAttachmentTableColumn> columns = new ArrayList<>();
    private EventBus eventBus = new EventBus();

    public MISAAttachmentTable(MISAAttachmentDatabase database, List<String> databaseFilters, String serializationId) {
        this.database = database;
        this.databaseFilters = databaseFilters;
        this.serializationId = serializationId;
    }

    public void addColumn(MISAAttachmentTableColumn column) {
        columns.add(column);
        columns.sort(Comparator.comparing(c -> c instanceof MISAAttachmentTableJsonValueColumn)
                .thenComparing(c -> ((MISAAttachmentTableColumn)c).getName()));
        getEventBus().post(new ColumnsChangedEvent(this));
    }

    public void removeColumn(MISAAttachmentTableColumn column) {
        columns.remove(column);
        getEventBus().post(new ColumnsChangedEvent(this));
    }

    public EventBus getEventBus() {
        return eventBus;
    }

    public Iterator createIterator() {
        List<String> filters = new ArrayList<>(databaseFilters);
        filters.add("\"serialization-id\" is '" + getSerializationId() + "'");
        ResultSet resultSet = database.query("id, sample, cache, property, \"serialization-id\"",
                filters, "" );
        return new Iterator(this, columns, resultSet);
    }

    public List<MISAAttachmentTableColumn> getColumns() {
        return Collections.unmodifiableList(columns);
    }

    public MISAAttachmentDatabase getDatabase() {
        return database;
    }

    public String getSerializationId() {
        return serializationId;
    }

    public static class ColumnsChangedEvent {
        private MISAAttachmentTable table;

        public ColumnsChangedEvent(MISAAttachmentTable table) {
            this.table = table;
        }

        public MISAAttachmentTable getTable() {
            return table;
        }
    }

    public static class Iterator implements AutoCloseable {
        private MISAAttachmentTable table;
        private List<MISAAttachmentTableColumn> columns;
        private ResultSet resultSet;
        private Object[] rowBuffer;
        private Gson gson = GsonUtils.getGson();
        private int currentRow;

        public Iterator(MISAAttachmentTable table, List<MISAAttachmentTableColumn> columns, ResultSet resultSet) {
            this.table = table;
            this.columns = columns;
            this.resultSet = resultSet;
            this.rowBuffer = new Object[columns.size()];
            this.currentRow = 0;
        }

        public Object[] nextRow() throws SQLException  {
            if(!resultSet.next())
                return null;
            ++currentRow;

            int id = resultSet.getInt(1);
            MISAAttachment attachment = table.database.queryAttachmentAt(id);
            attachment.startLoadAllIteration();

            for(int i = 0; i < columns.size(); ++i) {
                rowBuffer[i] = columns.get(i).getValue(table,
                        id,
                        resultSet.getString("sample"),
                        resultSet.getString("cache"),
                        resultSet.getString("property"),
                        resultSet.getString("serialization-id"), attachment);
            }

            attachment.stopLoadAllIteration(true);

            return rowBuffer;
        }

        @Override
        public void close() throws SQLException {
            resultSet.close();
        }
    }
}

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

package org.hkijena.misa_imagej.api.workbench;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.google.gson.Gson;
import com.google.gson.JsonElement;
import org.hkijena.misa_imagej.api.MISAAttachment;
import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilter;
import org.hkijena.misa_imagej.api.workbench.filters.MISAAttachmentFilterChangedEvent;
import org.hkijena.misa_imagej.utils.GsonUtils;

import java.sql.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

public class MISAAttachmentDatabase {

    private MISAOutput misaOutput;
    private Connection databaseConnection;
    private List<MISAAttachmentFilter> filters = new ArrayList<>();
    private EventBus eventBus = new EventBus();

    public MISAAttachmentDatabase(MISAOutput misaOutput) {
        this.misaOutput = misaOutput;
        try {
            initialize();
        } catch (ClassNotFoundException | SQLException e) {
            throw new RuntimeException(e);
        }
    }

    private void initialize() throws ClassNotFoundException, SQLException {
        Class.forName("org.sqlite.JDBC");
        databaseConnection = DriverManager.getConnection("jdbc:sqlite:" + misaOutput.getRootPath().resolve("attachment-index.sqlite"));
    }

    public MISAOutput getMisaOutput() {
        return misaOutput;
    }

    public List<MISAAttachmentFilter> getFilters() {
        return Collections.unmodifiableList(filters);
    }

    public EventBus getEventBus() {
        return eventBus;
    }

    public void addFilter(MISAAttachmentFilter filter) {
        if(!filters.contains(filter)) {
            filters.add(filter);
            getEventBus().post(new AddedFilterEvent(this, filter));
            filter.getEventBus().register(this);
        }
    }

    public void removeFilter(MISAAttachmentFilter filter) {
        filters.remove(filter);
        getEventBus().post(new RemovedFilterEvent(this, filter));
        filter.getEventBus().unregister(this);
    }

    private String createQueryStatementTemplate(String selectionStatement, List<String> additionalFilters, String postStatement) {
        StringBuilder template = new StringBuilder();
        template.append("select ").append(selectionStatement).append(" from attachments");

        List<MISAAttachmentFilter> enabledFilters = filters.stream().filter(MISAAttachmentFilter::isEnabled).collect(Collectors.toList());

        if(!enabledFilters.isEmpty() || !additionalFilters.isEmpty()) {
            template.append(" where");
            boolean first = true;
            for(MISAAttachmentFilter filter : enabledFilters) {
                if(!first) {
                    template.append(" and ");
                }
                else {
                    template.append(" ");
                    first = false;
                }
                template.append(filter.toSQLStatement());
            }
            for(String filter : additionalFilters) {
                if(!first) {
                    template.append(" and ");
                }
                else {
                    template.append(" ");
                    first = false;
                }
                template.append(filter);
            }
        }

        template.append(" ").append(postStatement);

        return template.toString();
    }

    public PreparedStatement createQueryStatement(String selectionStatement, List<String> additionalFilters, String postStatement) {

        List<MISAAttachmentFilter> enabledFilters = filters.stream().filter(MISAAttachmentFilter::isEnabled).collect(Collectors.toList());

        try {
            PreparedStatement statement = databaseConnection.prepareStatement(createQueryStatementTemplate(selectionStatement, additionalFilters, postStatement));
            PreparedStatementValuesBuilder builder = new PreparedStatementValuesBuilder(statement);
            for(MISAAttachmentFilter filter : enabledFilters) {
                filter.setSQLStatementVariables(builder);
            }
            statement.closeOnCompletion();
            return statement;
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public ResultSet query(String selectionStatement, List<String> additionalFilters, String postStatement) {
        try {
            return createQueryStatement(selectionStatement, additionalFilters, postStatement).executeQuery();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Returns the query as SQL
     * @param selectionStatement
     * @return
     */
    public String getQuerySQL(String selectionStatement, List<String> additionalFilters, String postStatement) {
        StringBuilder sql = new StringBuilder();
        sql.append("select ").append(selectionStatement).append(" from attachments");

        List<MISAAttachmentFilter> enabledFilters = filters.stream().filter(MISAAttachmentFilter::isEnabled).collect(Collectors.toList());

        if(!enabledFilters.isEmpty() || !additionalFilters.isEmpty()) {
            sql.append(" where");
            boolean first = true;
            for(MISAAttachmentFilter filter : enabledFilters) {
                if(!first) {
                    sql.append(" and ");
                }
                else {
                    sql.append(" ");
                    first = false;
                }
                sql.append(filter.toSQLQuery());
            }
            for(String filter : additionalFilters) {
                if(!first) {
                    sql.append(" and ");
                }
                else {
                    sql.append(" ");
                    first = false;
                }
                sql.append(filter);
            }
        }

        sql.append(" ").append(postStatement);

        return sql.toString();
    }

    private ResultSet queryAt(String selectionStatement, int id) {
        assert id > 0;
        StringBuilder sql = new StringBuilder();
        sql.append("select ").append(selectionStatement).append(" from attachments").append(" where id is ?");
        try {
            PreparedStatement statement = databaseConnection.prepareStatement(sql.toString());
            PreparedStatementValuesBuilder builder = new PreparedStatementValuesBuilder(statement);
            builder.addInt(id);
            return statement.executeQuery();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Queries JSON data at a specified ID
     * @param id
     * @return
     */
    public JsonElement queryJsonDataAt(int id) {
        assert id > 0;
        try (ResultSet resultSet = queryAt("\"json-data\"", id)) {
            assert resultSet.next();
            String json = resultSet.getString(1);
            Gson gson = GsonUtils.getGson();
            return gson.fromJson(json, JsonElement.class);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Returns an attachment that queries data from given database ID
     * @param id
     * @return
     */
    public MISAAttachment queryAttachmentAt(int id) {
        assert id > 0;
        try (ResultSet resultSet =  queryAt("sample, cache, property", id)) {
            assert resultSet.next();
            String path = resultSet.getString(1) + "/" + resultSet.getString(2) + "/" + resultSet.getString(3);
            return new MISAAttachment(this, id, path);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public Iterator createAttachmentIterator(List<String> filters) {
        ResultSet resultSet = query("id, sample, cache, property", filters, "");
        return new Iterator(this, resultSet);
    }


    @Subscribe
    public void handleFilterUpdateEvent(MISAAttachmentFilterChangedEvent event) {
        getEventBus().post(new UpdatedFiltersEvent(this));
    }

    public static class AddedFilterEvent {
        private MISAAttachmentDatabase database;
        private MISAAttachmentFilter filter;

        public AddedFilterEvent(MISAAttachmentDatabase database, MISAAttachmentFilter filter) {
            this.database = database;
            this.filter = filter;
        }

        public MISAAttachmentDatabase getDatabase() {
            return database;
        }

        public MISAAttachmentFilter getFilter() {
            return filter;
        }
    }

    public static class RemovedFilterEvent {
        private MISAAttachmentDatabase database;
        private MISAAttachmentFilter filter;

        public RemovedFilterEvent(MISAAttachmentDatabase database, MISAAttachmentFilter filter) {
            this.database = database;
            this.filter = filter;
        }

        public MISAAttachmentDatabase getDatabase() {
            return database;
        }

        public MISAAttachmentFilter getFilter() {
            return filter;
        }
    }

    public static class UpdatedFiltersEvent {
        private MISAAttachmentDatabase database;

        public UpdatedFiltersEvent(MISAAttachmentDatabase database) {
            this.database = database;
        }

        public MISAAttachmentDatabase getDatabase() {
            return database;
        }
    }

    public static class Iterator implements AutoCloseable {
        private MISAAttachmentDatabase database;
        private ResultSet resultSet;

        public Iterator(MISAAttachmentDatabase database, ResultSet resultSet) {
            this.database = database;
            this.resultSet = resultSet;
        }

        public MISAAttachment nextAttachment() throws SQLException {
            if(!resultSet.next())
                return null;
            String path = resultSet.getString(2) + "/" + resultSet.getString(3) + "/" + resultSet.getString(4);
            return new MISAAttachment(database, resultSet.getInt(1), path);
        }

        @Override
        public void close() throws SQLException {
            resultSet.close();
        }
    }
}

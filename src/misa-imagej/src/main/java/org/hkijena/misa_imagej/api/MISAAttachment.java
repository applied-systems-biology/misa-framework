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

package org.hkijena.misa_imagej.api;

import com.google.common.eventbus.EventBus;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonPrimitive;
import org.hkijena.misa_imagej.api.json.JSONSchemaObject;
import org.hkijena.misa_imagej.api.workbench.MISAAttachmentDatabase;

import java.awt.*;
import java.util.*;
import java.util.List;
import java.util.stream.Collectors;

/**
 * Wrapper around attached data that allows backtracking to its cache
 * The attachment is lazily loaded on demand
 */
public class MISAAttachment {
    private MISAAttachmentDatabase database;
    private int databaseIndex;
    boolean isLoaded;
    private EventBus eventBus = new EventBus();
    private List<Property> properties = new ArrayList<>();
    private JSONSchemaObject schema = null;
    private String attachmentFullPath;

    private List<Property> transactionBackupProperties;

    public MISAAttachment(MISAAttachmentDatabase database, int databaseIndex, String attachmentFullPath) {
        this.database = database;
        this.databaseIndex = databaseIndex;
        this.attachmentFullPath = attachmentFullPath;
    }

    public MISAAttachmentDatabase getDatabase() {
        return database;
    }

    public boolean hasData() {
        return isLoaded;
    }

    public String getDocumentationTitle() {
        if (schema != null) {
            return schema.getDocumentationTitle();
        } else {
            return "Unknown";
        }
    }

    public Color toColor() {
        if (schema != null) {
            return schema.toColor();
        } else {
            return Color.GRAY;
        }
    }

    public Property getProperty(String path) {
        Optional<Property> property = properties.stream().filter(e -> e.getPath().equals(path)).findFirst();
        if (property.isPresent()) {
            return property.get();
        } else {

            do {
                property = properties.stream().filter(e -> path.startsWith(e.getPath())).max(Comparator.comparing(property1 -> property1.getPath().length()));
                if (property.isPresent()) {
                    if (property.get().getPath().equals(path)) {
                        return property.get();
                    } else if (property.get() instanceof LazyProperty) {
                        if(property.get().hasValue())
                            break;
                        property.get().loadValue();
                    }
                } else {
                    break;
                }
            }
            while (true);

            return null;
        }
    }

    /**
     * Splits a database path into a path of strings
     *
     * @param path
     * @return
     */
    private static List<String> segmentPath(String path) {
        List<String> result = new ArrayList<>();
        StringBuilder current = new StringBuilder();
        boolean withinQuote = false;
        boolean nextCharacterIsEscaped = false;
        for (int i = 0; i < path.length(); ++i) {
            char c = path.charAt(i);
            if (nextCharacterIsEscaped) {
                current.append(c);
                nextCharacterIsEscaped = false;
            } else if (c == '\\') {
                nextCharacterIsEscaped = true;
            } else if (c == '\"') {
                withinQuote = !withinQuote;
            } else if (c == '/' && withinQuote) {
                current.append(c);
            } else if (c == '/') {
                result.add(current.toString());
                current.setLength(0);
            } else {
                current.append(c);
            }
        }
        if (current.length() > 0) {
            result.add(current.toString());
        }
        return result;
    }

    /**
     * Gets the full JSON object
     *
     * @return
     */
    public JsonObject getFullJson() {
        JsonObject result = database.queryJsonDataAt(databaseIndex).getAsJsonObject();

        // Explore other data
        Stack<JsonElement> stack = new Stack<>();
        stack.push(result);

        while (!stack.isEmpty()) {
            JsonElement current = stack.pop();
            if (current.isJsonObject()) {
                JsonObject currentObject = current.getAsJsonObject();
                for (String key : new HashSet<>(current.getAsJsonObject().keySet())) {
                    if (currentObject.get(key).isJsonObject() && currentObject.getAsJsonObject(key).has("misa-analyzer:database-index")) {
                        int dbIndex = currentObject.getAsJsonObject(key).getAsJsonPrimitive("misa-analyzer:database-index").getAsInt();
                        JsonElement newObject = database.queryJsonDataAt(dbIndex);
                        currentObject.remove(key);
                        currentObject.add(key, newObject);

                        if (newObject.isJsonObject())
                            stack.push(newObject.getAsJsonObject());
                    } else {
                        stack.push(currentObject.get(key));
                    }
                }
            } else if (current.isJsonArray()) {
                for (int i = 0; i < current.getAsJsonArray().size(); ++i) {
                    JsonElement item = current.getAsJsonArray().get(i);
                    if (item.isJsonObject() && item.getAsJsonObject().has("misa-analyzer:database-index")) {
                        int dbIndex = item.getAsJsonObject().getAsJsonPrimitive("misa-analyzer:database-index").getAsInt();
                        JsonElement newObject = database.queryJsonDataAt(dbIndex);
                        current.getAsJsonArray().set(i, newObject);

                        if (newObject.isJsonObject())
                            stack.push(newObject.getAsJsonObject());
                    } else {
                        stack.push(current.getAsJsonArray().get(i));
                    }
                }
            }
        }

        return result;
    }

    public void load() {
        if (!isLoaded) {
            JsonObject object = database.queryJsonDataAt(databaseIndex).getAsJsonObject();

            if (object.has("misa:serialization-id")) {
                String serializationId = object.get("misa:serialization-id").getAsString();
                schema = database.getMisaOutput().getAttachmentSchemas().getOrDefault(serializationId, null);
            }

            loadProperties(object, schema, "", "");
            isLoaded = true;
        }
    }

    public EventBus getEventBus() {
        return eventBus;
    }

    private void loadProperties(JsonObject rootObject, JSONSchemaObject rootSchema, String subPath, String subNamePath) {
        Stack<JsonElement> elements = new Stack<>();
        Stack<String> paths = new Stack<>();
        Stack<String> namePaths = new Stack<>();
        Stack<JSONSchemaObject> schemas = new Stack<>();
        elements.push(rootObject);
        paths.push(subPath);
        namePaths.push(subNamePath);
        schemas.push(rootSchema);

        while (!elements.isEmpty()) {
            JsonElement top = elements.pop();
            String topPath = paths.pop();
            String topNamePath = namePaths.pop();
            JSONSchemaObject topSchema = schemas.pop();

            if (top.isJsonPrimitive()) {
                String description = null;
                if(topSchema != null && topSchema.getDocumentationDescription() != null && !topSchema.getDocumentationDescription().isEmpty())
                    description = topSchema.getDocumentationDescription();
                properties.add(new MemoryProperty(topPath, topNamePath, description, top.getAsJsonPrimitive(), topSchema));
            } else if (top.isJsonObject()) {
                if (top.getAsJsonObject().has("misa-analyzer:database-index")) {
                    int dbId = top.getAsJsonObject().get("misa-analyzer:database-index").getAsInt();
                    String description = null;
                    if(topSchema != null && topSchema.getDocumentationDescription() != null && !topSchema.getDocumentationDescription().isEmpty())
                        description = topSchema.getDocumentationDescription();
                    properties.add(new LazyProperty(this, topPath, topNamePath, description, dbId));
                } else {
                    for (Map.Entry<String, JsonElement> entry : top.getAsJsonObject().entrySet()) {
                        if (entry.getKey().equals("misa:serialization-id") || entry.getKey().equals("misa:serialization-hierarchy"))
                            continue;

                        String name = entry.getKey();

                        elements.push(entry.getValue());
                        paths.push(topPath + "/" + entry.getKey());

                        if (topSchema != null && topSchema.hasPropertyFromPath(entry.getKey())) {
                            JSONSchemaObject subSchema = topSchema.getPropertyFromPath(entry.getKey());
                            if(subSchema != null && subSchema.getDocumentationTitle() != null && !subSchema.getDocumentationTitle().isEmpty())
                                name = subSchema.getDocumentationTitle();
                            schemas.push(subSchema);
                        } else {
                            schemas.push(null);
                        }

                        namePaths.push(topNamePath + "/" + name);
                    }
                }
            } else if (top.isJsonArray()) {
                for (int i = 0; i < top.getAsJsonArray().size(); ++i) {
                    elements.push(top.getAsJsonArray().get(i));
                    paths.push(topPath + "/[" + i + "]");
                    namePaths.push(topPath + "/[" + i + "]");
                    if (topSchema != null && topSchema.getAdditionalItems() != null) {
                        schemas.push(topSchema.getAdditionalItems());
                    } else {
                        schemas.push(null);
                    }
                }
            }
        }

        if (!isWithinLoadAllIteration())
            getEventBus().post(new MISAAttachment.DataLoadedEvent(this));
    }

    public List<Property> getProperties() {
        return Collections.unmodifiableList(properties);
    }

    public List<Property> getUnloadedProperties() {
        return properties.stream().filter(property -> !property.hasValue()).collect(Collectors.toList());
    }

    public void loadAll() {
        load();
        Optional<Property> property;
        while ((property = properties.stream().filter(p -> !p.hasValue()).findFirst()).isPresent()) {
            property.get().loadValue();
        }
    }

    public String getAttachmentFullPath() {
        return attachmentFullPath;
    }

    public boolean isWithinLoadAllIteration() {
        return transactionBackupProperties != null;
    }

    public boolean doLoadAllIteration() {
        if (isWithinLoadAllIteration()) {
            Optional<Property> property = properties.stream().filter(p -> !p.hasValue()).findFirst();
            if (!property.isPresent())
                return false;
            else
                property.get().loadValue();
            return true;
        }
        return false;
    }

    public void startLoadAllIteration() {
        if (!isWithinLoadAllIteration()) {
            load();
            transactionBackupProperties = properties;
            properties = new ArrayList<>(properties); // Backup the current property list
        }
    }

    public void stopLoadAllIteration(boolean canceled) {
        if (isWithinLoadAllIteration()) {
            if (canceled) {
                properties = transactionBackupProperties;
                transactionBackupProperties = null;

                for (Property property : properties) {
                    property.cancelLoadValue();
                }
            } else {
                transactionBackupProperties = null;
                getEventBus().post(new MISAAttachment.DataLoadedEvent(this));
            }
        }
    }

    public interface Property {
        String getPath();

        String getNamePath();

        String getDescription();

        JSONSchemaObject getSchema();

        void loadValue();

        void cancelLoadValue();

        boolean hasValue();
    }

    public static class MemoryProperty implements Property {
        private String path;
        private String namePath;
        private String description;
        private JsonPrimitive value;
        private JSONSchemaObject schema;

        public MemoryProperty(String path, String namePath, String description, JsonPrimitive value, JSONSchemaObject schema) {
            this.path = path;
            this.namePath = namePath;
            this.description = description;
            this.value = value;
            this.schema = schema;
        }

        @Override
        public String getPath() {
            return path;
        }

        public JsonPrimitive getValue() {
            return value;
        }

        @Override
        public boolean hasValue() {
            return true;
        }

        @Override
        public JSONSchemaObject getSchema() {
            return schema;
        }

        @Override
        public void loadValue() {

        }

        @Override
        public void cancelLoadValue() {

        }

        public String getNamePath() {
            return namePath;
        }

        @Override
        public String getDescription() {
            return description;
        }
    }

    public static class LazyProperty implements Property {
        private String path;
        private String namePath;
        private String description;
        private boolean isLoaded;
        private JSONSchemaObject schema;
        private MISAAttachment parent;
        private int databaseIndex;

        public LazyProperty(MISAAttachment parent, String path, String namePath, String description, int databaseIndex) {
            this.path = path;
            this.parent = parent;
            this.namePath = namePath;
            this.description = description;
            this.databaseIndex = databaseIndex;
        }

        public String getDocumentationTitle() {
            if (schema != null) {
                return schema.getDocumentationTitle();
            } else {
                return "Unknown";
            }
        }

        public Color toColor() {
            if (schema != null) {
                return schema.toColor();
            } else {
                return Color.GRAY;
            }
        }

        public void loadValue() {
            if (!isLoaded) {
                JsonObject object = parent.database.queryJsonDataAt(databaseIndex).getAsJsonObject();

                if (object.has("misa:serialization-id")) {
                    String serializationId = object.get("misa:serialization-id").getAsString();
                    this.schema = parent.database.getMisaOutput().getAttachmentSchemas().getOrDefault(serializationId, null);
                }

                isLoaded = true;
                parent.loadProperties(object, schema, path, namePath);
            }
        }

        @Override
        public void cancelLoadValue() {
            isLoaded = false;
        }

        @Override
        public boolean hasValue() {
            return isLoaded;
        }

        @Override
        public String getPath() {
            return path;
        }

        @Override
        public JSONSchemaObject getSchema() {
            return schema;
        }

        public String getNamePath() {
            return namePath;
        }

        @Override
        public String getDescription() {
            return description;
        }
    }

    public static class DataLoadedEvent {
        private MISAAttachment attachment;

        public DataLoadedEvent(MISAAttachment attachment) {
            this.attachment = attachment;
        }

        public MISAAttachment getAttachment() {
            return attachment;
        }
    }
}

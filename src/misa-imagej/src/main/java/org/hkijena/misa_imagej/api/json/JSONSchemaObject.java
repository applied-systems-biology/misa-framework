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

package org.hkijena.misa_imagej.api.json;

import com.google.common.eventbus.EventBus;
import com.google.gson.JsonElement;
import com.google.gson.JsonNull;
import com.google.gson.JsonObject;
import com.google.gson.JsonPrimitive;
import com.google.gson.annotations.SerializedName;
import org.hkijena.misa_imagej.api.MISAValidatable;
import org.hkijena.misa_imagej.api.MISAValidityReport;
import org.hkijena.misa_imagej.utils.GsonUtils;

import javax.swing.tree.DefaultMutableTreeNode;
import java.awt.*;
import java.util.*;
import java.util.List;

/**
 * Deserialized JSON Schema entry that allows editing and exporting to the final JSON
 */
public class JSONSchemaObject implements Cloneable, MISAValidatable {

    private transient String id;

    private transient JSONSchemaObject parent = null;

    private transient EventBus eventBus = new EventBus();

    private transient Object value = null;

    @SerializedName("type")
    private JSONSchemaObjectType type;

//    @SerializedName("description")
//    private String description = null;

    @SerializedName("properties")
    private HashMap<String, JSONSchemaObject> properties = new HashMap<>();

    @SerializedName("additionalItems")
    private JSONSchemaObject additionalItems = null;

    @SerializedName("default")
    private Object defaultValue = null;

    @SerializedName("enum")
    private List<Object> enumValues = null;

    @SerializedName("required")
    private List<String> requiredProperties = new ArrayList<>();

    @SerializedName("additionalProperties")
    private JSONSchemaObject additionalProperties = null;

    @SerializedName("misa:serialization-id")
    private String serializationId = null;

    @SerializedName("misa:serialization-hierarchy")
    private List<String> serializationHierarchy = new ArrayList<>();

    @SerializedName("misa:documentation-title")
    private String documentationTitle = null;

    @SerializedName("misa:documentation-description")
    private String documentationDescription = null;

    @SerializedName("misa:documentation-type-title")
    private String documentationTypeTitle = null;

    @SerializedName("misa:documentation-type-description")
    private String documentationTypeDescription = null;

    public JSONSchemaObject() {
    }

    public JSONSchemaObject(JSONSchemaObjectType type) {
        this.type = type;
    }

    public static JSONSchemaObject createObject() {
        JSONSchemaObject result = new JSONSchemaObject(JSONSchemaObjectType.jsonObject);
        return result;
    }

    public static JSONSchemaObject createArray(List<Object> value) {
        JSONSchemaObject result = new JSONSchemaObject(JSONSchemaObjectType.jsonArray);
        result.setValue(value);
        return result;
    }

    public static JSONSchemaObject createNumber(double number) {
        JSONSchemaObject result = new JSONSchemaObject(JSONSchemaObjectType.jsonNumber);
        result.setValue(number);
        return result;
    }

    public static JSONSchemaObject createBoolean(boolean b) {
        JSONSchemaObject result = new JSONSchemaObject(JSONSchemaObjectType.jsonBoolean);
        result.setValue(b);
        return result;
    }

    public static JSONSchemaObject createString(String string) {
        JSONSchemaObject result = new JSONSchemaObject(JSONSchemaObjectType.jsonString);
        result.setValue(string);
        return result;
    }

    @Override
    public Object clone() {
        JSONSchemaObject obj = new JSONSchemaObject();
        obj.type = getType();
//        obj.setDescription(getDescription());
        obj.documentationTitle = this.documentationTitle;
        obj.documentationDescription = this.documentationDescription;
        obj.documentationTypeTitle = this.documentationTypeTitle;
        obj.documentationTypeDescription = this.documentationTypeDescription;
        obj.setDefaultValue(getDefaultValue());
        obj.setEnumValues(getEnumValues());
        obj.setValue(getValue());
        obj.setRequiredProperties(getRequiredProperties());
        obj.setAdditionalProperties(getAdditionalProperties());
        obj.setSerializationId(getSerializationId());
        obj.setProperties(new HashMap<>());
        for (Map.Entry<String, JSONSchemaObject> kv : getProperties().entrySet()) {
            obj.getProperties().put(kv.getKey(), (JSONSchemaObject) kv.getValue().clone());
        }
        obj.setSerializationHierarchy(new ArrayList<>());
        obj.getSerializationHierarchy().addAll(getSerializationHierarchy());
        obj.update();
        return obj;
    }

    /**
     * Guaranteed way of returning additional properties (because this one might be null)
     *
     * @return
     */
    public JSONSchemaObject getAdditionalPropertiesTemplate() {
        JSONSchemaObject obj;
        if (getAdditionalProperties() != null) {
            obj = (JSONSchemaObject) getAdditionalProperties().clone();
        } else {
            obj = createObject();
        }
        return obj;
    }

    /**
     * Instantiates the additional property stored in this object schema
     *
     * @param name
     * @return
     */
    public JSONSchemaObject addAdditionalProperty(String name) {
        JSONSchemaObject obj = getAdditionalPropertiesTemplate();
        getProperties().put(name, obj);
        obj.setId(name);
        obj.setParent(this);
        obj.update();

        // Notify the change
        getEventBus().post(new AddedAdditionalPropertyEvent(obj));

        return obj;
    }

    public void removeAdditionalProperty(String name) {
        JSONSchemaObject obj = getProperties().get(name);
        getProperties().remove(name);

        // Notify the change
        getEventBus().post(new RemovedAdditionalPropertyEvent(obj));
    }

    public void update() {
        // Set the default value
        if (getValue() == null) {
            setValue(getDefaultValue());
        }

        // Update the map Ids & parent
        for (Map.Entry<String, JSONSchemaObject> kv : getProperties().entrySet()) {
            kv.getValue().setId(kv.getKey());
            kv.getValue().setParent(this);
            kv.getValue().update();
        }
    }

    /**
     * Returns the max depth of this object and its properties
     *
     * @return
     */
    public int getMaxDepth() {
        if (getProperties().isEmpty())
            return 0;
        else {
            int d = 0;
            for (Map.Entry<String, JSONSchemaObject> kv : getProperties().entrySet()) {
                d = Math.max(d, kv.getValue().getMaxDepth() + 1);
            }
            return d;
        }
    }

    /**
     * Returns the depth of this object
     *
     * @return
     */
    public int getDepth() {
        if (getParent() == null)
            return 0;
        else
            return getParent().getDepth() + 1;
    }

    public String getName() {
        return (getDocumentationTitle() == null || getDocumentationTitle().isEmpty()) ? getId() : getDocumentationTitle();
    }

    @Override
    public String toString() {
        return getValuePath();
    }

    public DefaultMutableTreeNode toTreeNode() {
        DefaultMutableTreeNode nd = new DefaultMutableTreeNode(this);

        ArrayList<JSONSchemaObject> objects = new ArrayList<>(getProperties().values());
        objects.sort(Comparator.comparingInt(JSONSchemaObject::getMaxDepth));

        for (JSONSchemaObject obj : objects) {
            nd.add(obj.toTreeNode());
        }

        return nd;
    }

    public JsonElement toJson() {
        switch (getType()) {
            case jsonString:
            case jsonNumber:
            case jsonBoolean:
                if (getValue() instanceof String)
                    return new JsonPrimitive((String) getValue());
                else if (getValue() instanceof Number)
                    return new JsonPrimitive((Number) getValue());
                else if (getValue() instanceof Boolean)
                    return new JsonPrimitive((Boolean) getValue());
                else
                    return JsonNull.INSTANCE;
            case jsonArray: {
                return GsonUtils.getGson().toJsonTree(getValue());
            }
            case jsonObject: {
                if(properties.isEmpty() && getValue() != null) {
                    return GsonUtils.getGson().toJsonTree(getValue());
                }else {
                    JsonObject result = new JsonObject();
                    for (Map.Entry<String, JSONSchemaObject> kv : getProperties().entrySet()) {
                        result.add(kv.getKey(), kv.getValue().toJson());
                    }
                    return result;
                }
            }
            default:
                throw new RuntimeException("Unknown type " + getType());
        }
    }

    public String getValuePath() {
        if (getParent() == null)
            return "";
        else
            return getParent().getValuePath() + "/" + getId();
    }

    /**
     * Gets a sub-property from path
     *
     * @param path
     * @return
     */
    public JSONSchemaObject getPropertyFromPath(String... path) {
        JSONSchemaObject result = this;
        for (String v : path) {
            result = result.getProperties().get(v);
        }
        return result;
    }

    public boolean hasPropertyFromPath(String... path) {
        JSONSchemaObject current = this;
        for (String v : path) {
            if (current.getProperties().containsKey(v))
                current = current.getProperties().get(v);
            else {
                return false;
            }
        }
        return true;
    }

    public JSONSchemaObject addProperty(String key, JSONSchemaObject property) {
        JSONSchemaObject copy = (JSONSchemaObject) property.clone();
        getProperties().put(key, copy);
        update();
        return copy;
    }

    public JSONSchemaObject ensurePropertyFromPath(String... path) {
        JSONSchemaObject current = this;
        for (String v : path) {
            if (current.getProperties().containsKey(v))
                current = current.getProperties().get(v);
            else
                current = current.addProperty(v, JSONSchemaObject.createObject());
        }
        return current;
    }

    private void flatten_(List<JSONSchemaObject> result) {
        result.add(this);
        for (Map.Entry<String, JSONSchemaObject> kv : getProperties().entrySet()) {
            kv.getValue().flatten_(result);
        }
    }

    public List<JSONSchemaObject> flatten() {
        ArrayList<JSONSchemaObject> result = new ArrayList<>();
        flatten_(result);
        return result;
    }

    public Object getValue() {
        if (defaultValue != null && value == null)
            value = defaultValue;
        return value;
    }

    public void setValue(Object value) {
        this.value = value;
        getEventBus().post(new ValueChangedEvent(this));
    }

    public boolean hasValue() {
        return getType() == JSONSchemaObjectType.jsonObject || getValue() != null;
    }

    @Override
    public MISAValidityReport getValidityReport() {
        MISAValidityReport report = new MISAValidityReport();
        if (hasValue())
            report.report(this, "Parameters", true, "");
        else
            report.report(this, "Parameters", false, "Value is not set");

        if (getProperties() != null) {
            for (JSONSchemaObject object : getProperties().values()) {
                report.merge(object.getValidityReport(), object.getId() == null ? "" : object.getId());
            }
        }

        return report;
    }

    public void setValueFromJson(JsonElement json, boolean createNewProperties) {
        if (json.isJsonNull())
            return;
        switch (getType()) {
            case jsonBoolean:
                setValue(json.getAsBoolean());
                break;
            case jsonNumber:
                setValue(json.getAsDouble());
                break;
            case jsonString:
                setValue(json.getAsString());
                break;
            case jsonArray:
                setValue(json.getAsJsonArray());
                break;
            case jsonObject:
                for (Map.Entry<String, JsonElement> kv : json.getAsJsonObject().entrySet()) {
                    if(!hasPropertyFromPath(kv.getKey()) && !createNewProperties)
                        continue;
                    ensurePropertyFromPath(kv.getKey()).setValueFromJson(kv.getValue(), createNewProperties);
                }
                break;
        }
    }

    public EventBus getEventBus() {
        return eventBus;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public JSONSchemaObject getParent() {
        return parent;
    }

    public void setParent(JSONSchemaObject parent) {
        this.parent = parent;
    }

    public JSONSchemaObjectType getType() {
        return type;
    }

//    public String getDescription() {
//        return description;
//    }
//
//    public void setDescription(String description) {
//        this.description = description;
//    }

    public Map<String, JSONSchemaObject> getProperties() {
        return properties;
    }

    public void setProperties(HashMap<String, JSONSchemaObject> properties) {
        this.properties = properties;
    }

    public JSONSchemaObject getAdditionalItems() {
        return additionalItems;
    }

    public void setAdditionalItems(JSONSchemaObject additionalItems) {
        this.additionalItems = additionalItems;
    }

    public Object getDefaultValue() {
        return defaultValue;
    }

    public void setDefaultValue(Object defaultValue) {
        this.defaultValue = defaultValue;
    }

    public List<Object> getEnumValues() {
        return enumValues;
    }

    public void setEnumValues(List<Object> enumValues) {
        this.enumValues = enumValues;
    }

    public List<String> getRequiredProperties() {
        return requiredProperties;
    }

    public void setRequiredProperties(List<String> requiredProperties) {
        this.requiredProperties = requiredProperties;
    }

    public JSONSchemaObject getAdditionalProperties() {
        return additionalProperties;
    }

    public void setAdditionalProperties(JSONSchemaObject additionalProperties) {
        this.additionalProperties = additionalProperties;
    }

    public String getSerializationId() {
        return serializationId;
    }

    public void setSerializationId(String serializationId) {
        this.serializationId = serializationId;
    }

    public List<String> getSerializationHierarchy() {
        return serializationHierarchy;
    }

    public void setSerializationHierarchy(List<String> serializationHierarchy) {
        this.serializationHierarchy = serializationHierarchy;
    }

    public String getDocumentationTitle() {
        if (documentationTitle != null && !documentationTitle.isEmpty())
            return documentationTitle;
        else
            return getId();
    }

    public void setDocumentationTitle(String documentationTitle) {
        this.documentationTitle = documentationTitle;
    }

    public boolean hasDocumentationDescription() {
        return this.documentationDescription != null && !this.documentationDescription.isEmpty();
    }

    public String getDocumentationDescription() {
        return documentationDescription;
    }

    public void setDocumentationDescription(String documentationDescription) {
        this.documentationDescription = documentationDescription;
    }

    /**
     * Returns true if the default value is different to the current value
     * For objects, the properties are compared
     * @return
     */
    public boolean wasChanged() {
        if(type == JSONSchemaObjectType.jsonObject) {
            for(JSONSchemaObject object : properties.values()) {
                if(object.wasChanged())
                    return true;
            }
            return false;
        }
        else {
            return getValue() == getDefaultValue() || Objects.equals(getValue(), getDefaultValue());
        }
    }

    /**
     * Automatically generates a color from the name
     *
     * @return
     */
    public Color toColor() {
        if (getId() != null) {
            float h = Math.abs(getId().hashCode() % 256) / 255.0f;
            return Color.getHSBColor(h, 0.8f, 0.8f);
        } else {
            float h = Math.abs(getName().hashCode() % 256) / 255.0f;
            return Color.getHSBColor(h, 0.8f, 0.8f);
        }
    }

    public String getTooltip() {
        StringBuilder tooltip = new StringBuilder();
        tooltip.append("<html>");
        if (this.hasDocumentationDescription()) {
            tooltip.append(this.getDocumentationDescription());
            tooltip.append("<br/><br/>");
            tooltip.append("<i>").append("(").append(this.getId()).append(")").append("</i>");
        } else {
            tooltip.append("(").append(this.getId()).append(")");
        }
        tooltip.append("</html>");
        return tooltip.toString();
    }

    public String getDocumentationTypeTitle() {
        return documentationTypeTitle;
    }

    public String getDocumentationTypeDescription() {
        return documentationTypeDescription;
    }

    public static class ValueChangedEvent {
        private JSONSchemaObject schemaObject;

        public ValueChangedEvent(JSONSchemaObject schemaObject) {
            this.schemaObject = schemaObject;
        }

        public JSONSchemaObject getSchemaObject() {
            return schemaObject;
        }
    }

    public static class AddedAdditionalPropertyEvent {
        private JSONSchemaObject property;

        public AddedAdditionalPropertyEvent(JSONSchemaObject property) {
            this.property = property;
        }

        public JSONSchemaObject getProperty() {
            return property;
        }
    }

    public static class RemovedAdditionalPropertyEvent {
        private JSONSchemaObject property;

        public RemovedAdditionalPropertyEvent(JSONSchemaObject property) {
            this.property = property;
        }

        public JSONSchemaObject getProperty() {
            return property;
        }
    }
}

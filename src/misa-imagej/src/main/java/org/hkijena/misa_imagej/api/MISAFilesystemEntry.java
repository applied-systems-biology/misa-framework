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

import com.google.gson.JsonObject;
import org.hkijena.misa_imagej.MISAImageJRegistryService;
import org.hkijena.misa_imagej.api.json.JSONSchemaObject;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

/**
 * A MISA++ filesystem entry
 */
public class MISAFilesystemEntry implements Cloneable {
    private String name;
    private Map<String, MISAFilesystemEntry> children = new HashMap<>();
    private MISAFilesystemEntry parent;
    private Path externalPath = null;
    private JSONSchemaObject metadata;
    private MISACacheIOType ioType;

    public MISAFilesystemEntry(MISAFilesystemEntry parent, String name, MISACacheIOType ioType) {
        this.parent = parent;
        this.name = name;
        this.ioType = ioType;
    }

    public MISAFilesystemEntry(MISAFilesystemEntry parent, JSONSchemaObject sourceObject, String name , MISACacheIOType ioType) {
        this(parent, name, ioType);

        if(sourceObject.hasPropertyFromPath("external-path")) {
            String pathString = sourceObject.getPropertyFromPath("external-path").getDefaultValue().toString();
            if(pathString.isEmpty()) {
                externalPath = null;
            }
            else {
                externalPath = Paths.get(pathString);
            }
        }
        if(sourceObject.hasPropertyFromPath("metadata")) {
            setMetadata((JSONSchemaObject) sourceObject.getPropertyFromPath("metadata").clone());
        }
        if(sourceObject.hasPropertyFromPath("children")) {
            for(Map.Entry<String, JSONSchemaObject> entry : sourceObject.getPropertyFromPath("children").getProperties().entrySet()) {
                children.put(entry.getKey(), new MISAFilesystemEntry(this, entry.getValue(), entry.getKey(), ioType));
            }
        }
    }

    /**
     * Clones this filesystem entry and its children.
     * Please note that the parent will be set to null. Children will be cloned as well and have their parent field set to the
     * result.
     * @return
     */
    @Override
    public Object clone() {
        MISAFilesystemEntry entry = new MISAFilesystemEntry(parent, name, ioType);
        entry.name = name;
        entry.externalPath = externalPath;
        entry.ioType = ioType;
        entry.parent = null;
        if(getMetadata() != null)
            entry.setMetadata((JSONSchemaObject) getMetadata().clone());
        for(Map.Entry<String, MISAFilesystemEntry> child : children.entrySet()) {
            MISAFilesystemEntry copy = (MISAFilesystemEntry)child.getValue().clone();
            copy.parent = entry;
            entry.children.put(child.getKey(), copy);
        }
        return entry;
    }

    public void findCaches(MISASample sample, List<MISACache> result) {
        MISACache cache = MISAImageJRegistryService.getInstance().getCacheRegistry().getCacheFor(sample,this);
        if(cache.isValid())
            result.add(cache);
        for(MISAFilesystemEntry entry : children.values()) {
            entry.findCaches(sample, result);
        }
    }

    public Path getInternalPath() {
        if(parent != null)
            return parent.getInternalPath().resolve(name);
        else
            return Paths.get(name);
    }

    public Path getExternalPath() {
        if(externalPath != null) {
            return externalPath;
        }
        else if(parent != null) {
            return parent.getExternalPath().resolve(name);
        }
        else {
            return null;
        }
    }

    public void setExternalPath(Path path) {
        externalPath = path;
    }

    /**
     * Sets the external path from a parameter JSON object that corresponds to this entry.
     * Will recursively descend
     * @param json
     */
    public void setExternalPathFromJson(JsonObject json) {
        if(json.has("external-path")) {
            externalPath = Paths.get(json.get("external-path").getAsString());
        }
        for(Map.Entry<String, MISAFilesystemEntry> kv : children.entrySet()) {
            if(json.has("children") && json.getAsJsonObject("children").has(kv.getKey())) {
                kv.getValue().setExternalPathFromJson(json.getAsJsonObject("children").getAsJsonObject(kv.getKey()));
            }
        }
    }

    /**
     * Returns true if there are external path definitions
     * @return
     */
    public boolean hasExternalPathDefinitions() {
        Stack<MISAFilesystemEntry> stack = new Stack<>();
        stack.push(this);

        while(!stack.isEmpty()) {
            MISAFilesystemEntry top = stack.pop();
            if(top.externalPath != null)
                return true;
            for(MISAFilesystemEntry entry : top.children.values()) {
                stack.push(entry);
            }
        }
        return false;
    }

    /**
     * Converts this filesystem enty into a proper MISA++ JSON entry
     * @return
     */
    public JsonObject toJson() {
        JsonObject object = new JsonObject();
        if(externalPath != null) {
            object.addProperty("external-path", getExternalPath().toString());
        }
        if(getMetadata() != null && getMetadata().wasChanged()) {
            object.add("metadata", getMetadata().toJson());
        }
        if(children != null) {
            JsonObject childrenJson = new JsonObject();
            for(Map.Entry<String, MISAFilesystemEntry> kv : children.entrySet()) {
                childrenJson.add(kv.getKey(), kv.getValue().toJson());
            }
            object.add("children", childrenJson);
        }
        return object;
    }

    public MISAFilesystemEntry resolve(String ...segments) {
        MISAFilesystemEntry current = this;
        for(String segment : segments) {
            if(!current.children.containsKey(segment)) {
                MISAFilesystemEntry entry = new MISAFilesystemEntry(this, segment, ioType);
                current.children.put(segment, entry);
            }
            current = current.children.get(segment);
        }
        return current;
    }

    @Override
    public String toString() {
        return getInternalPath().toString();
    }

    public void update() {
        for(Map.Entry<String, MISAFilesystemEntry> child : children.entrySet()) {
            child.getValue().name = child.getKey();
            child.getValue().update();
        }
    }

    public JSONSchemaObject getMetadata() {
        return metadata;
    }

    public void setMetadata(JSONSchemaObject metadata) {
        this.metadata = metadata;
    }

    public MISACacheIOType getIoType() {
        return ioType;
    }

    public String getName() {
        return name;
    }

    public void setParent(MISAFilesystemEntry parent) {
        this.parent = parent;
        if(parent != null) {
            parent.update();
        }
    }

    public MISAFilesystemEntry getParent() {
        return parent;
    }

    public void insert(String name, MISAFilesystemEntry importedFilesystem) {
        this.children.put(name, importedFilesystem);
        update();
    }

    public boolean remove(String name) {
        if(this.children.containsKey(name)) {
            this.children.remove(name);
            return true;
        }
        return false;
    }
}

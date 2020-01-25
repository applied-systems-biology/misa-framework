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

package org.hkijena.misa_imagej.api.repository;

import com.google.gson.annotations.SerializedName;

import java.awt.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class MISAModuleInfo {
    @SerializedName("id")
    private String id;

    @SerializedName("version")
    private String version;

    @SerializedName("name")
    private String name;

    @SerializedName("description")
    private String description;

    @SerializedName("dependencies")
    private List<MISAModuleInfo> dependencies = new ArrayList<>();

    @Override
    public String toString() {
        if(getName() == null || getName().isEmpty()) {
            return getId() + "-" + getVersion();
        }
        else {
            return getName() + " (" + getId() + "-" + getVersion() + ")";
        }
    }

    public String getId() {
        return id;
    }

    public String getVersion() {
        return version;
    }

    public String getName() {
        if(name == null || name.isEmpty())
            return getId();
        else
            return name;
    }

    /**
     * Automatically generates a color from the name
     * @return
     */
    public Color toColor() {
        float h = Math.abs(getId().hashCode() % 256) / 255.0f;
        return Color.getHSBColor(h, 0.8f, 0.8f);
    }

    public String getDescription() {
        return description;
    }

    public List<MISAModuleInfo> getDependencies() {
        return Collections.unmodifiableList(dependencies);
    }
}

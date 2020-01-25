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

package org.hkijena.misa_imagej.api.pipelining;

import com.google.common.eventbus.EventBus;
import com.google.gson.annotations.SerializedName;
import org.hkijena.misa_imagej.api.MISAModuleInstance;
import org.hkijena.misa_imagej.api.MISAValidatable;
import org.hkijena.misa_imagej.api.MISAValidityReport;
import org.hkijena.misa_imagej.api.repository.MISAModule;
import org.hkijena.misa_imagej.api.repository.MISAModuleRepository;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

public class MISAPipelineNode implements MISAValidatable {

    @SerializedName("id")
    private String id;

    @SerializedName("name")
    private String name;

    @SerializedName("description")
    private String description;

    @SerializedName("module-name")
    private String moduleName;

    @SerializedName("x")
    private int x;

    @SerializedName("y")
    private int y;

    private transient MISAPipeline pipeline;

    private transient MISAModuleInstance moduleInstance;

    private transient EventBus eventBus = new EventBus();

    public MISAPipelineNode() {

    }

    public MISAPipelineNode(MISAPipeline pipeline) {
        this.setPipeline(pipeline);
    }

    /**
     * Finds all nodes that can be used as input for this node
     * @return
     */
    public Collection<MISAPipelineNode> getAvailableInNodes() {
        List<MISAPipelineNode> result = new ArrayList<>();
        for(MISAPipelineNode source : getPipeline().getNodes()) {
            if(getPipeline().canAddEdge(source, this)) {
                result.add(source);
            }
        }
        return result;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
        this.getModuleInstance().setName(name);
        eventBus.post(new ChangedNameEvent(this));
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
        eventBus.post(new ChangedDescriptionEvent(this));
    }

    public String getModuleName() {
        return moduleName;
    }

    public void setModuleName(String moduleName) {
        this.moduleName = moduleName;
    }

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
        eventBus.post(new ChangedPositionEvent(this));
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
        eventBus.post(new ChangedPositionEvent(this));
    }

    public MISAPipeline getPipeline() {
        return pipeline;
    }

    public MISAModuleInstance getModuleInstance() {
        if(moduleInstance == null) {
            // Instantiate the module
            MISAModule module = MISAModuleRepository.getInstance().getModule(moduleName);
            moduleInstance = module.instantiate();
        }
        return moduleInstance;
    }

    public void setPipeline(MISAPipeline pipeline) {
        this.pipeline = pipeline;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
        eventBus.post(new ChangedIdEvent(this));
    }

    @Override
    public MISAValidityReport getValidityReport() {
        MISAValidityReport report = new MISAValidityReport();
        report.merge(getModuleInstance().getValidityReport());
        return report;
    }

    public EventBus getEventBus() {
        return eventBus;
    }

    public static class ChangedNameEvent {
        private MISAPipelineNode node;

        public ChangedNameEvent(MISAPipelineNode node) {
            this.node = node;
        }

        public MISAPipelineNode getNode() {
            return node;
        }
    }

    public static class ChangedDescriptionEvent {
        private MISAPipelineNode node;

        public ChangedDescriptionEvent(MISAPipelineNode node) {
            this.node = node;
        }

        public MISAPipelineNode getNode() {
            return node;
        }
    }

    public static class ChangedIdEvent {
        private MISAPipelineNode node;

        public ChangedIdEvent(MISAPipelineNode node) {
            this.node = node;
        }

        public MISAPipelineNode getNode() {
            return node;
        }
    }

    public static class ChangedPositionEvent {
        private MISAPipelineNode node;

        public ChangedPositionEvent(MISAPipelineNode node) {
            this.node = node;
        }

        public MISAPipelineNode getNode() {
            return node;
        }
    }
}

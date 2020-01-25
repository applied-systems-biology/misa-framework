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
import org.hkijena.misa_imagej.extension.datasources.MISAFolderLinkDataSource;

import java.awt.*;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class MISACache implements MISAValidatable {

    private MISASample sample;

    /**
     * Relative path within the imported or exported filesystem
     * This does not include "imported" or "exported"
     */
    private MISAFilesystemEntry filesystemEntry;

    private MISADataSource dataSource;

    private List<MISADataSource> availableDatasources = new ArrayList<>();

    private EventBus eventBus = new EventBus();

    private String documentationTitle;

    private String documentationDescription;


    public MISACache(MISASample sample, MISAFilesystemEntry filesystemEntry) {
        this.sample = sample;
        this.filesystemEntry = filesystemEntry;

        if(filesystemEntry != null && filesystemEntry.getMetadata() != null) {
            this.documentationTitle = filesystemEntry.getMetadata().getDocumentationTitle();
            this.documentationDescription = filesystemEntry.getMetadata().getDocumentationDescription();
        }

        // Default data sources that are always available
        this.availableDatasources.add(new MISAFolderLinkDataSource(this));
    }

    /**
     * Returns the filesystem entry this cache is attached to
     * @return
     */
    public MISAFilesystemEntry getFilesystemEntry() {
        return filesystemEntry;
    }

    /**
     * Gets tooltip for the cache data type
     * @return
     */
    public String getCacheTooltip() {
        StringBuilder docString = new StringBuilder();
        docString.append("<html>");
        if(getCacheTypeName() != null && !getCacheTypeName().isEmpty()) {
            docString.append("<b>");
            docString.append(getCacheTypeName());
            docString.append("</b>");
            docString.append("<br/><br/>");
        }
        docString.append(getCacheDocumentation());
        docString.append("</html>");
        return docString.toString();
    }

    /**
     * Gets tooltip for the cache data type
     * @return
     */
    public String getPatternTooltip() {
        StringBuilder docString = new StringBuilder();
        docString.append("<html>");
        if(getCachePatternTypeName() != null &&
                !getCachePatternTypeName().isEmpty()) {
            docString.append(getCachePatternTypeName());
            docString.append("<br/><br/>");
        }
        docString.append(getCachePatternDocumentation());
        docString.append("</html>");
        return docString.toString();
    }

    /**
     * Gets a tooltip for this cache
     * @return
     */
    public String getTooltip() {
        StringBuilder result = new StringBuilder();
        result.append("<html>");
        if(documentationTitle != null && !documentationTitle.isEmpty())
            result.append("<b>").append(documentationTitle).append("</b>");
        if(documentationDescription != null && !documentationDescription.isEmpty()) {
            if(documentationTitle != null && !documentationTitle.isEmpty())
                result.append("<br/><br/>");
            result.append(documentationDescription);
        }
        result.append("</html>");
        return result.toString();
    }

    /**
     * Returns a non-empty string that describes the internal path within the filesystem
     * @return
     */
    public String getRelativePathName() {
        if(getFilesystemEntry().getInternalPath().toString().isEmpty())
            return "<Root>";
        else
            return getFilesystemEntry().getInternalPath().toString();
    }

    /**
     * Returns a string that describes the internal path within the filesystem
     * @return
     */
    public String getRelativePath() {
        return getFilesystemEntry().getInternalPath().toString().replace('\\', '/');
    }

    /**
     * Relative path including the IO mode
     * @return
     */
    public String getFullRelativePath() {
        if(getIOType() == MISACacheIOType.Imported)
            return "imported/" + getRelativePath();
        else if(getIOType() == MISACacheIOType.Exported)
            return "exported/" + getRelativePath();
        else
            throw new UnsupportedOperationException();
    }

    /**
     * Returns the serialization ID of the pattern if available
     * Otherwise return null
     * @return
     */
    public String getPatternSerializationID() {
        if(getFilesystemEntry() != null && getFilesystemEntry().getMetadata() != null) {
            if(getFilesystemEntry().getMetadata().hasPropertyFromPath("pattern")) {
                return getFilesystemEntry().getMetadata().getPropertyFromPath("pattern").getSerializationId();
            }
        }
        return null;
    }

    /**
     * Returns the serialization ID of the description if available
     * Otherwise return null
     * @return
     */
    public String getDescriptionSerializationID() {
        if(getFilesystemEntry() != null && getFilesystemEntry().getMetadata() != null) {
            if(getFilesystemEntry().getMetadata().hasPropertyFromPath("description")) {
                return getFilesystemEntry().getMetadata().getPropertyFromPath("description").getSerializationId();
            }
        }
        return null;
    }

    /**
     * Returns the IO type of this cache
     * @return
     */
    public MISACacheIOType getIOType() {
        return getFilesystemEntry().getIoType();
    }

    /**
     * Returns true if this cache has a pattern or description
     * @return
     */
    public boolean isValid() {
        return getPatternSerializationID() != null || getDescriptionSerializationID() != null;
    }

    /**
     * Returns the name of this cache
     * @return
     */
    public String getCacheTypeName() {
        if(getFilesystemEntry().getMetadata().hasPropertyFromPath("description")) {
            String name = getFilesystemEntry().getMetadata().getPropertyFromPath("description").getDocumentationTitle();
            if(name != null && !name.isEmpty())
                return name;
        }
        return getPatternSerializationID() + " -> " + getDescriptionSerializationID();
    }

    /**
     * Returns the documentation for this cache
     * @return
     */
    public String getCacheDocumentation() {
        if(getFilesystemEntry().getMetadata().hasPropertyFromPath("description")) {
           return getFilesystemEntry().getMetadata().getPropertyFromPath("description").getDocumentationDescription();
        }
        return null;
    }

    /**
     * Returns the documentation for the pattern of this cache
     * @return
     */
    public String getCachePatternTypeName() {
        if(getFilesystemEntry().getMetadata().hasPropertyFromPath("pattern")) {
            return getFilesystemEntry().getMetadata().getPropertyFromPath("pattern").getDocumentationTitle();
        }
        return null;
    }

    /**
     * Returns the documentation for the pattern of this cache
     * @return
     */
    public String getCachePatternDocumentation() {
        if(getFilesystemEntry().getMetadata().hasPropertyFromPath("pattern")) {
            return getFilesystemEntry().getMetadata().getPropertyFromPath("pattern").getDocumentationDescription();
        }
        return null;
    }

    @Override
    public String toString() {
        return getPatternSerializationID() + "|" + getDescriptionSerializationID() + " @ " + getFilesystemEntry().toString();
    }

    /**
     * Automatically generates a color from the name
     * @return
     */
    public Color toColor() {
        float h = Math.abs(getCacheTypeName().hashCode() % 256) / 255.0f;
        return Color.getHSBColor(h, 0.5f, 1);
    }

    @Override
    public MISAValidityReport getValidityReport() {
        if(getIOType() == MISACacheIOType.Exported)
            return new MISAValidityReport(this, null, true, "");
        else if(getDataSource() == null) {
            return new MISAValidityReport(this, "Data " + getCacheTypeName() + " " + getRelativePathName(), false, "No data source was set!");
        }
        else {
            return getDataSource().getValidityReport();
        }
    }

    /**
     * Installs this cache into the install folder
     * this is only valid for imported caches
     * @param installFolder
     * @param forceCopy forces copying all files into the install folder
     */
    public void install(Path installFolder, boolean forceCopy) {
        if(getIOType() == MISACacheIOType.Imported) {
            getDataSource().install(installFolder, forceCopy);
        }
    }

    /**
     * Gets a data source by type
     * @param klass
     * @param <T>
     * @return
     */
    public <T extends MISADataSource> T getDataSourceByType(Class<T> klass) {
        return (T)getAvailableDataSources().stream().filter(klass::isInstance).findFirst().get();
    }

    /**
     * Gets all data sources that have the speciefied type
     * @param klass
     * @param <T>
     * @return
     */
    public <T extends MISADataSource> List<T> getDataSourcesByType(Class<T> klass) {
        List<T> result = new ArrayList<>();
        getAvailableDataSources().stream().filter(klass::isInstance).forEach(misaDataSource -> {
            result.add((T)misaDataSource);
        });
        return result;
    }

    /**
     * Returns a list of additional data sources that are available for this cache
     * @return
     */
    public List<MISADataSource> getAvailableDataSources() {
        return Collections.unmodifiableList(availableDatasources);
    }

    /**
     * Adds a new data source to the list of available ones
     * @param source
     */
    public void addAvailableDataSource(MISADataSource source) {
        availableDatasources.add(source);
    }

    /**
     * Removes a data source from the list of available sources
     * @param source
     */
    public void removeAvailableDataSource(MISADataSource source) {
        availableDatasources.remove(source);
        if(source == dataSource) {
            if(getPreferredDataSource() != null)
                setDataSource(getPreferredDataSource());
            else if(availableDatasources.size() > 0)
                setDataSource(availableDatasources.get(0));
            else
                setDataSource(null);
        }
    }

    /**
     * Returns a preferred data source or null
     * @return
     */
    public MISADataSource getPreferredDataSource() {
        return null;
    }

    /**
     * Returns the sample that this cache belongs to
     * @return
     */
    public MISASample getSample() {
        return sample;
    }

    public void setDataSource(MISADataSource dataSource) {
        this.dataSource = dataSource;
        getEventBus().post(new DataSourceChangeEvent(this, this.dataSource));
    }

    /**
     * A data source is responsible for providing the data of the cache
     */
    public MISADataSource getDataSource() {
        return dataSource;
    }

    public EventBus getEventBus() {
        return eventBus;
    }

    public String getDocumentationTitle() {
        return documentationTitle;
    }

    public String getDocumentationDescription() {
        return documentationDescription;
    }

    public static class DataSourceChangeEvent {
        private MISACache cache;
        private MISADataSource dataSource;

        public DataSourceChangeEvent(MISACache cache, MISADataSource dataSource) {
            this.cache = cache;
            this.dataSource = dataSource;
        }

        public MISACache getCache() {
            return cache;
        }

        public MISADataSource getDataSource() {
            return dataSource;
        }
    }
}

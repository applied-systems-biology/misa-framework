/**
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


#pragma once

#include <string>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <misaxx/core/misa_description_storage.h>
#include <boost/algorithm/string.hpp>

namespace misaxx {

    struct misa_filesystem_entry;

    namespace filesystem {
        using entry = std::shared_ptr<misa_filesystem_entry>;
        using const_entry = std::shared_ptr<const misa_filesystem_entry>;
    }

    /**
     * Available modes of entry types
     */
    enum class misa_filesystem_entry_type {
        unknown,
        imported,
        exported
    };

    /**
     * An entry in the MISA++ virtual filesystem
     */
    struct misa_filesystem_entry : std::enable_shared_from_this<misa_filesystem_entry>, public misa_serializable {

        using path = boost::filesystem::path;
        using iterator = std::unordered_map<std::string, std::shared_ptr<misa_filesystem_entry>>::iterator;
        using const_iterator = std::unordered_map<std::string, std::shared_ptr<misa_filesystem_entry>>::const_iterator;

        /**
         * Parent node of the current VFS entry
         */
        std::weak_ptr<misa_filesystem_entry> parent;

        /**
       * Internal name of the filesystem entry
       */
        std::string name;

        /**
         * Type of this entry
         */
        misa_filesystem_entry_type type;

        /**
         * Custom external path of this entry.
         * If not set, the path will be loaded from the parent.
         */
        path custom_external;

        /**
         * Contains the description and/or the pattern that can be attached to the filesystem
         */
        std::shared_ptr<misa_description_storage> metadata = std::make_shared<misa_description_storage>();

        explicit misa_filesystem_entry(std::string t_name, misa_filesystem_entry_type t_type, path t_custom_external = path());

        misa_filesystem_entry(const misa_filesystem_entry &value) = delete;

        misa_filesystem_entry(misa_filesystem_entry &&value) = default;

        /**
         * Returns the internal path inside this filesystem
         * @return
         */
        path internal_path() const;

        /**
         * Returns true if this entry is associated to an external path
         * @return
         */
        bool has_external_path() const;

        /**
         * Returns the external path of this node.
         * @return
         */
        path external_path() const;

        /**
         * Returns a managed pointer to this entry
         * @return
         */
        filesystem::entry self();

        /**
         * Returns a managed pointer to this entry
         * @return
         */
        filesystem::const_entry self() const;

        /**
         * Creates a child entry with given name and external path
         * @param t_name
         * @param t_custom_external
         * @return
         */
        filesystem::entry create(std::string t_name, path t_custom_external = path());

        /**
         * Inserts a child into this entry
         * @param ptr
         * @return
         */
        filesystem::entry insert(filesystem::entry ptr);

        /**
         * Removes an entry with given name
         * @param name
         * @return
         */
        bool remove(const std::string &t_name);

        iterator begin();

        const_iterator begin() const;

        iterator end();

        const_iterator end() const;

        iterator find(const std::string &t_name);

        const_iterator find(const std::string &t_name) const;

        /**
         * Returns true if the entry has no children
         * @return
         */
        bool empty() const;

        /**
         * Ensures that the external folder path exists if it is set.
         * Throws an exception if there is no external path.
         */
        void ensure_external_path_exists() const;

        /**
         * Accesses (which includes creating an entry if necessary)
         * @tparam As
         * @param t_segment
         * @return
         */
        filesystem::entry resolve(boost::filesystem::path t_segment);

        /**
         * Accesses (which includes creating an entry if necessary)
         * @tparam As
         * @param t_segment
         * @return
         */
        filesystem::const_entry at(boost::filesystem::path t_segment) const;

        /**
         * Returns true if this filesystem has given subpath
         * @param t_segment
         * @return
         */
        bool has_subpath(boost::filesystem::path t_segment) const;

        void from_json(const nlohmann::json &t_json) override;

        void to_json(nlohmann::json &t_json) const override;

        void to_json_schema(misa_json_schema_property &t_schema) const override;

        /**
         * Finds a filesystem entry (including children) where its external path is either a parent of the input path
         * or is equal to it. Returns nullptr if the path is not located within the filesystem.
         * @param t_path
         * @return
         */
        std::shared_ptr<misa_filesystem_entry> find_external_path(const boost::filesystem::path &t_path);

        /**
         * Returns the depth of this entry
         * @return
         */
        size_t get_depth() const;

        /**
         * Traverses the entry
         * @return
         */
        std::vector<filesystem::entry> traverse();

    protected:

        void build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const override;

    private:

        std::unordered_map<std::string, std::shared_ptr<misa_filesystem_entry>> children;
    };
}

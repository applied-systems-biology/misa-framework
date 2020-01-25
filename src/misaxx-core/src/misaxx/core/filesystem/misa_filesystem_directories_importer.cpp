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

#include <misaxx/core/filesystem/misa_filesystem_directories_importer.h>

using namespace misaxx;

misa_filesystem misa_filesystem_directories_importer::import() {
    misa_filesystem vfs;
    vfs.imported = std::make_shared<misa_filesystem_entry>("imported", misa_filesystem_entry_type ::imported, input_path);
    discoverImporterEntry(vfs.imported);
    vfs.exported = std::make_shared<misa_filesystem_entry>("exported", misa_filesystem_entry_type ::exported, output_path);
    return vfs;
}

void misa_filesystem_directories_importer::discoverImporterEntry(const filesystem::entry &t_entry) {
    std::cout << "[Filesystem][directories-importer] Importing entry " << t_entry->internal_path().string() << " @ " << t_entry->external_path().string() << "\n";
    auto metadata_file = t_entry->external_path() / "misa-data.json";
    if(boost::filesystem::is_regular_file(metadata_file)) {
        std::cout << "[Filesystem][directories-importer] Importing metadata from file " << metadata_file.string() << "\n";
        nlohmann::json json;
        std::ifstream stream;
        stream.open(metadata_file.string());
        stream >> json;
        t_entry->metadata->from_json(json);
    }

    for(const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_entry->external_path()))) {
        if(boost::filesystem::is_directory(entry)) {
            filesystem::entry e = t_entry->create(entry.path().filename().string());
            discoverImporterEntry(e);
        }
    }
}

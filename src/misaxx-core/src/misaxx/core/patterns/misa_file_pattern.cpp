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

#include <misaxx/core/patterns/misa_file_pattern.h>

using namespace misaxx;

misa_file_pattern::misa_file_pattern(std::vector<boost::filesystem::path> t_extensions) :
        extensions(std::move(t_extensions)) {

}

void misa_file_pattern::from_json(const nlohmann::json &t_json) {
    if (t_json.find("filename") != t_json.end())
        filename = t_json["filename"].get<std::string>();
    if (t_json.find("extensions") != t_json.end()) {
        extensions.clear();
        for (const auto &i : t_json["extensions"]) {
            extensions.emplace_back(i.get<std::string>());
        }
    }
}

void misa_file_pattern::to_json(nlohmann::json &t_json) const {
    misa_data_pattern::to_json(t_json);
    t_json["filename"] = filename.string();
    {
        std::vector<std::string> extensions_;
        for (const auto &extension : extensions) {
            extensions_.emplace_back(extension.string());
        }
        t_json["extensions"] = extensions_;
    }
}

void misa_file_pattern::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_data_pattern::to_json_schema(t_schema);
    t_schema["filename"].declare<std::string>()
            .make_optional(filename.string())
            .document_title("Filename")
            .document_description("Predefined filename");
    std::vector<std::string> extensions_;
    for (const auto &extension : extensions) {
        extensions_.emplace_back(extension.string());
    }
    t_schema["extensions"].declare<std::vector<std::string>>()
            .make_optional(extensions_)
            .document_title("Extensions")
            .document_description("List of extensions (including dot) that this pattern will match");
}

bool misa_file_pattern::has_filename() const {
    return !filename.empty();
}

bool misa_file_pattern::has_extensions() const {
    return !extensions.empty();
}

bool misa_file_pattern::matches(const boost::filesystem::path &t_path) const {
    if (has_filename() && t_path.filename() == filename)
        return true;
    if (extensions.empty())
        return true;
    for (const auto &extension : extensions) {
        if (t_path.extension() == extension)
            return true;
    }
    return false;
}

void misa_file_pattern::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_data_pattern::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/file"));
}

void misa_file_pattern::apply(misa_file_description &target) const {
    target.filename = filename;
}

void misa_file_pattern::apply(misa_file_description &target, const boost::filesystem::path &t_directory) const {
    if (has_filename()) {
        target.filename = filename;
    } else {
        for (const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_directory))) {
            if (matches(entry.path())) {
                target.filename = entry.path().filename();
                break;
            }
        }
    }
}

std::string misa_file_pattern::get_documentation_name() const {
    return "File pattern";
}

std::string misa_file_pattern::get_documentation_description() const {
    return "Pattern that looks for a file with a specified extension";
}

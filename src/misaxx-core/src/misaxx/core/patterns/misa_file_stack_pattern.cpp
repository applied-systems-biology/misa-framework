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

#include <misaxx/core/patterns/misa_file_stack_pattern.h>

using namespace misaxx;

misa_file_stack_pattern::misa_file_stack_pattern(std::vector<boost::filesystem::path> t_extensions) : extensions(
        std::move(t_extensions)) {

}

void misa_file_stack_pattern::from_json(const nlohmann::json &t_json) {
    extensions.clear();
    for (const auto &i : t_json["extensions"]) {
        extensions.emplace_back(i.get<std::string>());
    }
}

void misa_file_stack_pattern::to_json(nlohmann::json &t_json) const {
    misa_data_pattern::to_json(t_json);
    std::vector<std::string> extensions_;
    for (const auto &extension : extensions) {
        extensions_.emplace_back(extension.string());
    }
    t_json["extensions"] = extensions_;
}

void misa_file_stack_pattern::to_json_schema(misa_json_schema_property &t_schema) const {
    misa_data_pattern::to_json_schema(t_schema);
    std::vector<std::string> extensions_;
    for (const auto &extension : extensions) {
        extensions_.emplace_back(extension.string());
    }
    t_schema["extensions"].declare<std::vector<std::string>>().make_optional(extensions_)
            .document_title("Extensions")
            .document_description("List of extensions (including dot) that this pattern will match");
}

void misa_file_stack_pattern::build_serialization_id_hierarchy(std::vector<misa_serialization_id> &result) const {
    misa_data_pattern::build_serialization_id_hierarchy(result);
    result.emplace_back(misa_serialization_id("misa", "patterns/file-stack"));
}

void
misa_file_stack_pattern::apply(misa_file_stack_description &target, const boost::filesystem::path &t_directory) const {
    for (const auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(t_directory))) {
        if (extensions.empty()) {
            target.files.insert({entry.path().filename().string(), misa_file_description(entry.path())});
        } else {
            for (const auto &extension : extensions) {
                if (boost::iequals(entry.path().extension().string(), extension.string())) {
                    target.files.insert({entry.path().filename().string(), misa_file_description(entry.path())});
                    break;
                }
            }
        }
    }
}

std::string misa_file_stack_pattern::get_documentation_name() const {
    return "File stack pattern";
}

std::string misa_file_stack_pattern::get_documentation_description() const {
    return "A pattern that looks for a list of files with a specific extension";
}

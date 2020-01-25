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

#include <misaxx/imaging/caches/misa_image_stack_cache.h>

void
misaxx::imaging::misa_image_stack_cache::do_link(const misaxx::imaging::misa_image_stack_description &t_description) {
    auto &files = this->get();
    for(const auto &kv : t_description.files) {
        misa_image_description description;
        description.filename = kv.second.filename;
        misa_image_file cache;
        cache.suggest_link(this->get_internal_location(), this->get_location(), misaxx::misa_description_storage::with(std::move(description))); // We link manually with the loaded description
        files.insert({ kv.first, cache });
    }

    this->set_unique_location(this->get_location());
}

misaxx::imaging::misa_image_stack_description
misaxx::imaging::misa_image_stack_cache::produce_description(const boost::filesystem::path &t_location,
                                                             const misaxx::imaging::misa_image_stack_pattern &t_pattern) {
    misa_image_stack_description result;
    t_pattern.apply(result, t_location);
    return result;
}

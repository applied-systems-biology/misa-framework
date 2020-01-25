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

#include <misaxx/imaging/caches/misa_image_stack_cache.h>
#include <misaxx/core/misa_cached_data.h>
#include <misaxx/core/misa_default_description_accessors.h>

namespace misaxx::imaging {

    /**
     * A simple stack of images
     */
    struct misa_image_stack : public misaxx::misa_cached_data<misa_image_stack_cache>,
                              public misaxx::misa_description_accessors_from_cache<misa_image_stack_cache, misa_image_stack> {

        using iterator = typename misa_image_stack_t::iterator;
        using const_iterator = typename misa_image_stack_t::const_iterator;

        iterator begin() {
            return this->data->get().begin();
        }

        iterator end() {
            return this->data->get().end();
        }

        const_iterator begin() const {
            return this->data->get().begin();
        }

        const_iterator end() const {
            return this->data->get().end();
        }

        const_iterator find(const std::string &t_name) const {
            return this->data->get().find(t_name);
        }

        misa_image_file at(const std::string &t_name) const {
            return this->data->get().at(t_name);
        }

        size_t size() const {
            return this->data->get().size();
        }

        std::vector<std::string> get_filenames() const {
            std::vector<std::string> result;
            result.reserve(size());
            for(const auto &kv : *this) {
                result.push_back(kv.first);
            }
            return result;
        }

        /**
         * Returns true if there are no images in this stack
         * @return
         */
        bool empty() {
            return this->access_readonly().get().empty();
        }
    };
}





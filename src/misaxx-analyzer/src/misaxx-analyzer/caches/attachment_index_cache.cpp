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

#include "misaxx-analyzer/caches/attachment_index_cache.h"
#include "src/misaxx-analyzer/utils/sqlite_orm.h"

using namespace misaxx_analyzer;

namespace {

    inline auto create_storage(const std::string &path) {
        using namespace sqlite_orm;

        return make_storage(path, make_table("attachments",
                                             make_column("id", &attachment_index_row::id, autoincrement(), primary_key()),
                                             make_column("sample", &attachment_index_row::sample),
                                             make_column("cache", &attachment_index_row::cache),
                                             make_column("property", &attachment_index_row::property),
                                             make_column("serialization-id", &attachment_index_row::serialization_id),
                                             make_column("json-data", &attachment_index_row::json_data)
        ));
    }

    struct attachment_index_database_impl : public attachment_index_database {

        using storage_t = decltype(create_storage(""));

        storage_t database;

        explicit attachment_index_database_impl(const std::string &path) : database(create_storage(path)) {
            database.sync_schema();
            database.begin_transaction();
        }

        ~attachment_index_database_impl() {
            database.commit();
        }

        int insert(const attachment_index_row &row) override {
            return database.insert(row);
        }

    };

}

void misaxx_analyzer::attachment_index_cache::do_link(const misaxx_analyzer::attachment_index_description &t_description) {
    set_unique_location(get_location() / t_description.filename);
}

attachment_index_database &misaxx_analyzer::attachment_index_cache::get() {
    return *m_database;
}

const attachment_index_database &misaxx_analyzer::attachment_index_cache::get() const {
    return *m_database;
}

void misaxx_analyzer::attachment_index_cache::set(attachment_index_database value) {
    throw std::runtime_error("Setting the database externally is not allowed");
}

bool misaxx_analyzer::attachment_index_cache::has() const {
    return true;
}

bool misaxx_analyzer::attachment_index_cache::can_pull() const {
    return false;
}

void misaxx_analyzer::attachment_index_cache::pull() {
    m_database = std::make_shared<attachment_index_database_impl>(get_unique_location().string());
}

void misaxx_analyzer::attachment_index_cache::stash() {
    m_database.reset();
}

void misaxx_analyzer::attachment_index_cache::push() {
    // Database will do this on its own
}

misaxx_analyzer::attachment_index_description
misaxx_analyzer::attachment_index_cache::produce_description(const boost::filesystem::path &t_location,
                                                            const misaxx_analyzer::attachment_index_pattern &t_pattern) {
    attachment_index_description result;
    t_pattern.apply(result);
    return result;
}

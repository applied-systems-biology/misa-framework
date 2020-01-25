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

#include "misaxx/core/runtime/misa_runtime_log.h"
#include <chrono>
#include <misaxx/core/misa_json_schema_property.h>

void misaxx::misa_runtime_log::start(int thread, std::string name) {
    std::lock_guard<std::mutex> lock {mutex};
    entry e {*this};
    e.start_time = clock::now();
    e.end_time = clock::now();
    e.name = std::move(name);
    e.thread = thread;
    entries[thread].emplace_back(std::move(e));
}

void misaxx::misa_runtime_log::stop(int thread) {
    std::lock_guard<std::mutex> lock {mutex};
    std::vector<entry> &list = entries.at(thread);
    if(!list.empty()) {
        list.back().end_time = clock::now();
    }
}

void misaxx::misa_runtime_log::from_json(const nlohmann::json &) {
    throw std::runtime_error("Runtime logs cannot be loaded!");
}

void misaxx::misa_runtime_log::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
    for(const auto &kv : entries) {
        t_json["entries"]["thread" + std::to_string(kv.first)] = kv.second;
    }
}

void misaxx::misa_runtime_log::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
    t_schema.resolve("entries")->declare_required<std::vector<entry>>();
}

void
misaxx::misa_runtime_log::build_serialization_id_hierarchy(std::vector<misaxx::misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misaxx", "runtime-log"));
}

misaxx::misa_runtime_log::entry::entry(const misaxx::misa_runtime_log &log) : m_log(&log) {

}

void misaxx::misa_runtime_log::entry::from_json(const nlohmann::json &) {
    throw std::runtime_error("Runtime logs cannot be loaded!");
}

void misaxx::misa_runtime_log::entry::to_json(nlohmann::json &t_json) const {
    misa_serializable::to_json(t_json);
    t_json["start-time"] = std::chrono::duration_cast<duration>(start_time - m_log->start_time).count();
    t_json["end-time"] = std::chrono::duration_cast<duration>(end_time - m_log->start_time).count();
    t_json["unit"] = "ms";
    t_json["name"] = name;
}

void misaxx::misa_runtime_log::entry::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_serializable::to_json_schema(t_schema);
    t_schema.resolve("start-time")->declare_required<double>();
    t_schema.resolve("end-time")->declare_required<double>();
    t_schema.resolve("unit")->declare_required<std::string>();
    t_schema.resolve("name")->declare_required<std::string>();
}

void misaxx::misa_runtime_log::entry::build_serialization_id_hierarchy(
        std::vector<misaxx::misa_serialization_id> &result) const {
    misa_serializable::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misaxx", "runtime-log/entry"));
}



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

#include <misaxx/core/module_info.h>
#include <misaxx-analyzer/module_info.h>

misaxx::misa_module_info misaxx_analyzer::module_info() {
    misaxx::misa_module_info info;
    info.set_id("misaxx-analyzer");
    info.set_version("1.0.0.2");
    info.set_name("MISA++ Result Analyzer");
    info.set_description("Companion module for MISA-ImageJ that applies performance-critical tasks");
    info.add_author("Ruman Gerst");
    info.set_license("BSD-2-Clause");
    info.set_organization("Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Institute (HKI), Jena, Germany");
    info.set_url("https://applied-systems-biology.github.io/misa-framework/");

    // External dependency: OME Files
    misaxx::misa_module_info sqlite_info;
    sqlite_info.set_id("sqlite3");
    sqlite_info.set_name("SQLite");
    sqlite_info.set_version("3");
    sqlite_info.set_url("https://www.sqlite.org/");
    sqlite_info.set_organization("SQLite Consortium");
    sqlite_info.set_license("Public Domain");
    sqlite_info.set_is_external(true);

    info.add_dependency(misaxx::module_info());
    info.add_dependency(std::move(sqlite_info));

    return info;
}

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

#include <boost/program_options.hpp>
#include <misaxx/core/filesystem/misa_filesystem_empty_importer.h>
#include <misaxx/core/filesystem/misa_filesystem_directories_importer.h>
#include <misaxx/core/filesystem/misa_filesystem_json_importer.h>
#include <misaxx/core/misa_cached_data_base.h>
#include <misaxx/core/attachments/misa_location.h>
#include <misaxx/core/utils/manual_stopwatch.h>
#include <misaxx/core/runtime/misa_parameter_registry.h>
#include <misaxx/core/runtime/misa_cli.h>
#include <iomanip>
#include "misa_readme_builder.h"

using namespace misaxx;

namespace misaxx {
    struct misa_cli_impl {
        boost::filesystem::path m_parameter_schema_path;
        boost::filesystem::path m_readme_path;
    };
}

misa_cli::misa_cli() : m_pimpl(new misa_cli_impl()) {

}

misa_cli::~misa_cli() {
    delete m_pimpl;
}


misa_cli::cli_result misa_cli::load_from_cli(const int argc, const char **argv) {
    namespace po = boost::program_options;

    po::options_description general_options("Runtime options");
    general_options.add_options()
            ("help,h", "Help screen")
            ("version,v", "Prints the module name and version info")
            ("module-info", "Prints the module module information as serialized JSON")
            ("parameters,p", po::value<std::string>(), "Provides the list of parameters")
            ("threads,t", po::value<int>(), "Sets the number of threads")
            ("skip", "Requests that already existing results should be used instead of re-calculating them")
            ("write-parameter-schema", po::value<std::string>(), "Writes a parameter schema to the target file")
            ("write-readme", po::value<std::string>(), "Writes a README file to the target file")
            ("full-runtime-log", "Writes a comprehensive log containing the runtimes of each tasks into the output directory")
            ("write-worker-graph", "Writes the DAG of workers a misa-workers.dot into the output directory");

    po::command_line_parser parser(argc, argv);
    parser.options(general_options);
    po::parsed_options parsed_options = parser.run();

    po::variables_map vm;
    po::store(parsed_options, vm);
    po::notify(vm);

    if(vm.count("help")) {
        auto info = misaxx::runtime_properties::get_module_info();
        std::cout << info.get_id() << " " << info.get_version() << "\n";
        std::cout << general_options << "\n";
        return misa_cli::cli_result::no_workload;
    }
    if(vm.count("version")) {
        auto info = misaxx::runtime_properties::get_module_info();
        std::cout << info.get_id() << " " << info.get_version() << "\n";
        return misa_cli::cli_result::no_workload;
    }
    if(vm.count("module-info")) {
        auto info = misaxx::runtime_properties::get_module_info();
        nlohmann::json json;
        info.to_json(json);
        std::cout << json << "\n";
        return misa_cli::cli_result::no_workload;
    }
    if(vm.count("write-parameter-schema")) {
        this->set_is_simulating(true);
        m_pimpl->m_parameter_schema_path = vm["write-parameter-schema"].as<std::string>();
        if(!m_pimpl->m_parameter_schema_path.parent_path().empty())
            boost::filesystem::create_directories(m_pimpl->m_parameter_schema_path.parent_path());
    }
    if(vm.count("write-readme")) {
        this->set_is_simulating(true);
        m_pimpl->m_readme_path = vm["write-readme"].as<std::string>();
        if(!m_pimpl->m_readme_path.parent_path().empty())
            boost::filesystem::create_directories(m_pimpl->m_readme_path.parent_path());
    }
    if(vm.count("write-worker-graph")) {
        this->set_create_worker_graph(true);
    }
    if(vm.count("skip")) {
        if(!this->is_simulating()) {
            this->set_request_skipping(true);
        }
    }
    if(vm.count("threads")) {
        if(!this->is_simulating()) {
            this->set_num_threads(vm["threads"].as<int>());
        }
        else {
            this->set_num_threads(1);
            std::cout << "<#> <#> RUNNING IN SIMULATION MODE. This application will run only with 1 thread." << "\n";
        }
    }
//    if(vm.count("no-skip")) {
//        if(!m_runtime->is_simulating()) {
//            m_runtime->enable_skipping = false;
//        }
//    }
    if(vm.count("parameters")) {
        std::string filename = vm["parameters"].as<std::string>();
        std::cout << "<#> <#> Loading parameters from " << filename << "\n";
        if(!boost::filesystem::exists(filename))
            throw std::runtime_error("The file " + filename + " does not exist!");
        std::ifstream in { filename };
        nlohmann::json j;
        in >> j;
        this->set_parameter_json(std::move(j));
    }
    else if(!this->is_simulating()) {
        auto info = misaxx::runtime_properties::get_module_info();
        std::cout << info.get_id() << " " << info.get_version() << "\n";
        std::cout << general_options << "\n";
        return misa_cli::cli_result::error;
    }

    // Load runtime parameters that are not from CLI
    if(!vm.count("threads") && !this->is_simulating()) {
        auto schema = misaxx::parameter_registry::register_parameter({ "runtime", "num-threads" });
        schema->declare_optional<int>(1);
        this->set_num_threads(misaxx::parameter_registry:: template get_json<int>({ "runtime", "num-threads" }));
    }
    if(!vm.count("skip") && !this->requests_skipping()) {
        auto schema = misaxx::parameter_registry::register_parameter({ "runtime", "request-skipping" });
        schema->declare_optional<bool>(false);
        this->set_request_skipping(misaxx::parameter_registry:: template get_json<bool>({ "runtime", "request-skipping" }));
    }
    if(!vm.count("write-worker-graph") && !this->is_creating_worker_graph()) {
        auto schema = misaxx::parameter_registry::register_parameter({ "runtime", "write-worker-graph" });
        schema->declare_optional<bool>(false);
        this->set_create_worker_graph(misaxx::parameter_registry:: template get_json<bool>({ "runtime", "write-worker-graph" }));
    }
    if(!this->is_simulating()) {
        if(vm.count("full-runtime-log")) {
            this->set_enable_full_runtime_log(true);
        }
        else {
            auto schema = misaxx::parameter_registry::register_parameter({ "runtime", "full-runtime-log" });
            schema->declare_optional<bool>(false);
            this->set_enable_full_runtime_log(misaxx::parameter_registry::get_json<bool>({ "runtime", "full-runtime-log" }));
        }
    }

    return misa_cli::cli_result::continue_with_workload;
}

misa_cli::cli_result misa_cli::run() {
    std::cout << "<#> <#> Starting run with " << this->get_num_threads() << " threads" << "\n";

    if(this->is_simulating()) {
        std::cout << "<#> <#> RUNNING IN SIMULATION MODE. This will build a parameter schema, but no real work is done!" << "\n";
    }

    // Call the runtime's prepare & run function
    misa_runtime::prepare_and_run();

    // Build schema
    if(this->is_simulating()) {
        if(!m_pimpl->m_parameter_schema_path.empty()) {
            std::cout << "<#> <#> Writing parameter schema to " << m_pimpl->m_parameter_schema_path.string() << "\n";
            nlohmann::json j;
            this->get_schema_builder()->to_json(j);
            std::ofstream w;
            w.open(m_pimpl->m_parameter_schema_path.string());
            w << std::setw(4) << j;
        }

        if(!m_pimpl->m_readme_path.empty()) {
            std::cout << "<#> <#> Writing README to " << m_pimpl->m_readme_path.string() << "\n";
            nlohmann::json j;
            this->get_schema_builder()->to_json(j);
            build_readme(j, m_pimpl->m_readme_path);
        }
    }

    return misa_cli::cli_result::ok;
}

int misa_cli::prepare_and_run(const int argc, const char **argv) {
    const misa_cli::cli_result ret = load_from_cli(argc, argv);
    switch(ret) {
        case misa_cli::cli_result::continue_with_workload:
            if(run() == misa_cli::cli_result ::ok)
                return 0;
            else
                return 1;
        case misa_cli::cli_result::no_workload:
            return 0;
        case misa_cli::cli_result::error:
            return 1;
        case misa_cli::cli_result ::ok:
            return 0;
        default:
            throw std::runtime_error("Unsupported cli result case!");
    }
}

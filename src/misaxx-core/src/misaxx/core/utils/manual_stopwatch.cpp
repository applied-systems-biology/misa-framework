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

#include <misaxx/core/utils/manual_stopwatch.h>
#include <stdexcept>
#include <iostream>

using namespace misaxx::utils;

manual_stopwatch::manual_stopwatch(const std::string &t_name) : m_current_start(clock_t::now()),
                                                                m_initial_start(clock_t::now()),
                                                                m_operation(""),
                                                                m_name(t_name),
                                                                m_started(false) {}

manual_stopwatch::manual_stopwatch() : manual_stopwatch("Stopwatch") {}

double manual_stopwatch::current_elapsed_from_start() const {
    return std::chrono::duration_cast<duration_ms_t>
            (clock_t::now() - m_initial_start).count();
}

double manual_stopwatch::current_elapsed() const {
    return std::chrono::duration_cast<duration_ms_t>
            (clock_t::now() - m_current_start).count();
}

double manual_stopwatch::elapsed() const {

    if(m_started) {
        throw std::runtime_error("Stopwatch is currently running!");
    }

    return std::chrono::duration_cast<duration_ms_t>
            (m_stop - m_initial_start).count();
}

void manual_stopwatch::new_operation(const std::string &t_operation, bool t_announce) {

    if(!m_started)
        return;

    if(!m_operation.empty()) {
        std::cout << "<< Finished sub-operation >> " << "[" << current_elapsed() << "ms" << "] " << m_operation << " <> " << m_name << "\n";
    }

    m_current_start = clock_t::now();
    m_operation = t_operation;

    if(t_announce) {
        std::cout << "<< Started sub-operation >> " << m_operation << " <> " << m_name << "\n";
    }
}

void manual_stopwatch::start() {
    if(!m_started) {
        m_current_start = clock_t::now();
        m_initial_start = clock_t::now();
        std::cout << "<< Started >> " << m_name << "\n";

        m_started = true;
    }
}

void manual_stopwatch::stop() {
    if(m_started) {
        m_stop = clock_t::now();
        m_started = false;
        std::cout << "<< Finished >> " << "[" << elapsed() << "ms" << "] " << m_name << "\n";

        m_durations.push_back(elapsed());
    }
}

const std::vector<double> &manual_stopwatch::get_durations() const {
    return m_durations;
}

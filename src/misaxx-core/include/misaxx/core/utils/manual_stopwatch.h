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

#include <chrono>
#include <ratio>
#include <vector>
#include <string>

namespace misaxx::utils {
    /**
     * Manually controlled stopwatch that tracks the time between start() and stop().
     * Supports sub-operations that can be triggered from code (will currentls only output)
     * and multiple runs stored in get_durations()
     */
    class manual_stopwatch {
    public:

        using clock_t = std::chrono::high_resolution_clock;
        using duration_ms_t = std::chrono::duration<double, std::milli >;

        explicit manual_stopwatch(const std::string &t_name);

        manual_stopwatch();

        double current_elapsed_from_start() const;

        double current_elapsed() const;

        double elapsed() const;

        void new_operation(const std::string &t_operation, bool t_announce = true);

        void start();

        void stop();

        /**
         * Returns the
         * @return
         */
        const std::vector<double> &get_durations() const;

    private:

        std::chrono::time_point<clock_t> m_current_start;
        std::chrono::time_point<clock_t> m_initial_start;
        std::chrono::time_point<clock_t> m_stop;
        std::string m_operation;
        std::string m_name;
        bool m_started;

        std::vector<double> m_durations;
    };
}
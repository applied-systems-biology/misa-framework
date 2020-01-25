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
#include <cassert>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace misaxx::imaging::utils {
    /**
     * Finds percentiles of a vector.
     * NOTE: This function WILL partially sort the vector!
     * @param t_pixels
     * @param t_percentiles Vector of percentiles to fetch
     * @return List of percentile values with the same order as t_percentiles
     */
    template<typename T, typename U>
    inline void find_percentiles(std::vector<U> &t_result, std::vector<T> &t_pixels, const std::vector<double> &t_percentiles, bool t_multi_use_nth_element = true) {

        t_result.clear();

        if(t_pixels.empty()) {
            throw std::runtime_error("Cannot get percentile from empty array!");
        }
        if(t_pixels.size() == 1) {
            t_result.push_back(t_pixels[0]);
            return;
        }

        std::vector<int> ranks;
        ranks.reserve(t_percentiles.size());

        for(double percentile : t_percentiles) {
            assert(percentile >= 0.0 && percentile <= 100.0);
            int rank = static_cast<int>(std::ceil(percentile / 100.0 * t_pixels.size()));

            ranks.push_back(rank);
        }

        // Naive way is to use std::sort (full sorting) O(n*log n)
        // We use nth_element if we have only 1 element, otherwise we partially sort until the highest rank
        if(t_percentiles.size() > 1 && !t_multi_use_nth_element) {
            int reference_rank = *(std::max_element(ranks.begin(), ranks.end()));
            std::partial_sort(t_pixels.begin(), t_pixels.begin() + reference_rank, t_pixels.end());

            t_result.reserve(t_percentiles.size());

            for(int rank : ranks) {
                t_result.push_back(t_pixels[rank]);
            }
        }
        else {

            t_result.reserve(t_percentiles.size());

            for(int rank : ranks) {
                std::nth_element(t_pixels.begin(), t_pixels.begin() + rank, t_pixels.end());
                t_result.push_back(t_pixels[rank]);
            }
        }

    }

    template<typename T> inline T find_percentile(std::vector<T> &t_pixels, const double t_percentile) {
        std::vector<T> t_result;
        t_result.reserve(1);

        find_percentiles<T, T>(t_result, t_pixels, { t_percentile });

        return t_result[0];
    }

    template<typename T> inline T find_percentile(const std::vector<T> &t_pixels, const double t_percentile) {
        std::vector<T> t_result;
        t_result.reserve(1);

        std::vector<T> px(t_pixels);

        find_percentiles<T, T>(t_result, px, { t_percentile });

        return t_result[0];
    }

}
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

#include <misaxx/core/misa_cache.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <misaxx/core/misa_cache.h>
#include <misaxx/imaging/patterns/misa_image_pattern.h>
#include <misaxx/imaging/patterns/misa_image_stack_pattern.h>
#include <misaxx/core/misa_default_cache.h>
#include <misaxx/imaging/descriptions/misa_image_description.h>
#include <opencv2/opencv.hpp>

namespace misaxx::imaging {

    /**
     * A cache that holds an OpenCV cv::Mat or a coixx::image
     * @tparam Image
     */
    class misa_image_file_cache : public misaxx::misa_default_cache<misaxx::utils::cache<cv::Mat>,
            misa_image_pattern, misa_image_description> {
    public:

        cv::Mat &get() override;

        const cv::Mat &get() const override;

        void set(cv::Mat value) override;

        bool has() const override;

        bool can_pull() const override;

        void pull() override;

        void stash() override;

        void push() override;

        void do_link(const misa_image_description &t_description) override;

    protected:

        misa_image_description produce_description(const boost::filesystem::path &t_location, const misa_image_pattern &t_pattern) override;

    private:
        cv::Mat m_value;
        boost::filesystem::path m_path;
    };
}

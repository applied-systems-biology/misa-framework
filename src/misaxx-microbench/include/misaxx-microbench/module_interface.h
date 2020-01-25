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

#include <misaxx/core/misa_module_interface.h>
#include <misaxx/imaging/accessors/misa_image_file.h>
#include <misaxx/core/accessors/misa_exported_attachments.h>

namespace misaxx_microbench {
    struct module_interface : public misaxx::misa_module_interface {

        misaxx::imaging::misa_image_file m_input_image;
        misaxx::imaging::misa_image_file m_output_median;
        misaxx::imaging::misa_image_file m_output_morphology;
        misaxx::imaging::misa_image_file m_output_fft_ifft;
        misaxx::imaging::misa_image_file m_output_otsu;
        misaxx::imaging::misa_image_file m_output_percentile;
        misaxx::imaging::misa_image_file m_output_canny;
        misaxx::imaging::misa_image_file m_output_wiener;
        misaxx::misa_exported_attachments m_runtimes;

        void setup() override;
    };
}

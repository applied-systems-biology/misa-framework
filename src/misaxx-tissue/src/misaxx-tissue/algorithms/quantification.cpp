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

#include "quantification.h"
#include <misaxx/ome/attachments/misa_ome_pixel_count.h>
#include <misaxx-tissue/attachments/tissue.h>


using namespace misaxx;
using namespace misaxx::ome;
using namespace misaxx_tissue;

void quantification::work() {

    auto module = get_module_as<module_interface>();

    tissue result;

    // Find out how many pixels our object has
    result.pixels.count = 0;
    for(auto &plane : module->m_output_segmented) {
        if(!plane.has_attachment<misa_ome_pixel_count>()) {
            auto access = plane.access_readonly();
            auto px = cv::countNonZero(access.get());
            plane.attach(misa_ome_pixel_count(px));
            result.pixels.count += px;
        }
        else {
            result.pixels.count += plane.get_attachment<misa_ome_pixel_count>().count;
        }
    }

    // Calculate the volume
    result.volume = result.pixels.get_volume(module->m_voxel_size);
    module->m_output_quantification.attach(std::move(result));
}

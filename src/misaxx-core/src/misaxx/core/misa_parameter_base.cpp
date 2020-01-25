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

#include <misaxx/core/misa_parameter_base.h>
#include <misaxx/core/misa_json_schema_property.h>

using namespace misaxx;

misa_parameter_base::misa_parameter_base(misa_parameter_base::path t_location,
                                         std::shared_ptr<misa_json_schema_property> t_schema) : location(std::move(t_location)), schema(std::move(t_schema)) {

}
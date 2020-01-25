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

#include <misaxx/core/filesystem/misa_filesystem_empty_importer.h>

using namespace misaxx;

misa_filesystem misa_filesystem_empty_importer::import() {
    misa_filesystem vfs;
    vfs.imported = std::make_shared<misa_filesystem_entry>("imported", misa_filesystem_entry_type::imported);
    vfs.exported = std::make_shared<misa_filesystem_entry>("exported", misa_filesystem_entry_type::exported);
    return vfs;
}

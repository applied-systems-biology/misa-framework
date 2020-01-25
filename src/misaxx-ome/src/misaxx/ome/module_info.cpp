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

#include <misaxx/core/misa_module_info.h>
#include <misaxx/core/module_info.h>
#include <misaxx/ome/module_info.h>
#include <misaxx/imaging/module_info.h>

misaxx::misa_module_info misaxx::ome::module_info() {
    misaxx::misa_module_info info;
    info.set_id("misaxx-imaging-ome");
    info.set_version("1.0.0");
    info.set_name("MISA++ OME TIFF Support");
    info.set_description("Support for OME TIFF");
    info.add_author("Ruman Gerst");
    info.set_license("BSD-2-Clause");
    info.set_organization("Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Institute (HKI), Jena, Germany");
    info.set_url("https://applied-systems-biology.github.io/misa-framework/");

    // External dependency: OME Files
    misaxx::misa_module_info ome_files_info;
    ome_files_info.set_id("ome-files");
    ome_files_info.set_name("OME-Files");
    ome_files_info.set_url("https://www.openmicroscopy.org/");
    ome_files_info.set_organization("University of Dundee & Open Microscopy Environment");
    ome_files_info.set_citation("Goldberg, I. G., Allan, C., Burel, J.-M., Creager, D., Falconi, A., Hochheiser, H., Johnston, J., Mellen, J., Sorger, P. K., and Swedlow, J. R. (2005). The open microscopy environment (ome) data model and xml file: open tools for informatics and quantitative analysis in biological imaging. Genome biology, 6(5), R47.");
    ome_files_info.set_license("GPL");
    ome_files_info.set_is_external(true);

    info.add_dependency(misaxx::module_info());
    info.add_dependency(misaxx::imaging::module_info());
    info.add_dependency(std::move(ome_files_info));


    return info;
}

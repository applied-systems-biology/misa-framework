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
#include <misaxx/imaging/module_info.h>

misaxx::misa_module_info misaxx::imaging::module_info() {
    misaxx::misa_module_info info;
    info.set_id("misaxx-imaging");
    info.set_version("1.0.1.0");
    info.set_name("MISA++ Imaging Support");
    info.set_description("Support for OpenCV");
    info.add_author("Ruman Gerst");
    info.set_license("BSD-2-Clause");
    info.set_organization("Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Institute (HKI), Jena, Germany");
    info.set_url("https://applied-systems-biology.github.io/misa-framework/");

    // External dependency: OpenCV
    misaxx::misa_module_info opencv_info;
    opencv_info.set_id("opencv");
    opencv_info.set_name("OpenCV");
    opencv_info.set_url("https://opencv.org/");
    opencv_info.set_organization("OpenCV team");
    opencv_info.set_citation("Bradski, Gary, and Adrian Kaehler. \"OpenCV.\" Dr. Dobb’s journal of software tools 3 (2000).");
    opencv_info.set_license("BSD-3-Clause");
    opencv_info.set_is_external(true);

    // External dependency: OpenCV
    misaxx::misa_module_info libtiff_info;
    libtiff_info.set_id("libtiff");
    libtiff_info.set_name("LibTiff");
    libtiff_info.set_url("http://www.libtiff.org/");
    libtiff_info.set_authors({"Sam Leffler", "Frank Warmerdam", "Andrey Kiselev", "Mike Welles", "Dwight Kelly"});
    libtiff_info.set_license("BSD");
    libtiff_info.set_is_external(true);

    info.add_dependency(misaxx::module_info());
    info.add_dependency(std::move(opencv_info));
    info.add_dependency(std::move(libtiff_info));

    return info;
}

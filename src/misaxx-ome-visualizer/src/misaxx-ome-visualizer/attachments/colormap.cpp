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

#include "misaxx-ome-visualizer/attachments/colormap.h"
#include <iomanip>
#include <opencv2/opencv.hpp>

namespace {
    std::string bgr_to_hex(const cv::Vec3b &bgr) {
        uint x = ((static_cast<uint>(bgr[2]) * 255) << 16) |
                 ((static_cast<uint>(bgr[1]) * 255) << 8) |
                 ((static_cast<uint>(bgr[0]) * 255) << 0);

        std::stringstream stream;
        stream << "#" << std::setfill('0') << std::setw(6) << std::hex << x;
        std::string result( stream.str() );

        return result;

    }

    cv::Vec3b hex_to_bgr(const std::string &hex) {
        if(hex.length() != 7 || hex[0] != '#') {
            throw std::runtime_error("String must have format #RRGGBB!");
        }
		
        unsigned long x = std::stoul(hex.substr(1), nullptr, 16);

        unsigned long b = ((x >> 0) & 0xFF);
        unsigned long g = ((x >> 8) & 0xFF);
        unsigned long r = ((x >> 16) & 0xFF);

        return cv::Vec3b(static_cast<uchar>(b), static_cast<uchar>(g), static_cast<uchar>(r));
    }
}

void misaxx_ome_visualizer::colormap::from_json(const nlohmann::json &t_json) {
    misa_locatable::from_json(t_json);
    for(auto it = t_json.at("data").begin(); it != t_json.at("data").end(); ++it) {
        data[std::stoi(it.key())] = hex_to_bgr(it.value());
    }
}

void misaxx_ome_visualizer::colormap::to_json(nlohmann::json &t_json) const {
    misa_locatable::to_json(t_json);
    for(const auto &kv : data) {
        t_json["data"][std::to_string(kv.first)] = bgr_to_hex(kv.second);
    }
}

void misaxx_ome_visualizer::colormap::to_json_schema(misaxx::misa_json_schema_property &t_schema) const {
    misa_locatable::to_json_schema(t_schema);
    t_schema["data"].declare_required<std::unordered_map<std::string, std::string>>();
}

std::string misaxx_ome_visualizer::colormap::get_documentation_name() const {
    return "Recoloring map";
}

std::string misaxx_ome_visualizer::colormap::get_documentation_description() const {
    return "Contains information on which label is assigned to which color";
}

void misaxx_ome_visualizer::colormap::build_serialization_id_hierarchy(
        std::vector<misaxx::misa_serialization_id> &result) const {
    misa_locatable::build_serialization_id_hierarchy(result);
    result.emplace_back(misaxx::misa_serialization_id("misa-ome-visualizer", "attachments/colormap"));
}

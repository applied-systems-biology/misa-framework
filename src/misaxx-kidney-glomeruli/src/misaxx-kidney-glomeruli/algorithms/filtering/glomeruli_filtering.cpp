//
// Created by rgerst on 11.04.19.
//

#include <misaxx-kidney-glomeruli/module_interface.h>
#include "glomeruli_filtering.h"

void misaxx_kidney_glomeruli::glomeruli_filtering::work() {
    auto module = get_module_as<module_interface>();

    if(!m_enable_label_filtering.query() && !m_enable_quantification_filtering.query())
        return;

    std::unordered_set<int> invalid_glomeruli;
    {
        auto attachment_access = module->m_output_quantification.access_attachments_readonly();
        for(const auto &kv : attachment_access.get().at<glomeruli>().data) {
            if(!(kv.second.valid xor m_invert.query())) {
                invalid_glomeruli.insert(kv.second.label);
            }
        }
    }

    if(m_enable_label_filtering.query()) {
        for(size_t i = 0; i < module->m_output_segmented3d.size(); ++i) {
            std::cout << "Filtering output " << std::to_string(i) << "/" << std::to_string(module->m_output_segmented3d.size()) << "\n";
            auto layer_access = module->m_output_segmented3d.at(i).access_readwrite();
            for(int y = 0; y < layer_access.get().rows; ++y) {
                int *row = layer_access.get().ptr<int>(y);
                for(int x = 0; x < layer_access.get().cols; ++x) {
                    if(invalid_glomeruli.find(row[x]) != invalid_glomeruli.end()) {
                        row[x] = 0;
                    }
                }
            }
        }
    }
    if(m_enable_quantification_filtering.query()) {
        for(int invalid : invalid_glomeruli) {
            auto attachment_access = module->m_output_quantification.access_attachments_readwrite();
            attachment_access.get().at<glomeruli>().data.erase(invalid);
        }
    }
}

void misaxx_kidney_glomeruli::glomeruli_filtering::create_parameters(misaxx::misa_parameter_builder &t_parameters) {
    m_enable_label_filtering = t_parameters.create_algorithm_parameter<bool>("enable-label-filtering", true).document_title("Filter labels")
            .document_description("If enabled, the glomeruli3d data will only contain valid glomeruli.");
    m_enable_quantification_filtering = t_parameters.create_algorithm_parameter<bool>("enable-quantification-filtering", false).document_title("Filter quantification results")
            .document_description("If enabled, the quantification results will only contain valid glomeruli.");
    m_invert = t_parameters.create_algorithm_parameter<bool>("invert", false).document_title("Invert filter").document_description("If true, valid glomeruli are removed, instead");
}

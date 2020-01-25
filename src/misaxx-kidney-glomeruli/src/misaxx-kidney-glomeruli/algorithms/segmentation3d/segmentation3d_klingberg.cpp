//
// Created by rgerst on 08.04.19.
//

#include "segmentation3d_klingberg.h"
#include <set>

namespace cv::images {
    using mask = cv::Mat_<uchar>;
    using labels = cv::Mat_<int>;
}

namespace {
    template<typename Set1, typename Set2>
    bool set_intersects(const Set1 & set1, const Set2 &set2) {
        for(const auto &entry : set1) {
            if(set2.find(entry) != set2.end())
                return true;
        }
        return false;
    }
}

void misaxx_kidney_glomeruli::segmentation3d_klingberg::work() {

    auto module = get_module_as<module_interface>();

    std::vector<cv::images::labels> labels;
    size_t loaded_label_count = 0;
    size_t first_loaded_label_index = 0;

    const auto limsize = static_cast<size_t>(m_max_glomerulus_radius.query());

    int global_max_label = 0;

    for(size_t i = 0; i < module->m_output_segmented2d.size(); ++i) {
        cv::images::labels label;
        int max_label = cv::connectedComponents( module->m_output_segmented2d.at(i).access_readonly().get(),
                label, 4, CV_32S);

        std::cout << "Found " << max_label << " glomeruli in layer "<< std::to_string(i) << "\n";

        if(!labels.empty()) {
            // All connections from this layer -> labels of last layer
            std::unordered_map<int, std::unordered_set<int>> connections;

            // Look for connections to the last layer if available
            {
                const cv::images::labels &last_label = labels[i - 1];
                for(int y = 0; y < label.rows; ++y) {
                    const int *row = label[y];
                    const int *last_row = last_label[y];
                    for(int x = 0; x < label.cols; ++x) {
                        if(row[x] > 0) {
                            if(last_row[x] > 0) {
                                connections[row[x]].insert(last_row[x]);
                            }
                            else {
                                connections[row[x]]; // Declare existance
                            }
                        }
                    }
                }
            }


            std::unordered_map<int, int> local_renaming;
            std::unordered_map<int, int> global_renaming;

            for(auto &kv : connections) {
                if (kv.second.empty()) {
                    ++global_max_label;
                    local_renaming[kv.first] = global_max_label;
                }
                else if(kv.second.size() == 1) {
                    local_renaming[kv.first] = *kv.second.begin();
                }
                else {
                    int target = *kv.second.begin();
                    local_renaming[kv.first] = target;

                    // Find all labels that are connected to target
                    std::unordered_set<int> target_components;
                    for(auto &kv2 : connections) {
                        if(kv2.second.find(target) != kv2.second.end()) {
                            for(int src : kv2.second) {
                                if(src != target) {
                                    target_components.insert(src);
                                }
                            }
                        }
                    }

                    // Rename globally components -> target
                    for(int src : target_components) {
                        global_renaming[src] = target;
                    }

                    // Fix * -> component to: * -> target
                    for(auto &kv2 : global_renaming) {
                        if(target_components.find(kv2.second) != target_components.end()) {
                            kv2.second = target;
                        }
                    }

                    // Fix local renaming
                    for(auto &kv2 : local_renaming) {
                        if(target_components.find(kv2.second) != target_components.end()) {
                            kv2.second = target;
                        }
                    }

                    // Apply global renaming to all connections
                    for(auto &kv2 : connections) {
                        std::unordered_set<int> new_connections;
                        for(int src : kv2.second) {
                            auto it = global_renaming.find(src);
                            if(it != global_renaming.end())
                                new_connections.insert(it->second);
                            else
                                new_connections.insert(src);
                        }
                        kv2.second = std::move(new_connections);
                    }
                }
            }

            for(int y = 0; y < label.rows; ++y) {
                int *row = label[y];
                for(int x = 0; x < label.cols; ++x) {
                    if(row[x] > 0) {
                        row[x] = local_renaming.at(row[x]);
                    }
                }
            }

            for(size_t j = first_loaded_label_index; j < labels.size(); ++j) {
                cv::images::labels  &previous_label = labels.at(j);
                for(int y = 0; y < previous_label.rows; ++y) {
                    int *row = previous_label[y];
                    for(int x = 0; x < previous_label.cols; ++x) {
                        if(row[x] > 0) {
                            auto it = global_renaming.find(row[x]);
                            if(it != global_renaming.end()) {
                                row[x] = it->second;
                            }
                        }
                    }
                }
            }

            // Move the first pass results into the current buffer
            labels.emplace_back(std::move(label));
            ++loaded_label_count;
        }
        else {
            // Set global max label to current glomeruli count
            global_max_label = max_label;
            labels.emplace_back(std::move(label));
            ++loaded_label_count;
        }

        while(loaded_label_count > limsize) {
            module->m_output_segmented3d.at(first_loaded_label_index).access_write().set(std::move(labels.at(first_loaded_label_index)));
            --loaded_label_count;
            ++first_loaded_label_index;
        }
    }

    // Move all other labels into the cache
    for(size_t j = first_loaded_label_index; j < labels.size(); ++j) {
        if(labels.at(j).empty())
            throw std::logic_error("Unexpected unloaded label!");
        module->m_output_segmented3d.at(j).access_write().set(std::move(labels.at(j)));
    }
}

void misaxx_kidney_glomeruli::segmentation3d_klingberg::create_parameters(
        misaxx::misa_task::parameter_list &t_parameters) {
    misa_task::create_parameters(t_parameters);
    m_max_glomerulus_radius = t_parameters.create_algorithm_parameter<double>("max-glomerulus-radius", 65);
}

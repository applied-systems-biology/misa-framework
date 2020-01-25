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

#include "segment_experiment.h"
#include <iostream>

using namespace misaxx_segment_cells;

namespace cv::images {
    using grayscale8u = cv::Mat1b;
    using grayscale32f = cv::Mat1f;
    using mask = cv::Mat1b;
    using labels = cv::Mat1i;
}

namespace {
    cv::images::grayscale32f get_as_grayscale_float_copy(const cv::Mat &img) {
        if(img.type() == CV_32F) {
            return img.clone();
        }
        else if(img.type() == CV_64F) {
            cv::images::grayscale32f result;
            img.convertTo(result, CV_32F, 1);
            return result;
        }
        else if(img.type() == CV_8U) {
            cv::images::grayscale32f result;
            img.convertTo(result, CV_32F, 1.0 / 255.0);
            return result;
        }
        else if(img.type() == CV_16U) {
            cv::images::grayscale32f result;
            img.convertTo(result, CV_32F, 1.0 / std::numeric_limits<ushort>::max());
            return result;
        }
        else {
            throw std::runtime_error("Unsupported image depth: " + std::to_string(img.type()));
        }
    }

    cv::images::mask get_as_mask(const cv::images::grayscale32f &img) {
        cv::images::mask result {img.size(), 0};
        img.convertTo(result, CV_8U, 255);
        return result;
    }

    void close_holes(cv::images::mask &img) {
        using T = uchar;
        const uchar white = 255;
        const uchar black = 0;
        cv::images::mask buffer { img.size(), 0 };
        std::vector<cv::Point> neighbors;
        neighbors.emplace_back(cv::Point(-1,0));
        neighbors.emplace_back(cv::Point(1,0));
        neighbors.emplace_back(cv::Point(0,1));
        neighbors.emplace_back(cv::Point(0,-1));

        std::stack<cv::Point> stack;

        // Find the points in x direction (1st dimension)
        cv::Point pos(0,0);

        int rows = img.rows;
        int cols = img.cols;

        for(int i = 0; i < rows; ++i) {
            pos.x = 0;

            if(img.at<T>(pos) == 0 && buffer.at<T>(pos) == 0) {
                buffer.at<T>(pos) = white;
                stack.push(pos);
            }

            pos.x = cols - 1;

            if(img.at<T>(pos) == 0 && buffer.at<T>(pos) == 0) {
                buffer.at<T>(pos) = white;
                stack.push(pos);
            }

            // Increase counter of y
            if(pos.y < rows) {
                ++pos.y;
            }
            else {
                pos.y = 0;
            }
        }

        pos = cv::Point(0,0);

        // Find the points in y direction (2nd dimension)

        for(int i = 0; i < cols; ++i) {
            pos.y = 0;

            if(img.at<T>(pos) == 0 && buffer.at<T>(pos) == 0) {
                buffer.at<T>(pos) = white;
                stack.push(pos);
            }

            pos.y = rows - 1;

            if(img.at<T>(pos) == 0 && buffer.at<T>(pos) == 0) {
                buffer.at<T>(pos) = white;
                stack.push(pos);
            }

            // Increase counter of y
            if(pos.x < cols) {
                ++pos.x;
            }
            else {
                pos.x = 0;
            }
        }

        // Apply
        while(!stack.empty()) {
            cv::Point pos2 = stack.top();
            stack.pop();

            for(const cv::Point & rel_neighbor : neighbors) {
                cv::Point absolute = rel_neighbor + pos2;

                if(absolute.x >= 0 && absolute.y >= 0 && absolute.x < img.cols && absolute.y < img.rows) {
                    if(img.at<T>(absolute) == 0 && buffer.at<T>(absolute) == 0) {
                        buffer.at<T>(absolute) = white;
                        stack.push(absolute);
                    }
                }
            }
        }

        // Invert the image
        for(int i = 0; i < buffer.rows; ++i) {
            auto *row = buffer.ptr<T>(i);
            for(int j = 0; j < buffer.cols; ++j) {
                row[j] = white - row[j];
            }
        }

        img = buffer;
    }

    double get_max_value(const cv::Mat &img) {
        double max;
        cv::minMaxLoc(img, nullptr, &max);
        return max;
    }

    void normalize_by_max(cv::images::grayscale32f &img) {
        const double max = get_max_value(img);
        for(int y = 0; y < img.rows; ++y) {
            auto *row = img[y];
            for(int x = 0; x < img.cols; ++x) {
                row[x] = static_cast<float>(row[x] / max);
            }
        }
    }
}

void segment_experiment::work() {
    auto access = m_inputImage.access_readonly();
    cv::images::grayscale32f img = get_as_grayscale_float_copy(access.get());
    cv::GaussianBlur(img.clone(), img, cv::Size(0,0), 1.0);
    cv::images::mask thresholded { img.size(), 0 };
    cv::threshold(get_as_mask(img), thresholded, 0, 255, cv::THRESH_OTSU);
    close_holes(thresholded);
    cv::images::mask thresholded_inv = 255 - thresholded;

    // Find seed points
    cv::images::grayscale32f distance {img.size(), 0};
    cv::distanceTransform(thresholded, distance, CV_DIST_L2, CV_DIST_MASK_PRECISE);

    cv::images::grayscale32f dilated {img.size(), 0};
    cv::morphologyEx(distance, dilated, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5)));
    cv::images::mask local_maxima = distance == dilated;
    local_maxima.setTo(0, thresholded_inv);

    cv::images::labels seeds {img.size(), 0};
    cv::connectedComponents(local_maxima, seeds, 4, CV_32S);
    seeds.setTo(-1, thresholded_inv);

    // Convert distances into watershed heightmap (Because OpenCV implementation requires RGB)
    cv::Mat3b heightMap { img.size(), cv::Vec3b(0,0,0) };
    const double max_distance = get_max_value(distance);
    for(int y = 0; y < img.rows; ++y) {
        const float *row_src = distance[y];
        cv::Vec3b *row_dst = heightMap[y];
        for(int x = 0; x < img.cols; ++x) {
            auto v = static_cast<uchar>((max_distance - row_src[x]) / max_distance * 255);
            row_dst[x] = cv::Vec3b(v, v, v);
        }
    }

    // Apply watershedding
    cv::cvtColor(thresholded, heightMap, CV_GRAY2BGR);

    cv::watershed(heightMap, seeds);
    seeds.setTo(0, thresholded_inv);

    m_outputImage.write(seeds > 0);
}

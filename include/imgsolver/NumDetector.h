#pragma once

#include <opencv2/opencv.hpp>

namespace imgsolver {
    class NumDetector {
        public:
            virtual ~NumDetector() {}
            virtual int get_number(cv::Mat &image) = 0; // const = 0;
    };
}
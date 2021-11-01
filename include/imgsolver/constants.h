#pragma once
#include <opencv2/opencv.hpp>

namespace imgsolver {
    static const cv::Scalar WHITE_SCALAR        = cv::Scalar(255,255,255);
    static const int    UNDEFINED               = -2;
    static const int    BORDER_SIZE             = 5;
    static const float  CONSIDERED_LINE_FACTOR  = 0.5;
    static const std::string DEFAULT_MODEL      = "nono-model";
    static const std::string DEFAULT_MODEL_FILE = "../../models/tensorflow/nono-model-graph.pb";

    enum detector {TESSERACT,DNN};
}
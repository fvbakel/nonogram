#pragma once

#include <opencv2/opencv.hpp>

namespace imgsolver {
    class ImageDebug {
        protected:
            bool m_dump_images          = false;
            int  m_debug_file_nr        = 100;
            
            void debug_save_image(std::string &prefix,cv::Mat &image);

        public:
            ImageDebug();
            ~ImageDebug();

            void enable_dump_images();
    };
}
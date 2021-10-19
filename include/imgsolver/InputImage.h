#pragma once

#include <string>

namespace imgsolver {
    class InputImage {
        private:
            std::string      m_full_filename;
            std::string      m_filename;
        public:
            std::string     get_full_filename();
            std::string     get_filename();
            InputImage(const std::string &filename);
            ~InputImage();
    };
}
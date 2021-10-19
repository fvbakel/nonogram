#include <imgsolver/InputImage.h>

namespace imgsolver {

    InputImage::InputImage(const std::string &filename) {
        m_filename = std::string(filename);
    }

    std::string InputImage::get_full_filename() {
        return m_full_filename;
    }
    std::string InputImage::get_filename() {
        return m_filename;
    }

    InputImage::~InputImage() {
        return;
    }

}
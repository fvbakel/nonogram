#include   <sstream>
#include   <imgsolver/ImageDebug.h>


namespace imgsolver {
    ImageDebug::ImageDebug() {

    }

    void ImageDebug::enable_dump_images() {
        m_dump_images = true;
    }

    void ImageDebug::debug_save_image(std::string &prefix,cv::Mat &image) {
        std::stringstream filename;
        filename << "/tmp/D_" << prefix << "_" << m_debug_file_nr << ".png";
        cv::imwrite(filename.str() ,image);
        m_debug_file_nr++;
    }

    ImageDebug::~ImageDebug() {

    }


}
#include   <imgsolver/TesseractDetect.h>

namespace imgsolver {
    TesseractDetect::TesseractDetect(std::string modelname) {
        m_modelname = modelname;
        init();
    }

    void TesseractDetect::init() {
        m_ocr = new tesseract::TessBaseAPI();
        
        // export TESSDATA_PREFIX=/home/fvbakel/git/tessdata_best
        if (m_ocr->Init(NULL, m_modelname.c_str())) {
            std::cerr << "Could not initialize tesseract.\n";
            std::__throw_runtime_error("Could not initialize tesseract.");
        }

        m_ocr->SetVariable("user_defined_dpi", "70");
        m_ocr->SetVariable("tessedit_char_blacklist", ".,!?@#$%&*()<>_-+=/:;'\"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        m_ocr->SetVariable("tessedit_char_whitelist", "0123456789");
        m_ocr->SetVariable("classify_bln_numeric_mode", "1");
    }

    int TesseractDetect::get_number(cv::Mat &image) { // const {
        std::string detected_text;
        int result = -1;
        m_ocr->SetImage(image.data, image.cols, image.rows, image.channels() , image.step);
        detected_text = std::string(m_ocr->GetUTF8Text());
        detected_text.erase(std::remove(detected_text.begin(), detected_text.end(), '\n'), detected_text.end());
        char *p;
        int value = (int) strtol(detected_text.c_str(),&p,10);
        if (*p ) {
            std::cout << "ERROR: Unable to read number: " << detected_text<< "\n";
        } else {
            if (value > 0) {
                result = value;
            }
        }
        return result;
    }

    TesseractDetect::~TesseractDetect() {
        if (m_ocr != nullptr) {
            m_ocr->End();
            delete m_ocr;
        }
    }
}
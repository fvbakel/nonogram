#pragma once

#include <imgsolver/constants.h>
#include <tesseract/baseapi.h>
#include <imgsolver/NumDetector.h>


namespace imgsolver {
    class TesseractDetect : public NumDetector  {
        private:
            tesseract::TessBaseAPI *m_ocr = nullptr;
            std::string   m_modelname;

            void init();

        public:
            TesseractDetect(std::string modelname = DEFAULT_MODEL);
            ~TesseractDetect();
            int get_number(cv::Mat &image) const override ;
    };
}
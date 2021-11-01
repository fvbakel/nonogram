#pragma once

#include <vector>

#include <opencv2/opencv.hpp>
#include <imgsolver/constants.h>
#include <imgsolver/NumDetector.h>
#include <imgsolver/ImageDebug.h>


namespace imgsolver {
    class DnnDetect : public NumDetector, public ImageDebug {
        private:
            cv::dnn::Net            m_net;
            std::string             m_modelname;
            int                     m_width   = 10;
            int                     m_height  = 20;
            cv::Size                m_size = cv::Size(m_width,m_height);
            double                  m_c_scale = 1.0 / 255.0;
            cv::Mat                 m_org_img;
            cv::Mat                 m_gray_img;
            cv::Mat                 m_bw_img;
            std::vector<cv::Rect>   m_bb_digits;
            int                     m_cur_num = -1;

            void update_tmp_img();
            void update_bb_digits();
            int parse_digit(int i);

        public:
            DnnDetect(std::string modelname = DEFAULT_MODEL);
            ~DnnDetect();
            int get_number(cv::Mat &image) override; //const override ;
    };
}
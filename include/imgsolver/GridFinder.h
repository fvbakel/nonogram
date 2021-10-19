#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <tesseract/baseapi.h>
#include <solvercore/Nonogram.h>



namespace imgsolver {
    static const cv::Scalar WHITE_SCALAR = cv::Scalar(255,255,255);
    static const int UNDEFINED = -2;
    static const float Y_CLUE_GAB_FACTOR = 1.2;

    class GridFinder {
        private:
            cv::Mat    *m_org_img;
            cv::Mat     m_gray_img;
            cv::Mat     m_bw_img;

            tesseract::TessBaseAPI *m_ocr = nullptr;

            NonogramInput *m_output;

            std::vector<int> m_x_lines;
            std::vector<int> m_x_thickness;
            std::vector<int> m_y_lines;
            std::vector<int> m_y_thickness;

            int m_search_offset = 20;

            int m_smallest_y_clue_gab = UNDEFINED;
            int m_y_clue_gab = UNDEFINED;

            void determine_x_lines();
            void determine_y_lines();
            void process_x_clues();
            void process_y_clues(bool determine_y_clue_gab = false);
            
            void  update_y_clue_gab(cv::Rect  &rect);

            void parse_x_clue_column(cv::Rect  &rect);
            void parse_y_clue_line(cv::Rect  &rect);
            cv::Rect bounding_box(cv::Mat &image);
            int  parse_one_number(cv::Mat &image);

        public:
            GridFinder(cv::Mat *img);
            ~GridFinder();
            NonogramInput *parse();

            
    };
}
#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <tesseract/baseapi.h>
#include <solvercore/Nonogram.h>

namespace imgsolver {
    static const cv::Scalar WHITE_SCALAR = cv::Scalar(255,255,255);
    static const int    UNDEFINED = -2;
    static const float  CONSIDERED_LINE_FACTOR = 0.5;
    // 40% larger gab than the smallest gab is still considered the same number
    // TODO: make this a member variable that can be set?
    static const float Y_CLUE_GAB_FACTOR = 1.4;

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
            int m_x_right_offset      = UNDEFINED;
            int m_y_bottom_offset      = UNDEFINED;

            int m_smallest_y_clue_gab = UNDEFINED;
            int m_y_clue_gab = UNDEFINED;

            void cleanup_bw_img();
            void clear_left_border();
            void clear_top_border();
            void determine_offsets();
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
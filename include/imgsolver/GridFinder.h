#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <imgsolver/constants.h>
#include <imgsolver/NumDetector.h>
#include <imgsolver/TesseractDetect.h>

#include <solvercore/Nonogram.h>

namespace imgsolver {

    // 40% larger gab than the smallest gab is still considered the same number
    // TODO: make this a member variable that can be set?
    static const float Y_CLUE_GAB_FACTOR = 1.4;

    class GridFinder {
        private:
            cv::Mat    *m_org_img;
            cv::Mat     m_gray_img;
            cv::Mat     m_bw_img;
            cv::Mat     m_tmp_org_subset;
            cv::Mat     m_tmp_bw_subset;

            NumDetector *m_num_detector   = nullptr;

            NonogramInput *m_output;

            std::vector<int> m_x_lines;
            std::vector<int> m_x_thickness;
            std::vector<int> m_y_lines;
            std::vector<int> m_y_thickness;

            int m_search_offset         = 20;
            int m_x_right_offset        = UNDEFINED;
            int m_x_left_offset         = 0;
            int m_y_bottom_offset       = UNDEFINED;

            int m_smallest_y_clue_gab   = UNDEFINED;
            int m_largest_y_clue_gab    = UNDEFINED;
            int m_y_clue_gab            = UNDEFINED;

            bool m_has_fixed_y_width    = false;
            int  m_y_clue_width         = UNDEFINED;
            int  m_y_clue_line_thickness= UNDEFINED;

            bool m_parsed               = false;
            bool m_dump_images          = false;
            

            void cleanup_bw_img();
            void clear_left_border();
            void clear_top_border();
            void determine_fixed_width();
            void determine_offsets();
            void determine_x_lines();
            void determine_y_lines();
            void process_x_clues();
            void process_y_clues(bool determine_y_clue_gab = false);
            
            void update_y_clue_gab(cv::Rect  &rect);
            void check_y_clue_gab();

            void parse_x_clue_column(cv::Rect  &rect);
            void parse_y_clue_line(cv::Rect  &rect);
            void parse_y_clue_line_fixed_width();
            void parse_y_clue_line_gab_separated();
            bool extract_clue(cv::Rect &rect_clue, cv::Mat &result);
            bool bounding_box(cv::Mat &image, cv::Rect &result);
            int  parse_one_number(cv::Mat &image);

            int m_debug_file_nr = 300;
            void debug_save_image(std::string &prefix,cv::Mat &image);

        public:
            GridFinder(cv::Mat *img, std::string modelname = DEFAULT_MODEL);
            ~GridFinder();
            void enable_dump_images();
            void get_location(
                int x_index,
                int y_index,
                cv::Rect &rect
            );
            NonogramInput *parse();
    };
}
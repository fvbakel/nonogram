#include   <imgsolver/GridFinder.h>

namespace imgsolver {
    GridFinder::GridFinder(cv::Mat *img) {
        m_org_img = img;
        m_output = new NonogramInput();
        // make pure black and white
        if (m_org_img->channels() >1) {
            cv::cvtColor(*m_org_img, m_gray_img, cv::COLOR_BGR2GRAY);
        } else {
            m_gray_img = cv::Mat(*m_org_img);
        }
        cv::threshold(m_gray_img, m_bw_img,127, 255, cv::THRESH_BINARY);

        m_ocr = new tesseract::TessBaseAPI();
        // Initialize tesseract-ocr with English, without specifying tessdata path
        if (m_ocr->Init(NULL, "eng")) {
            std::cerr << "Could not initialize tesseract.\n";
            std::__throw_runtime_error("Could not initialize tesseract.");
        }

        m_ocr->SetVariable("user_defined_dpi", "70");
        //m_ocr->SetVariable("tosp_min_sane_kn_sp","");
        m_ocr->SetVariable("tessedit_char_blacklist", ".,!?@#$%&*()<>_-+=/:;'\"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        m_ocr->SetVariable("tessedit_char_whitelist", "0123456789");
        m_ocr->SetVariable("classify_bln_numeric_mode", "1");
    }

    void GridFinder::determine_x_lines() {
        int y = m_search_offset;
        int x = m_search_offset;

        bool last_was_black = false;
        int line_thickness = 0;
        for (x = m_search_offset ;x < m_bw_img.cols;x++) {
            // std::cout << x << ","<< y << "=" << ((int) m_bw_img.at<uchar>(x,y)) << "\n";
            if (((int) m_bw_img.at<uchar>(y,x)) == 0) {
                if (last_was_black) {
                    line_thickness++;
                } else {
                    line_thickness = 1;
                    m_x_lines.push_back(x);
                    last_was_black = true;
                }
            } else {
                if (last_was_black) {
                    m_x_thickness.push_back(line_thickness);
                }
                last_was_black = false;
            }
        }
    }

    void GridFinder::determine_y_lines() {
        int y = m_search_offset;
        int x = m_search_offset;

        bool last_was_black = false;
        int line_thickness = 0;
        for (y = m_search_offset ;y < m_bw_img.rows;y++) {
           // std::cout << x << ","<< y << "=" << ((int) m_bw_img.at<uchar>(x,y)) << "\n";
            if (((int) m_bw_img.at<uchar>(y,x)) == 0) {
                if (last_was_black) {
                    line_thickness++;
                } else {
                    line_thickness = 1;
                    m_y_lines.push_back(y);
                    last_was_black = true;
                }
            } else {
                if (last_was_black) {
                    m_y_thickness.push_back(line_thickness);
                }
                last_was_black = false;
            }
        }
    }

    void GridFinder::process_x_clues() {
        //TODO: determine/remove/add edge lines first
        int height = m_y_lines[0] + 1;
        int y = 0;
        for (int i = 0; i < m_x_lines.size() -1 ;i++) {
            int x = m_x_lines[i] + m_x_thickness[i];
            int width = m_x_lines[i+1] - x;
            cv::Rect rect(x, y, width, height);
            parse_x_clue_column(rect);
        }
    }

    /*
    Given a vertical oriented rectangle of the original image with 
    a sequence of one or more numbers seperated by some white space
    This functions will add this as a x clue to the output
    */
    void GridFinder::parse_x_clue_column(cv::Rect  &rect) {
        cv::Mat org_subset = (*m_org_img)(rect);
        cv::Mat bw_subset = (m_bw_img)(rect);
        int start_y = 0;
        int width = bw_subset.cols;
        std::vector<int> blacks;
        for (int y = 0; y < bw_subset.rows;y++) {
            bool black_found = false;
            for (int x = 0; x < bw_subset.cols;x++) {
                if (((int) bw_subset.at<uchar>(y,x)) == 0) {
                    black_found = true;
                    break;
                }
            }
            if (black_found && start_y == 0) {
                // first row with a black value
                start_y = y;
            } 

            if ( !black_found && start_y != 0) {
                int height = y - start_y;
                
                cv::Rect rect_clue(0, start_y, bw_subset.cols, height);
                cv::Mat clue = (org_subset)(rect_clue);
                cv::Mat cropped = clue(bounding_box(clue));
                cv::Mat border;
                int border_size = 5;
                cv::copyMakeBorder(cropped,border,border_size,border_size,0,border_size,cv::BORDER_CONSTANT,WHITE_SCALAR);
                int value = parse_one_number(border);
                if (value > 0) {
                    blacks.push_back(value);
                }
                //reset search
                start_y = 0;
            }
        }
        m_output->add_x_clue(blacks);
    }

    /*
    Given a horizontal oriented rectangle with a sequence of one or more
    numbers seperated by a space. This function will update the y gab 
    if a smaller size is found
    */
    void GridFinder::update_y_clue_gab(cv::Rect &rect) {
        cv::Mat bw_subset           = (m_bw_img)(rect);
        bool    previous_black      = false;
        bool    gab_start           = false;
        int     nr_with_white_only  = 0;

        for (int x = 0; x < bw_subset.cols;x++) {
            bool black_found = false;
            for (int y = 0; y < bw_subset.rows;y++) {
                if (((int) bw_subset.at<uchar>(y,x)) == 0) {
                    black_found = true;
                    break;
                }
            }
            if (black_found) {
                if (gab_start) {
                    // end of gab found
                    if (nr_with_white_only < m_smallest_y_clue_gab || m_smallest_y_clue_gab == UNDEFINED) {
                        m_smallest_y_clue_gab = nr_with_white_only;
                        m_y_clue_gab = m_smallest_y_clue_gab * Y_CLUE_GAB_FACTOR;
                    }
                    gab_start = false;
                }

                previous_black = true;
                nr_with_white_only = 0;
            } else {
                if (previous_black) {
                    gab_start = true;
                }
                previous_black = false;
                nr_with_white_only++;
            }
        }
    }

    /*
    Given a number remove the white space on all sides
    */
    cv::Rect GridFinder::bounding_box(cv::Mat &image) {
        int start_x =0;
        int start_y =0;
        int end_x = image.cols;
        int end_y = image.rows;

        for (int x = 0; x < image.cols;x++) {
            bool black_found = false;
            for (int y = 0; y < image.rows;y++) {
                if (((int) image.at<uchar>(y,x)) == 0) {
                    black_found = true;
                    break;
                }
            }
            if (black_found) {
                start_x = x;
                break;
            }
        }
        for (int x = (image.cols-1); x > start_x;x--) {
            bool black_found = false;
            for (int y = 0; y < image.rows;y++) {
                if (((int) image.at<uchar>(y,x)) == 0) {
                    black_found = true;
                    break;
                }
            }
            if (black_found) {
                end_x = x;
                break;
            }
        }
        for (int y = 0; y < image.rows;y++) {
            bool black_found = false;
            for (int x = 0; x < image.cols;x++) {
                if (((int) image.at<uchar>(y,x)) == 0) {
                    black_found = true;
                    break;
                }
            }
            if (black_found) {
                start_y = y;
                break;
            }
        }
        for (int y = (image.rows -1); y > start_y ;y--) {
            bool black_found = false;
            for (int x = 0; x < image.cols;x++) {
                if (((int) image.at<uchar>(y,x)) == 0) {
                    black_found = true;
                    break;
                }
            }
            if (black_found) {
                end_y = y;
                break;
            }
        }

        int width  = end_x - start_x;
        int height = end_y - start_y;
        cv::Rect rect(start_x, start_y, width, height);
        return rect;
    }
    
    /*
    Given a horizontal oriented rectangle of the original image with 
    a sequence of one or more numbers seperated by a space.
    This functions will add this as a y clue to the output
    */
    void GridFinder::parse_y_clue_line(cv::Rect  &rect) {
        cv::Mat org_subset = (*m_org_img)(rect);
        cv::Mat bw_subset = (m_bw_img)(rect);
        int start_x = 0;
        int nr_with_black = 0;
        int nr_with_white_only = 0;
        std::vector<int> blacks;
        for (int x = 0; x < bw_subset.cols;x++) {
            bool black_found = false;
            for (int y = 0; y < bw_subset.rows;y++) {
                if (((int) bw_subset.at<uchar>(y,x)) == 0) {
                    black_found = true;
                    break;
                }
            }
            if (black_found) {
                nr_with_black++;
                if (start_x == 0) {
                    start_x = x;
                }
                nr_with_white_only = 0;
            } else {
                nr_with_white_only++;
            }

            if (nr_with_black > 0 && (nr_with_white_only == m_y_clue_gab || x == (bw_subset.cols -1) )) {

                int width = x - start_x;
                cv::Rect rect_clue(start_x, 0, width, bw_subset.rows);
                cv::Mat clue = (org_subset)(rect_clue);
                cv::Mat cropped = clue(bounding_box(clue));
                cv::Mat border;
                int border_size = 5;
                cv::copyMakeBorder(cropped,border,border_size,border_size,0,border_size,cv::BORDER_CONSTANT,WHITE_SCALAR);
                int value = parse_one_number(border);
                if (value > 0) {
                    blacks.push_back(value);
                }
                //reset search
                start_x = 0;
                nr_with_black = 0;
                nr_with_white_only = 0;
            }
        }
        m_output->add_y_clue(blacks);
    }
    
    /*
    Given an image with one number, get that number as int
    */
    int GridFinder::parse_one_number(cv::Mat &image) {
        std::string detected_text;
        int result = -1;
        m_ocr->SetImage(image.data, image.cols, image.rows, image.channels() , image.step);
        detected_text = std::string(m_ocr->GetUTF8Text());
        //std::cout << "INFO: Found int as string:[" <<detected_text << "]\n";
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

       // cv::imshow("Test", image);
       // cv::waitKey(0);

        return result;
    }

    void GridFinder::process_y_clues(bool determine_y_clue_gab) {
        //TODO: determine/remove/add edge line information first
        
        int width = m_x_lines[0];
        int x = 0;
        for (int i = 0; i < m_y_lines.size() -1 ;i++) {
            int y = m_y_lines[i] + m_y_thickness[i];
            int height = m_y_lines[i+1] - y;
            cv::Rect rect(x, y, width, height);
            if (determine_y_clue_gab) {
                update_y_clue_gab(rect);
            } else {
                parse_y_clue_line(rect);
            }
        }
    }

    NonogramInput* GridFinder::parse() {
        determine_x_lines();
        determine_y_lines();
        process_y_clues(true);

        process_x_clues();
        process_y_clues();

        return m_output;
        //cv::imshow("Test", m_bw_img);
        //cv::waitKey(0);
        //imwrite("/tmp/test.png", m_bw_img);
    }

    GridFinder::~GridFinder() {
        if (m_ocr!=nullptr) {
            // Destroy used object and release memory
            m_ocr->End();
            delete m_ocr;
        }
    }
}

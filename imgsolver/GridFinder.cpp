#include   <imgsolver/GridFinder.h>

namespace imgsolver {
    GridFinder::GridFinder(cv::Mat *img, string modelname) {
        m_org_img = img;
        m_output = new NonogramInput();
        // make pure black and white
        if (m_org_img->channels() >1) {
            cv::cvtColor(*m_org_img, m_gray_img, cv::COLOR_BGR2GRAY);
        } else {
            m_gray_img = cv::Mat(*m_org_img);
        }
        cv::threshold(m_gray_img, m_bw_img,127, 255, cv::THRESH_BINARY);
        cleanup_bw_img();
       // cv::imshow("Test",m_bw_img);
      //  cv::waitKey(0);

        m_ocr = new tesseract::TessBaseAPI();
        
        // export TESSDATA_PREFIX=/home/fvbakel/git/tessdata_best
        if (m_ocr->Init(NULL, modelname.c_str())) {
            std::cerr << "Could not initialize tesseract.\n";
            std::__throw_runtime_error("Could not initialize tesseract.");
        }

        m_ocr->SetVariable("user_defined_dpi", "70");
        m_ocr->SetVariable("tessedit_char_blacklist", ".,!?@#$%&*()<>_-+=/:;'\"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        m_ocr->SetVariable("tessedit_char_whitelist", "0123456789");
        m_ocr->SetVariable("classify_bln_numeric_mode", "1");
    }

    void GridFinder::cleanup_bw_img() {
        clear_left_border();
        clear_top_border();
    }

    void GridFinder::clear_left_border() {
        int x = 0;
        int threshold = m_bw_img.rows * CONSIDERED_LINE_FACTOR;
        while (x<m_bw_img.cols) {
            int y = 0;
            int nr_black = 0;
            while (y<m_bw_img.rows) {
                if (((int) m_bw_img.at<uchar>(y,x)) == 0) {
                    nr_black++;
                }
                y++;
            }
            if (nr_black != 0) {
                if (nr_black > threshold) {
                    y = 0;
                    while (y < m_bw_img.rows) {
                        m_bw_img.at<uchar>(y,x) = 255;
                        y++;
                    }
                } else {
                    break;
                }
            }
            x++;
        }

        m_x_left_offset = x;
    }

    void GridFinder::clear_top_border() {
        int y = 0;
        int threshold = m_bw_img.cols * CONSIDERED_LINE_FACTOR;
        while (y<m_bw_img.rows) {
            int x = 0;
            int nr_black = 0;
            while (x<m_bw_img.cols) {
                if (((int) m_bw_img.at<uchar>(y,x)) == 0) {
                    nr_black++;
                }
                x++;
            }
            if (nr_black != 0) {
                if (nr_black > threshold) {
                    x = 0;
                    while (x < m_bw_img.cols) {
                        m_bw_img.at<uchar>(y,x) = 255;
                        x++;
                    }
                } else {
                    break;
                }
            }
            y++;
        }
    }

    void GridFinder::determine_fixed_width() {
        cv::Mat dark_bw_img;
        cv::threshold(m_gray_img, dark_bw_img,254, 255, cv::THRESH_BINARY);

        // get rectangle of first y clue
       // int width = m_x_lines[0] - m_x_left_offset;
        int x = m_x_left_offset;
        int y = m_y_lines[0] + m_y_thickness[0];
        //int height = m_y_lines[1] - y;
     //   int height = 1;
        //cv::Rect rect(x, y, width, height);
        //cv::Mat tmp_subset = dark_bw_img(rect);
        //std::string tmp = std::string("fixed");
        //debug_save_image(tmp,tmp_subset);
        bool last_was_black = false;
        while ( x < m_x_lines[0] ) {
            if (((int) dark_bw_img.at<uchar>(y,x))   == 0) {
                if (last_was_black) {
                    m_y_clue_line_thickness++;    
                } else {
                    if (m_y_clue_line_thickness == UNDEFINED) {
                        m_y_clue_line_thickness = 1;
                        m_y_clue_width = 0;
                    } else {
                        m_has_fixed_y_width = true;
                        break;
                    }
                }
                last_was_black =true;
            } else {
                m_y_clue_width++;
                last_was_black =false;
            }
            x++;
        }
        /*
        if (m_has_fixed_y_width) {
            std::cout << "Found fixed y clue width:" << m_y_clue_width << "\n";
            std::cout << "Found fixed y clue thickness:" << m_y_clue_line_thickness << "\n";
        } else {
            std::cout << "Not found fixed y clue width:" << m_y_clue_width << "\n";
            std::cout << "Not found fixed y clue thickness:" << m_y_clue_line_thickness << "\n";
        }
        */
        
    }

    void GridFinder::determine_offsets() {
        int x = m_bw_img.cols / 2;
        int x_2 = x + 7;
        int x_3 = x + 17;
        int y = m_bw_img.rows -1;
        bool first_black_found = false;
        while (m_y_bottom_offset == UNDEFINED && y > 0) {
            if (    ((int) m_bw_img.at<uchar>(y,x))   == 0 ||
                    ((int) m_bw_img.at<uchar>(y,x_2)) == 0 ||
                    ((int) m_bw_img.at<uchar>(y,x_3)) == 0
            ) {
                first_black_found = true;
            } 
            if (    first_black_found && (
                        ((int) m_bw_img.at<uchar>(y,x))   != 0 ||
                        ((int) m_bw_img.at<uchar>(y,x_2)) != 0 ||
                        ((int) m_bw_img.at<uchar>(y,x_3)) != 0
                    )
            ) {
                m_y_bottom_offset = y;
            }
            y--;
        }

        x = m_bw_img.cols -1;
        y = m_bw_img.rows / 2;
        int y_2 = y + 7;
        int y_3 = y + 17;
        first_black_found = false;
        while (m_x_right_offset == UNDEFINED && x > 0) {
            if (    ((int) m_bw_img.at<uchar>(y,x))   == 0 ||
                    ((int) m_bw_img.at<uchar>(y_2,x)) == 0 ||
                    ((int) m_bw_img.at<uchar>(y_3,x)) == 0
            ) {
                first_black_found = true;
            } 
            if (    first_black_found && (
                        ((int) m_bw_img.at<uchar>(y,x))   != 0 ||
                        ((int) m_bw_img.at<uchar>(y_2,x)) != 0 ||
                        ((int) m_bw_img.at<uchar>(y_3,x)) != 0
                    )
            ) {
                m_x_right_offset = x;
            }
            
            x--;
        }
        if (m_x_right_offset == UNDEFINED) {
            std::__throw_domain_error("Unable to determine x right offset!");
        }
        if (m_y_bottom_offset == UNDEFINED) {
            std::__throw_domain_error("Unable to determine y bottom offset!");
        }
    }

    void GridFinder::determine_x_lines() {
        int y = m_y_bottom_offset;
        int x = m_search_offset;

        bool last_was_black = false;
        int line_thickness = 0;
        for (x = m_search_offset ;x < m_x_right_offset+2;x++) {
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
        int x = m_x_right_offset;

        bool last_was_black = false;
        int line_thickness = 0;
        for (y = m_search_offset ;y < m_y_bottom_offset+2;y++) {
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
        m_tmp_org_subset = (*m_org_img)(rect);
        m_tmp_bw_subset = m_bw_img(rect);
        int start_y = 0;
        int width = m_tmp_bw_subset.cols;
        std::vector<int> blacks;
        for (int y = 0; y < m_tmp_bw_subset.rows;y++) {
            bool black_found = false;
            for (int x = 0; x < m_tmp_bw_subset.cols;x++) {
                if (((int) m_tmp_bw_subset.at<uchar>(y,x)) == 0) {
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
                cv::Rect rect_clue(0, start_y, m_tmp_bw_subset.cols, height);
                cv::Mat clue;
                if (extract_clue(rect_clue,clue)) {
                    int value = parse_one_number(clue);
                    if (value > 0) {
                        blacks.push_back(value);
                    }
                }
                //reset search
                start_y = 0;
            }
        }
        m_output->add_x_clue(blacks);
    }

    bool GridFinder::extract_clue(cv::Rect &rect_clue, cv::Mat &result) {
        cv::Mat clue = m_tmp_org_subset(rect_clue);
        cv::Mat bw_clue = m_tmp_bw_subset(rect_clue);
        cv::Rect bounding_box_rect;
        if (bounding_box(bw_clue,bounding_box_rect)) {
            cv::Mat cropped = clue(bounding_box_rect).clone();
            int border_size = BORDER_SIZE;
            cv::copyMakeBorder(cropped,result,border_size,border_size,0,border_size,cv::BORDER_CONSTANT,WHITE_SCALAR);
            return true;
        } else {
            return false;
        }
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
                    if (    nr_with_white_only > 0 &&
                            (   nr_with_white_only < m_smallest_y_clue_gab || 
                                m_smallest_y_clue_gab == UNDEFINED
                            )
                    ) {
                        m_smallest_y_clue_gab = nr_with_white_only;
                        m_y_clue_gab = ceil(m_smallest_y_clue_gab * Y_CLUE_GAB_FACTOR);
                    }
                    if (    nr_with_white_only > 0 &&
                            (   nr_with_white_only > m_largest_y_clue_gab || 
                                m_largest_y_clue_gab == UNDEFINED
                            )
                    ) {
                        m_largest_y_clue_gab = nr_with_white_only;
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

    void GridFinder::check_y_clue_gab() {
        if (m_largest_y_clue_gab < m_y_clue_gab ) {
            // there are no double numbers?
            m_y_clue_gab = m_smallest_y_clue_gab ;
        }
    }

    /*
    Given a number remove the white space on all sides
    */
    bool GridFinder::bounding_box(cv::Mat &image, cv::Rect &result) {
        int start_x =0;
        int start_y =0;
        int end_x = image.cols;
        int end_y = image.rows;

        // left
        bool found_at_least_one_black = false;
        for (int x = 0; x < image.cols;x++) {
            bool black_found = false;
            for (int y = 0; y < image.rows;y++) {
                if (((int) image.at<uchar>(y,x)) == 0) {
                    black_found = true;
                    found_at_least_one_black = true;
                    break;
                }
            }
            if (black_found) {
                start_x = x;
                break;
            }
        }
        // right
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
        // top
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
        // bottom
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
        // TODO: improve below ...
        int width  = end_x - start_x + 1;
        int height = end_y - start_y + 1;
        result.x=start_x;
        result.y=start_y;
        result.width=width;
        result.height=height;
        //cv::Rect rect(start_x, start_y, width, height);
        //result = rect;
        return found_at_least_one_black;
    }
    
    void GridFinder::parse_y_clue_line(cv::Rect  &rect) {
        m_tmp_org_subset = (*m_org_img)(rect);
        m_tmp_bw_subset = m_bw_img(rect);
        if (m_has_fixed_y_width) {
            parse_y_clue_line_fixed_width();
        } else {
            parse_y_clue_line_gab_separated();
        }
    }

    void GridFinder::parse_y_clue_line_fixed_width() {
        std::vector<int> blacks;
        int x = m_x_left_offset;
        while (x < m_x_lines[0]) {
            cv::Rect rect_clue(x, 0, m_y_clue_width, m_tmp_bw_subset.rows);
            cv::Mat clue;
            if (extract_clue(rect_clue,clue)) {
                int value = parse_one_number(clue);
                if (value > 0) {
                    blacks.push_back(value);
                }
            }
            x = x + m_y_clue_line_thickness + m_y_clue_width;
        }
        m_output->add_y_clue(blacks);
    }

    /*
    Given a horizontal oriented rectangle of the original image with 
    a sequence of one or more numbers seperated by a space 
    This functions will add this as a y clue to the output
    */
    void GridFinder::parse_y_clue_line_gab_separated() {
        int start_x = 0;
        int nr_with_black = 0;
        int nr_with_white_only = 0;
        std::vector<int> blacks;
        for (int x = m_x_left_offset; x < m_tmp_bw_subset.cols;x++) {
            bool black_found = false;
            for (int y = 0; y < m_tmp_bw_subset.rows;y++) {
                if (((int) m_tmp_bw_subset.at<uchar>(y,x)) == 0) {
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

            if (nr_with_black > 0 && (nr_with_white_only == m_y_clue_gab || x == (m_tmp_bw_subset.cols -1) )) {

                int width = x - start_x;
                cv::Rect rect_clue(start_x, 0, width, m_tmp_bw_subset.rows);
                cv::Mat clue;
                if (extract_clue(rect_clue,clue)) {
                    int value = parse_one_number(clue);
                    if (value > 0) {
                        blacks.push_back(value);
                    }
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
       // cv::imshow("Test",image);
       // cv::waitKey(0);
        if (m_dump_images) {
            debug_save_image(detected_text,image);
        }

        return result;
    }

    void GridFinder::process_y_clues(bool determine_y_clue_gab) {
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
        determine_offsets();
        determine_x_lines();
        determine_y_lines();
        determine_fixed_width();
        if (!m_has_fixed_y_width) {
            process_y_clues(true);
            check_y_clue_gab();
        }

        process_x_clues();
        process_y_clues();

        return m_output;
    }

    void GridFinder::enable_dump_images() {
        m_dump_images = true;
    }

    void GridFinder::debug_save_image(std::string &prefix,cv::Mat &image) {
        stringstream filename;
        filename << "/tmp/F_" << prefix << "_" << m_debug_file_nr << ".png";
        cv::imwrite(filename.str() ,image);
        m_debug_file_nr++;
    }

    GridFinder::~GridFinder() {
        if (m_ocr!=nullptr) {
            m_ocr->End();
            delete m_ocr;
        }
    }
}

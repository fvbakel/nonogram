
#include <sstream>
#include <imgsolver/DnnDetect.h>

namespace imgsolver {

    DnnDetect::DnnDetect(std::string modelname) {
        m_modelname = modelname;
        std::stringstream tmp_stream;
        char *dir_name = getenv("MODEL_DIR");
        if (dir_name != nullptr) {
            tmp_stream << dir_name;
        } else {
            tmp_stream << DEFAULT_MODEL_DIR;
        }
        tmp_stream << "/tensorflow/" << modelname << "-graph.pb";
        std::string model_filename = tmp_stream.str();
        m_net = cv::dnn::readNetFromTensorflow(model_filename);
    }

    int DnnDetect::get_number(cv::Mat &image) {
        m_cur_num = UNDEFINED;
        m_org_img = image;
        update_tmp_img();
        update_bb_digits();
        if (m_bb_digits.size() > 0) {
            m_cur_num = 0;
            for (int i = 0; i < m_bb_digits.size(); i++) {
                int digit = parse_digit(i);
                if (digit!=UNDEFINED) {
                    m_cur_num = m_cur_num * 10;
                    m_cur_num += digit;
                }
            }
        }
        
        return m_cur_num;
    }

    int DnnDetect::parse_digit(int i) {
        cv::Mat tmp_img;
        cv::resize( m_bw_img(m_bb_digits[i]),
                    tmp_img,
                    m_size,0,0,
                    cv::INTER_NEAREST
        );

        cv::Mat blob = cv::dnn::blobFromImage( tmp_img,
                                m_c_scale
         );

        m_net.setInput(blob);
        cv::Mat prediction = m_net.forward();
        cv::Point classIdPoint;
        double final_prob;
        cv::minMaxLoc(prediction.reshape(1, 1), 0, &final_prob, 0, &classIdPoint);
        int found_digit = classIdPoint.x;

        if (m_dump_images) {
            std::stringstream stream;
            stream << found_digit << "_digit_";
            std::string prefix = stream.str();
            debug_save_image(prefix,tmp_img);
        }
        
        return found_digit;
    }

    void DnnDetect::update_tmp_img() {
        if (m_org_img.channels() >1) {
            cv::cvtColor(m_org_img, m_gray_img, cv::COLOR_BGR2GRAY);
        } else {
            m_gray_img = cv::Mat(m_org_img);
        }
        cv::threshold(m_gray_img, m_bw_img,127, 255, cv::THRESH_BINARY);
    }

    void DnnDetect::update_bb_digits() {
        m_bb_digits.clear();
        int start_x = UNDEFINED;

        // left to right
        for (int x = 0; x < m_bw_img.cols;x++) {
            bool black_found = false;
            for (int y = 0; y < m_bw_img.rows;y++) {
                if (((int) m_bw_img.at<uchar>(y,x)) == 0) {
                    black_found = true;
                    break;
                }
            }
            if (black_found && start_x == UNDEFINED) {
                start_x = x;
            } else if (!black_found && start_x !=UNDEFINED) {
                int width = x - start_x;
                cv::Rect rect = cv::Rect(start_x,0,width,m_bw_img.rows);
                m_bb_digits.push_back(rect);
                start_x = UNDEFINED;
            }
        }
        if (start_x != UNDEFINED) {
            int width = m_bw_img.cols - start_x;
            cv::Rect rect = cv::Rect(start_x,0,width,m_bw_img.rows);
            m_bb_digits.push_back(rect);
        }

        // trim top down
        for (int i = 0; i < m_bb_digits.size(); i++) {
            start_x = m_bb_digits[i].x;
            int end_x = start_x + m_bb_digits[i].width;
            int start_y = m_bb_digits[i].y;
            int end_y = start_y + m_bb_digits[i].height;
            for (int y = start_y ;y< end_y;y++ ) {
                bool black_found = false;
                for (int x = start_x ;x< end_x;x++ ) {
                    if (((int) m_bw_img.at<uchar>(y,x)) == 0) {
                        black_found = true;
                        break;
                    }
                }
                if (black_found) {
                    m_bb_digits[i].y = y;
                    m_bb_digits[i].height = m_bb_digits[i].height - y;
                    break;
                }
            }
        }

        // trim bottom up
        for (int i = 0; i < m_bb_digits.size(); i++) {
            start_x = m_bb_digits[i].x;
            int end_x = start_x + m_bb_digits[i].width;
            int start_y = m_bb_digits[i].y;
            int end_y = start_y + m_bb_digits[i].height;
            for (int y = (end_y -1) ;y> start_y;y-- ) {
                bool black_found = false;
                for (int x = start_x ;x< end_x;x++ ) {
                    if (((int) m_bw_img.at<uchar>(y,x)) == 0) {
                        black_found = true;
                        break;
                    }
                }
                if (black_found) {
                    int diff = end_y - y -1;
                    m_bb_digits[i].height = m_bb_digits[i].height - diff;
                    break;
                }
            }
        }
    }

    DnnDetect::~DnnDetect() {

    }
}


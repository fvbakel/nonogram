#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <imgsolver/InputImage.h>
#include <imgsolver/GridFinder.h>
#include <solvercore/Nonogram.h>

using namespace imgsolver;
using namespace std;
using namespace cv;

class InputParser{
    public:
        InputParser (int &argc, char **argv){
            for (int i=1; i < argc; ++i)
                this->tokens.push_back(std::string(argv[i]));
        }

        const std::string& getCmdOption(const std::string &option) const{
            std::vector<std::string>::const_iterator itr;
            itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()){
                return *itr;
            }
            static const std::string empty_string("");
            return empty_string;
        }

        bool cmdOptionExists(const std::string &option) const{
            return std::find(this->tokens.begin(), this->tokens.end(), option)
                   != this->tokens.end();
        }
    private:
        std::vector <std::string> tokens;
};



void test_inputimage() {
    std::cout << "Start " << __FUNCTION__ << "\n";
    
    std::cout << "End " << __FUNCTION__ << "\n";
}


std::string vector_to_string(vector<int> *vect){
    std::stringstream str_str;
    for (int i = 0 ; i< vect->size();i++) {
        if (i!=0) {
            str_str << ",";
        }
        str_str << vect->at(i);
    }
    return str_str.str();
}


void assert_clue(vector<int> *clue, vector<int> *expected_clue, string &msg) {
    bool correct = true;
    if (clue->size() != expected_clue->size()) {
        correct =false;
    } else {
        for (int i = 0 ; i < clue->size();i++) {
            if (clue->at(i) != expected_clue->at(i)) {
                correct = false;
            }
        }
    }
    if (!correct) {
        std::cerr << msg <<": Got: "<< vector_to_string(clue) <<" Expected " << vector_to_string(expected_clue) << "\n";
        assert(correct);
    }
}

static const char* FILENAME = "../../test_data/puzzle-30-nonogram.png";
void test_gridfinder_1() {
    std::cout << "Start " << __FUNCTION__ << "\n";

    string filename = FILENAME;
    Mat img = cv::imread(filename, IMREAD_GRAYSCALE);
    GridFinder *finder = new GridFinder(&img);

    NonogramInput *input = finder->parse();
    std::cout <<"input->get_nr_of_x_clues()=" <<input->get_nr_of_x_clues() << "\n";
    std::cout <<"input->get_nr_of_y_clues()=" <<input->get_nr_of_y_clues() << "\n";
    assert(input->get_nr_of_x_clues() == 20);
    assert(input->get_nr_of_y_clues() == 20);

    string msg_str;
    vector<int> expected;

    msg_str = string("get_x_clue(0): ");
    expected.clear();
    expected.assign({2,2});
    assert_clue(input->get_x_clue(0),&expected,msg_str);

    msg_str = string("get_x_clue(1): ");
    expected.clear();
    expected.assign({2,2,2});
    assert_clue(input->get_x_clue(1),&expected,msg_str);

    msg_str = string("get_x_clue(2): ");
    expected.clear();
    expected.assign({1,2,2,2,1});
    assert_clue(input->get_x_clue(2),&expected,msg_str);

    msg_str = string("get_x_clue(3): ");
    expected.clear();
    expected.assign({2,9,1});
    assert_clue(input->get_x_clue(3),&expected,msg_str);

    msg_str = string("get_x_clue(4): ");
    expected.clear();
    expected.assign({2,10,2,2});
    assert_clue(input->get_x_clue(4),&expected,msg_str);

    msg_str = string("get_x_clue(5): ");
    expected.clear();
    expected.assign({4,4,2});
    assert_clue(input->get_x_clue(5),&expected,msg_str);

    msg_str = string("get_x_clue(6): ");
    expected.clear();
    expected.assign({3,3,2});
    assert_clue(input->get_x_clue(6),&expected,msg_str);

    msg_str = string("get_x_clue(7): ");
    expected.clear();
    expected.assign({1,2,4,3,2});
    assert_clue(input->get_x_clue(7),&expected,msg_str);

    msg_str = string("get_x_clue(8): ");
    expected.clear();
    expected.assign({1,2,2,2,5});
    assert_clue(input->get_x_clue(8),&expected,msg_str);

    msg_str = string("get_x_clue(9): ");
    expected.clear();
    expected.assign({4,1,2,4});
    assert_clue(input->get_x_clue(9),&expected,msg_str);

    msg_str = string("get_x_clue(10): ");
    expected.clear();
    expected.assign({3,2,1,2,4});
    assert_clue(input->get_x_clue(10),&expected,msg_str);

    msg_str = string("get_x_clue(11): ");
    expected.clear();
    expected.assign({2,3,2,3});
    assert_clue(input->get_x_clue(11),&expected,msg_str);

    msg_str = string("get_x_clue(12): ");
    expected.clear();
    expected.assign({1,3,3,2});
    assert_clue(input->get_x_clue(12),&expected,msg_str);

    msg_str = string("get_x_clue(13): ");
    expected.clear();
    expected.assign({6,4,2});
    assert_clue(input->get_x_clue(13),&expected,msg_str);

    msg_str = string("get_x_clue(14): ");
    expected.clear();
    expected.assign({2,12,1});
    assert_clue(input->get_x_clue(14),&expected,msg_str);

    msg_str = string("get_x_clue(15): ");
    expected.clear();
    expected.assign({11});
    assert_clue(input->get_x_clue(15),&expected,msg_str);

    msg_str = string("get_x_clue(16): ");
    expected.clear();
    expected.assign({3,7});
    assert_clue(input->get_x_clue(16),&expected,msg_str);

    msg_str = string("get_x_clue(17): ");
    expected.clear();
    expected.assign({4,2,2});
    assert_clue(input->get_x_clue(17),&expected,msg_str);

    msg_str = string("get_x_clue(18): ");
    expected.clear();
    expected.assign({2,2});
    assert_clue(input->get_x_clue(18),&expected,msg_str);

    msg_str = string("get_x_clue(19): ");
    expected.clear();
    expected.assign({2,2});
    assert_clue(input->get_x_clue(19),&expected,msg_str);

    /* Y clues*/
    msg_str = string("get_y_clue(0): ");
    expected.clear();
    expected.assign({3,2});
    assert_clue(input->get_y_clue(0),&expected,msg_str);

    msg_str = string("get_y_clue(1): ");
    expected.clear();
    expected.assign({2,2,2,1});
    assert_clue(input->get_y_clue(1),&expected,msg_str);

    msg_str = string("get_y_clue(2): ");
    expected.clear();
    expected.assign({13,1});
    assert_clue(input->get_y_clue(2),&expected,msg_str);

    msg_str = string("get_y_clue(3): ");
    expected.clear();
    expected.assign({9,2});
    assert_clue(input->get_y_clue(3),&expected,msg_str);

    msg_str = string("get_y_clue(4): ");
    expected.clear();
    expected.assign({3,3,2});
    assert_clue(input->get_y_clue(4),&expected,msg_str);

    msg_str = string("get_y_clue(5): ");
    expected.clear();
    expected.assign({4,4});
    assert_clue(input->get_y_clue(5),&expected,msg_str);

    msg_str = string("get_y_clue(6): ");
    expected.clear();
    expected.assign({4,3,2,1});
    assert_clue(input->get_y_clue(6),&expected,msg_str);

    msg_str = string("get_y_clue(7): ");
    expected.clear();
    expected.assign({2,2,2,2,2,2});
    assert_clue(input->get_y_clue(7),&expected,msg_str);

    msg_str = string("get_y_clue(8): ");
    expected.clear();
    expected.assign({1,2,1,1,5});
    assert_clue(input->get_y_clue(8),&expected,msg_str);

    msg_str = string("get_y_clue(9): ");
    expected.clear();
    expected.assign({3,1,2,4});
    assert_clue(input->get_y_clue(9),&expected,msg_str);

    msg_str = string("get_y_clue(10): ");
    expected.clear();
    expected.assign({4,2,3,1});
    assert_clue(input->get_y_clue(10),&expected,msg_str);

    msg_str = string("get_y_clue(11): ");
    expected.clear();
    expected.assign({2,2,2,4,2});
    assert_clue(input->get_y_clue(11),&expected,msg_str);

    msg_str = string("get_y_clue(12): ");
    expected.clear();
    expected.assign({1,3,10});
    assert_clue(input->get_y_clue(12),&expected,msg_str);

    msg_str = string("get_y_clue(13): ");
    expected.clear();
    expected.assign({5,8});
    assert_clue(input->get_y_clue(13),&expected,msg_str);

    msg_str = string("get_y_clue(14): ");
    expected.clear();
    expected.assign({2,3,5});
    assert_clue(input->get_y_clue(14),&expected,msg_str);

    msg_str = string("get_y_clue(15): ");
    expected.clear();
    expected.assign({1,5,2});
    assert_clue(input->get_y_clue(15),&expected,msg_str);

    msg_str = string("get_y_clue(16): ");
    expected.clear();
    expected.assign({1,4});
    assert_clue(input->get_y_clue(16),&expected,msg_str);

    msg_str = string("get_y_clue(17): ");
    expected.clear();
    expected.assign({1,7});
    assert_clue(input->get_y_clue(17),&expected,msg_str);

    msg_str = string("get_y_clue(18): ");
    expected.clear();
    expected.assign({11});
    assert_clue(input->get_y_clue(18),&expected,msg_str);

    msg_str = string("get_y_clue(19): ");
    expected.clear();
    expected.assign({8});
    assert_clue(input->get_y_clue(19),&expected,msg_str);

    delete input;
    delete finder;
    std::cout << "End " << __FUNCTION__ << "\n";
}

static const char* FILENAME_2 = "../../test_data/nonogram.org.png";
void test_gridfinder_2() {
    std::cout << "Start " << __FUNCTION__ << "\n";

    string filename = FILENAME_2;
    Mat img = cv::imread(filename, IMREAD_GRAYSCALE);
    GridFinder *finder = new GridFinder(&img);

    NonogramInput *input = finder->parse();
    std::cout <<"input->get_nr_of_x_clues()=" <<input->get_nr_of_x_clues() << "\n";
    std::cout <<"input->get_nr_of_y_clues()=" <<input->get_nr_of_y_clues() << "\n";
    assert(input->get_nr_of_x_clues() == 25);
    assert(input->get_nr_of_y_clues() == 25);

    string msg_str;
    vector<int> expected;

    msg_str = string("get_x_clue(0): ");
    expected.clear();
    expected.assign({7});
    assert_clue(input->get_x_clue(0),&expected,msg_str);

    delete input;
    delete finder;
    std::cout << "End " << __FUNCTION__ << "\n";
}

void print_usage() {
    std::cout << "Usage:\n";
    std::cout << "test_imgsolver [-h] [-i] [-s] -f filename\n";
    std::cout << "  Optional:\n";
    std::cout << "  -h            Display this help text\n";
    std::cout << "  -a            Run all tests\n";
    std::cout << "  -InputImg     Run ImgInput test\n";
    std::cout << "  -GridFinder   Run ImgInput test\n";
}

int main(int argc, char *argv[]) {

    InputParser input(argc, argv);
    if(input.cmdOptionExists("-h")){
        print_usage();
        exit(0);
    } 

    bool all =false;
    if(input.cmdOptionExists("-a")){
        all = true;
    }

    if(input.cmdOptionExists("-InputImg") || all){
        test_inputimage() ;
    }

    if(input.cmdOptionExists("-GridFinder_1") || all){
        test_gridfinder_1() ;
        
    }
    if(input.cmdOptionExists("-GridFinder_2") || all){
        test_gridfinder_2() ;
        
    }

    return 0;
}

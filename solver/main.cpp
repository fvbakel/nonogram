#include <stdio.h>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types_c.h>

#include <imgsolver/GridFinder.h>
#include <solvercore/Nonogram.h>



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

using namespace cv;

void display_nonogram(string &filename,Nonogram *nonogram) {
    namedWindow(filename,1);
    int x_size = nonogram->get_x_size();
    int y_size = nonogram->get_y_size();
    Vec3b white_col = Vec3b(255,255, 255);
    Vec3b black_col = Vec3b(0,0, 0);
    Vec3b no_color_col = Vec3b(0,255, 0);
    Mat result(y_size,x_size,CV_8UC3, Scalar(255,255, 255));
    for (int y_index = 0; y_index < y_size; y_index++) {
        for (int x_index = 0; x_index < x_size; x_index++) {
            Location *location = nonogram->get_Location(x_index, y_index);
            enum color loc_color = location->get_color();
            Vec3b *chosen = &white_col;
            if (loc_color == black) {
                chosen = &black_col;
            } else if (loc_color == no_color) {
                chosen = &no_color_col;
            }
            result.at<Vec3b>(Point(x_index,y_index)) = *chosen;
        }
    }

    int scale = 512 / x_size;
    Mat scaled;
    resize(result,scaled,Size(),scale,scale,INTER_NEAREST);
    //display the image:
    imshow(filename, scaled);
    
    //wait for the user to press any key:
    waitKey(0);
}

void print_solved_status(Nonogram *nonogram) {
    if(nonogram->is_solved()) {
        printf("Solved successfully\n");
    } else {
        printf("Unable to solve\n");
    }
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void process_file ( string &filename, 
                    bool rule_improve_log = false,
                    bool dispay = false,
                    bool dump_images =false
) {
    printf("Start processing: %s\n",filename.c_str());
    Nonogram *nonogram = nullptr;
    NonogramInput *input = nullptr;
    if (hasEnding(filename,string("png"))) {
        Mat img = cv::imread(filename, IMREAD_GRAYSCALE);
        imgsolver::GridFinder finder = imgsolver::GridFinder(&img);
        if (dump_images) {
            finder.enable_dump_images();
        }
        auto start = std::chrono::high_resolution_clock::now();
        input = finder.parse();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        cout << "Picture parsing ready, took:" << duration.count() << " sec" << endl;
        nonogram = new Nonogram(*input);
    } else if ( hasEnding(filename,string("non")) ||
                hasEnding(filename,string("txt"))
    ) {
        nonogram = new Nonogram(filename);
    } else {
        printf("Error, fileformat is not supported!\n");
    }

    if (nonogram!=nullptr) {
        if (nonogram->is_input_valid()) {
            if (rule_improve_log) {
                nonogram->enable_rule_improve_log();
            }
            auto start = std::chrono::high_resolution_clock::now();
            nonogram->solve();
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
            cout << "Solving ready, took:" << duration.count() << " sec" << endl;

            if (dispay) {
                print_solved_status(nonogram);
                display_nonogram(filename,nonogram);
            } else {
                nonogram->print();
                print_solved_status(nonogram);
            }
        }
        delete nonogram;
        if (input != nullptr) {
            delete input;
        }
    }
    printf("End processing: %s\n",filename.c_str());
}

void print_usage() {
    printf("Usage:\n");
    printf("nonogram [-h] [-i] [-s] -f filename\n");
    printf("  Optional:\n");
    printf("  -h    Display this help text\n");
    printf("  -i    Log possible improvements for the rule mechanism\n");
    printf("  -s    Show solution as a image\n");
    printf("  -d    Dump images used in the OCR process in /tmp\n");
    printf("  Required:\n");
    printf("  -f    Input file name. Supported formats: txt,non and png\n");
}

int main(int argc, char *argv[]) {

    

    InputParser input(argc, argv);
    if(input.cmdOptionExists("-h")){
        print_usage();
        exit(0);
    } else if (!input.cmdOptionExists("-f")) {
        printf("Missing -f parameter\n");
        print_usage();
        exit(0);
    }
    std::string filename = input.getCmdOption("-f");

    process_file (
        filename,
        input.cmdOptionExists("-i"),
        input.cmdOptionExists("-s"),
        input.cmdOptionExists("-d")
    );
    return 0;
}

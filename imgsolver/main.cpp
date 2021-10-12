#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types_c.h>
#include <Nonogram.h>

using namespace cv;

void display(string &filename,Nonogram *nonogram) {
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

void process_file (string &filename, bool rule_improve_log = false) {
    printf("Start processing: %s\n",filename.c_str());
    Nonogram *nonogram = new Nonogram(filename);
    if (rule_improve_log) {
        nonogram->enable_rule_improve_log();
    }
    nonogram->solve();

    if(nonogram->is_solved()) {
        printf("Solved successfully\n");
    } else {
        printf("Unable to solve\n");
    }
    display(filename,nonogram);
    delete nonogram;
    printf("End processing: %s\n",filename.c_str());
}

int main(int argc, char *argv[]) {

    if (argc > 1) {
        printf("Started\n");
        string filename = string(argv[1]);
        bool rule_improve_log = false;
        if (argc > 2) {
            rule_improve_log = true;
        }
        process_file (filename,rule_improve_log);
        printf("Ready\n");
    }
    return 0;
}

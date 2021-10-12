#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types_c.h>

using namespace cv;

int main(int argc, char** argv) {
    
    //create a gui window:
    namedWindow("Output",1);
    
    //initialize a 120X350 matrix of black pixels:
    //Mat output = Mat::zeros( 120, 350, CV_8UC3 );
    Mat output(120,350,CV_8UC3, Scalar(255,255, 255));
    
    int x = 0;
    int y = 0;
    int width = 10;
    int height = 10;
    // our rectangle...
    cv::Rect rect(x, y, width, height);
    
    cv::rectangle(output, rect, cv::Scalar(0, 0, 0),FILLED);
    
    output.at<cv::Vec3b>(12,12)[0]=0; // change it to black
    output.at<cv::Vec3b>(12,12)[1]=0;
    output.at<cv::Vec3b>(12,12)[2]=0;

    //write text on the matrix:
    putText(output,
            "Hello World :)",
            cvPoint(15,70),
            FONT_HERSHEY_PLAIN,
            3,
            cvScalar(0,255,0),
            4);
    
    //display the image:
    imshow("Output", output);
    
    //wait for the user to press any key:
    waitKey(0);
    
    return 0;

}

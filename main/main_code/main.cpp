#include <iostream>
#include <stdio.h>
#include "main.h"

// OpenCV libraries
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Namepaces
using namespace std;
using namespace cv;

bool maze;
int threshold_min = 40;
int threshold_max = 255;
int largest_contour_index = 0;
int largest_area = 0;
int img_width = 640;
int img_height = 480;

float cx(Mat frame)
{
    largest_contour_index = 0;
    largest_area = 0;  

    Mat grey;
    cvtColor(frame, grey, COLOR_BGR2GRAY);

    Mat blur;
    GaussianBlur(grey, blur, Size(9, 9), 0);

    Mat threshold_img;
    threshold(blur, threshold_img, threshold_min, threshold_max, THRESH_BINARY_INV);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Mat contoured = frame.clone();
    findContours(threshold_img.clone(), contours, hierarchy, 1, CHAIN_APPROX_NONE);
      
    for (int i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours.at(i));

        if (area > largest_area)
        {
            largest_contour_index = i;
            largest_area = area;
        }
    }

    if (contours.size() > 0)
    {
        drawContours(contoured, contours, largest_contour_index, Scalar(0, 255, 0), 1);
        Moments m = moments(contours[largest_contour_index]);

        float cx = m.m10 / m.m00;

        line(contoured, Point(cx, 0), Point(cx, 720), Scalar(255, 0, 0), 1);

        return(cx);
    }
    else
    {
        return(NULL);
    }
}

void mode_maze()
{
    Mat frame; // Stores video frame
    VideoCapture cap; // Store video
    
    cap.open(0, 0); // Initates capture (device id, default api)
    cap.set(CAP_PROP_FRAME_WIDTH, img_width);
    cap.set(CAP_PROP_FRAME_HEIGHT, img_height);

    if (!cap.isOpened()) // Unable to open capture
    { 
        cerr << "ERROR! Unable to open camera\n";
        return;
    }
    
    while(maze)
    {      
        cap.read(frame);

        Mat f_left = frame(Rect(0, 0, img_width/3, img_height));
        Mat f_mid = frame(Rect(img_width / 3, 0, img_width / 3, img_height));
        Mat f_right = frame(Rect(2*(img_width/3), 0, img_width / 3, img_height));

        float cx_l = cx(f_left);
        float cx_m = cx(f_mid);
        float cx_r = cx(f_right);

        imshow("l", f_left);
        imshow("m", f_mid);
        imshow("r", f_right);

        if (waitKey(5) >= 0)
            break;
    }
    return;
}

int main()
{
    maze = true;
    mode_maze();
}
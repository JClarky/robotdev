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

// Camera setup
int threshold_min = 40;
int threshold_max = 255;
int largest_contour_index = 0;
int largest_area = 0;
int img_width = 640;
int img_height = 480;

// Mode
bool maze;

// Get center of blob
float cx(Mat frame)
{
    return(50);
    largest_contour_index = 0;
    largest_area = 0;  

    // Turn  frame grey
    Mat grey;
    cvtColor(frame, grey, COLOR_BGR2GRAY);

    // Blur greyed frame
    Mat blur;
    GaussianBlur(grey, blur, Size(9, 9), 0);

    // Make greyed frame b/w
    Mat threshold_img;
    threshold(blur, threshold_img, threshold_min, threshold_max, THRESH_BINARY_INV);

    // Contour vectors
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // Clone frame to be manipulated
    Mat contoured = frame.clone();
    // Draw contours 
    findContours(threshold_img.clone(), contours, hierarchy, 1, CHAIN_APPROX_NONE);
      
    // Find the largest contour
    for (int i = 0; i < contours.size(); i++)
    {
        // Get the area of the current contour
        double area = contourArea(contours.at(i));

        // Compare current contours area to previous contour area
        if (area > largest_area)
        {
            largest_contour_index = i;
            largest_area = area;
        }
    }

    // Draw contours onto frame
    if (contours.size() > 0)
    {
        drawContours(contoured, contours, largest_contour_index, Scalar(0, 255, 0), 1);
        Moments m = moments(contours[largest_contour_index]);

        float cx = m.m10 / m.m00;

        // Draw line onto biggest contour
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

    // Unable to open capture
    if (!cap.isOpened())
    { 
        cerr << "ERROR! Unable to open camera\n";
        return;
    }
    
    // Main loops for maze mode
    while(maze)
    {    
        //cap.read(frame);
        frame = imread("C:/Users/jayde/OneDrive/Documents/Code/Robot_Development/robotdev/simulation/cam.jpg");

        if (frame.empty())
        {
            continue;
        }
        
        // Split image into three sections
        Mat f_left = frame(Rect(0, 0, img_width/3, img_height));
        Mat f_mid = frame(Rect(img_width / 3, 0, img_width / 3, img_height));
        Mat f_right = frame(Rect(2*(img_width/3), 0, img_width / 3, img_height));

        // Get cx of each split frame
        float cx_l = cx(f_left);
        float cx_m = cx(f_mid);
        float cx_r = cx(f_right);

        // If there is a contour on the left
        
        if (cx_l)
        {
            move(-10,100);
        }
        // If there is a contour in the middle
        else if (cx_m)
        {
            // Get center pixel value of image
            float c = img_width / 2;
            // If the the contour is to the right of center
            if (cx_m > c)
            {
                cx_m = cx_m - c;
                float r = (cx_m / c) * 100;
                move(100, r);
            }
            // If the the contour is to the left of center
            else
            {
                float l = (cx_m / c) * 100;
                move(l, 100);
            }

        }
        // If there is a contour on the right
        else if (cx_r)
        {
            move(100, -10);
        }
        // No contours
        else
        {
            move(100, -20);
        }

        // Display frames
        imshow("l", f_left);
        imshow("m", f_mid);
        imshow("r", f_right);

        if (waitKey(5) >= 0)
            break;
    }
    return;
}

void sumo_mode()
{
    if (true)
    {

    }
}

int main()
{
    maze = true;
    mode_maze();
    output out;
    cout << out.s_left_distance;
}
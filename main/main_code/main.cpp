#include <iostream>
#include <stdio.h>
//#include <hardware_interface.h>
//#include <simulation_interface.h>

// OpenCV libraries
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

bool maze;
int threshold_min = 70;
int threshold_max = 255;

RNG rng(12345);

void mode_maze()
{
    Mat frame; // Stores video frame
    VideoCapture cap; // Store video
    cap.open(0, 0); // Initates capture (device id, default api)

    if (!cap.isOpened()) // Unable to open capture
    { 
        cerr << "ERROR! Unable to open camera\n";
        return;
    }
    
    while(maze)
    {        
        cap.read(frame); // Reads the cameras frame
        if(frame.empty())
        {
            cout << "lmao the frame is blank";
            break;
        }

        Mat grey;
        cvtColor(frame, grey, COLOR_BGR2GRAY);

        Mat blur;
        GaussianBlur(grey, blur, Size(9,9), 0);

        Mat threshold_img;
        threshold(blur, threshold_img, threshold_min, threshold_max, THRESH_BINARY_INV);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        
        Mat contoured = frame.clone();
        findContours(threshold_img.clone(), contours, hierarchy, 1, CHAIN_APPROX_NONE);

        cout << "\nContour";
        cout << contours[0];
        cout << "\n";

        int largest_area;
        int largest_contour_index;

        for( size_t i = 0; i< contours.size(); i++ ) // iterate through each contour.
        {
            double area = contourArea( contours[i] );  //  Find the area of contour

            if( area > largest_area )
            {
                largest_area = area;
                largest_contour_index = i;               //Store the index of largest contour 
            }
        }

        int idx = 0;
        /*for( ; idx >= 0; idx = hierarchy[idx][0] )
        {
            drawContours(contoured, contours, idx, (0,255,0), 2, LINE_8, hierarchy);
        }*/

        drawContours(contoured, contours, largest_contour_index, (0,255,0), 2, LINE_8, hierarchy);

        //drawContours(contoured, contours, 0, (0,255,0), FILLED, 8, hierarchy);

        try
        {
            //imshow("GREY", grey);
            imshow("BLUR", blur);
            imshow("THRESHOLD", threshold_img);
            imshow("CONTOURED", contoured);
        }
        catch(const exception& e)
        {
            cerr << e.what() << '\n';
            cout << "imshow error";
        }        
        

        if (waitKey(5) >= 0)
            break;
    }
    return; // Camera disabled 
}

int main()
{
    maze = true;
    mode_maze();
}
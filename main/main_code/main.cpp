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
        cout << "gtfo cam starting";
        
        cap.read(frame); // Reads the cameras frame
        if(frame.empty())
        {
            cout << "lmao the frame is blank";
            break;
        }
        Mat grey;
        cvtColor(frame, grey, COLOR_BGR2GRAY);

        imshow("RAW", frame);
        imshow("GREY", grey);

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
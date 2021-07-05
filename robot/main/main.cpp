/**
 * @file main.cpp 
 * @author Jayden Clark
 * 
 * @brief 
 * This code is the main source file the robot
 * It controls all main high level logic of the robot
 * This mainly includes mode control
 * 
 * @version 1
 * @date 2021-02-25 
 * @copyright Copyright (c) 2021
 * 
 * RUN BUILD USING "sudo ./robot/builds/current"
 *
 */

// General libraries
#include <pigpio.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <math.h>  
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// Header files
#include "main.h"

// OpenCV libraries
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Namepaces
using namespace std;
using namespace cv;

// Hardware/Sim interface
Output out;

// Camera setup
int threshold_min = 40;
int threshold_max = 255;
int largest_contour_index = 0;
int largest_area = 0;
int img_width = 640;
int img_height = 480;

// Mode
bool maze;

float max_distance = 500; // Max distance to follow
float left_angle = -30;
float right_angle = 30;

// Left distance sensor
#define LEFT_DISTANCE_PIN_ECHO      17
#define LEFT_DISTANCE_PIN_TRIGGER    27

// Middle left distance sensor
#define MIDDLE_LEFT_DISTANCE_PIN_ECHO      18
#define MIDDLE_LEFT_DISTANCE_PIN_TRIGGER    23

// Middle distance sensor
#define MIDDLE_DISTANCE_PIN_ECHO      24
#define MIDDLE_DISTANCE_PIN_TRIGGER    25

// Middle right distance sensor
#define MIDDLE_RIGHT_DISTANCE_PIN_ECHO      11
#define MIDDLE_RIGHT_DISTANCE_PIN_TRIGGER    8

// Right distance sensor
#define RIGHT_DISTANCE_PIN_ECHO      7
#define RIGHT_DISTANCE_PIN_TRIGGER    1

//
// Maze mode
//

// Get center of blob
float cx(Mat frame)
{
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
        return(0);
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
        Mat f_left = frame(Rect(0, img_height/2, img_width/3, img_height/2));
        Mat f_mid = frame(Rect(img_width / 3, img_height/2, img_width / 3, img_height/2));
        Mat f_right = frame(Rect(2*(img_width/3), img_height/2, img_width / 3, img_height/2));

        // Get cx of each split frame
        float cx_l = cx(f_left);
        float cx_m = cx(f_mid);
        float cx_r = cx(f_right);

        cout << "cx_l " << cx_l << "\n";
        cout << "cx_m " << cx_m << "\n";
        cout << "cx_r " << cx_r << "\n";

        // If there is a contour on the left
        
        if (cx_l)
        {
            cout << "contour left\n";
            move(-10,100);
        }
        // If there is a contour in the middle
        else if (cx_m)
        {
            cout << "contour middle\n";
            // Get center pixel value of image
            float c = img_width / 2;
            // If the the contour is to the right of center
            if (cx_m < c)
            {
                cx_m = cx_m - c;
                float r = (cx_m / c) * 100;
                cout << "contour mid right\n";
                move(100, r);
            }
            // If the the contour is to the left of center
            else
            {
                float l = (cx_m / c) * 100;
                cout << "contour mid left\n";
                move(l, 100);
            }

        }
        // If there is a contour on the right
        else if (cx_r)
        {
            cout << "contour right\n";
            move(100, -10);
        }
        // No contours
        else
        {
            cout << "contour none\n";
            move(100, -20);
        }

        // temppppppppppppppppppppppppppppppppppppppppppppppp
        
        largest_contour_index = 0;
        largest_area = 0;  

        // Turn  frame grey
        Mat grey;
        cvtColor(f_left, grey, COLOR_BGR2GRAY);

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
        Mat contoured_f_left = f_left.clone();
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
            drawContours(contoured_f_left, contours, largest_contour_index, Scalar(0, 255, 0), 1);
            Moments m = moments(contours[largest_contour_index]);

            float cx = m.m10 / m.m00;

            // Draw line onto biggest contour
            line(contoured_f_left, Point(cx, 0), Point(cx, 720), Scalar(255, 0, 0), 1);

        }
        largest_contour_index = 0;
        largest_area = 0;  

        // Turn  frame grey
        cvtColor(f_mid, grey, COLOR_BGR2GRAY);

        // Blur greyed frame
        GaussianBlur(grey, blur, Size(9, 9), 0);

        // Make greyed frame b/w
        threshold(blur, threshold_img, threshold_min, threshold_max, THRESH_BINARY_INV);

        // Clone frame to be manipulated
        Mat contoured_f_mid = f_mid.clone();
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
            drawContours(contoured_f_mid, contours, largest_contour_index, Scalar(0, 255, 0), 1);
            Moments m = moments(contours[largest_contour_index]);

            float cx = m.m10 / m.m00;

            // Draw line onto biggest contour
            line(contoured_f_mid, Point(cx, 0), Point(cx, 720), Scalar(255, 0, 0), 1);

        }
        largest_contour_index = 0;
        largest_area = 0; 

        // Turn  frame grey
        cvtColor(f_right, grey, COLOR_BGR2GRAY);

        // Blur greyed frame
        GaussianBlur(grey, blur, Size(9, 9), 0);

        // Make greyed frame b/w
        threshold(blur, threshold_img, threshold_min, threshold_max, THRESH_BINARY_INV);


        // Clone frame to be manipulated
        Mat contoured_f_right = f_right.clone();
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
            drawContours(contoured_f_right, contours, largest_contour_index, Scalar(0, 255, 0), 1);
            Moments m = moments(contours[largest_contour_index]);

            float cx = m.m10 / m.m00;

            // Draw line onto biggest contour
            line(contoured_f_right, Point(cx, 0), Point(cx, 720), Scalar(255, 0, 0), 1);

        }


        // temppppppppppppppppppppppppppppppppppppppppppppppp

        // Display frames
        imshow("l", f_left);
        imshow("m", f_mid);
        imshow("r", f_right);

        imshow("lc", contoured_f_left);
        imshow("mc", contoured_f_mid);
        imshow("rc", contoured_f_right);

        using namespace this_thread; // sleep_for, sleep_until
        using namespace chrono; // nanoseconds, system_clock, seconds

        sleep_for(nanoseconds(10)); // wait 10000 nano seconds otherwise we refresh faster than frame rate
        sleep_until(system_clock::now() + nanoseconds(10000)); // change to 10000

        if (waitKey(5) >= 0)
            break;
    }
    return;
}
bool valid(float distance)
{
    cout << "\n dist:" << distance;
    if(0.1 < distance)
    {
        cout << "\n yes";
        return(true);
    }
    else
    {
        return(false);
    }
}

float angle(int idx)
{
    if(idx == 0)
    {
        return(left_angle);
    }
    if(idx == 1)
    {
        return(right_angle);
    }
    else
    {
        return(0);
    }
}

float map(float min_1, float max_1, float min_2, float max_2, float x)
{ 
    // min_1 and max_1 is motor scale
    // min_2 and max_2 is theta scale
    return( (min_1-max_1) * ( (x-min_2) / (max_2-min_2) ) + max_1 );
}

void follow()
{
    while (true)
    {
        using namespace this_thread; // sleep_for, sleep_until
        using namespace chrono; // nanoseconds, system_clock, seconds

        sleep_for(nanoseconds(10)); // wait 10000 nano seconds otherwise we refresh faster than frame rate
        sleep_until(system_clock::now() + nanoseconds(10000)); // change to 10000

        out.update(out); // Update the struct classs with current sensor data

        float distances[3] = { out.s_left_distance , out.s_middle_distance , out.s_right_distance }; // store distance values into common array
        const int distances_size = sizeof(distances) / sizeof(int); // find size of N as bytes to store integer varies by system
        int idx = distance(distances, max_element(distances, distances + distances_size));
        float left = distances[0];
        float straight = distances[1];
        float right = distances[2];
        /*cout << "\nleft: " << left;
        cout << "\nstraight: " << straight;
        cout << "\nright: " << right;*/

        float left_motor_speed = 0;
        float right_motor_speed = 0;

        /*
        for(int i = 0; i < 3; i++)
        {
            cout << i << ":" << distances[i];
        }*/

        /* Logic following mode;
            - If we have 3 valid distances find the smallest and get its angle
            - If we have 2 distances (centre and side) find the angle of the enemy
            - If we have 1 distance use its angle
            - If we have no distances just spin around
        */

        // 3 valid distances
        if(valid(left) == true && valid(straight) == true && valid(right) == true)
        {
            cout << "\n3 distances";
            int idx = distance(distances, min_element(distances, distances + distances_size)); // finds index of minimum distance in array
            float theta = angle(idx); 
            if(idx == 0)     
            {
                right_motor_speed = 100;
                left_motor_speed = map(-50, 100, 0, 50, theta);
            }
            else if(idx == 1)
            {
                right_motor_speed = map(-50, 100, 0, 50, theta);
                left_motor_speed = 100;
            }
            else
            {
                right_motor_speed = 100;
                left_motor_speed = 100;
            }
            
            cout << "\ntheta" << theta;
            move(left_motor_speed, right_motor_speed);
        }        
        // If 2 distances (centre and side)
        else if(valid(straight) == true && valid(left) == true || valid(straight) == true && valid(right) == true) // If centre is valid
        {
            cout << "\n3 2 dist";
            if(valid(left)) // If left valid 
            {
                // convert angle to positive
                float distance_angle = left_angle * -1;
                // x is distance between straight and left
                float x = sqrt( pow(straight, 2) + pow(left, 2) -2 * straight * left * cos(distance_angle));
                // y is straight to centre of x
                float y = 0.5 * x;
                // theta2 is angle between y and straight
                float theta2 = asin( (left * sin(distance_angle)) / (x) );
                // z from centre of x to theta angle
                float z = sqrt( pow(straight, 2) + pow(y, 2) -2 * straight * y * cos(theta2));
                // angle from centre
                float theta = acos( (pow(z, 2) + pow(straight, 2) - pow(y, 2)) / (2 * straight * z) );
                right_motor_speed = 100;
                left_motor_speed = map(-50, 100, 0, 50, theta);
                
            }
            else if(valid(right)) // If right valid 
            {
                // convert angle to positive
                float distance_angle = right_angle;
                // x is distance between straight and right
                float x = sqrt( pow(straight, 2) + pow(right, 2) -2 * straight * left * cos(distance_angle));
                // y is straight to centre of x
                float y = 0.5 * x;
                // theta2 is angle between y and straight
                float theta2 = asin( (left * sin(distance_angle)) / (x) );
                // z from centre of x to theta angle
                float z = sqrt( pow(straight, 2) + pow(y, 2) -2 * straight * y * cos(theta2));
                // angle from centre
                float theta = acos( (pow(z, 2) + pow(straight, 2) - pow(y, 2)) / (2 * straight * z) );
                right_motor_speed = map(-50, 100, 0, 50, theta);
                left_motor_speed = 100;
            }
            move(left_motor_speed, right_motor_speed);
        }
        // If 1 distance is valid (bias left side)
        else
        {
            cout << "\n1 distance";                 
            if (idx == 0 && out.s_left_distance != 0)
            {
                cout << out.s_left_distance << "\n"; // Print chosen index value
                move(-20, 100);           
                cout << "\n-20,100"; 
            }
            else if (idx == 1 && out.s_right_distance != 0)
            {
                cout << out.s_right_distance << "\n"; // Print chosen index value
                move(100, -20);
                cout << "\n100,-20";
            }
            else if(out.s_middle_distance != 0)
            {
                cout << out.s_middle_distance << "\n"; // Print chosen index value
                move(100, 100);
                cout << "\n100,100";
            }
            else
            {
                cout << "\n100,-100";
                move(100, -100);
            }   
        }                

    }
    
}

//
// Sumo mode
//

void sumo_mode()
{
    if (true)
    {

    }
}

//
// Flip command
//

bool flip()
{
    /*
    move(100,100);
    sleep(1);
    move(-100,-100);
    sleep(1);
    move(0,0);
    sleep(1);
    move(-100,-100);
    sleep(1);
    move(100,100);
    sleep(1);
    move(0,0);
    return(true);
    */
    return(true);
}

//
// Testing mode
//

void testing()
{
    try
	{
        float speed = 0;
		while (true)
		{
            out.update(out);
            cout << "\nleft dist" << out.s_left_distance << "\n";
            cout << "middle left dist" << out.s_middle_left_distance << "\n";
            cout << "middle dist" << out.s_middle_distance << "\n";
            cout << "middle right dist" << out.s_middle_right_distance << "\n";
            cout << "right dist" << out.s_right_distance << "\n";
            cout << "left line" << out.s_left_line << "\n";
            cout << "right line" << out.s_right_line << "\n";
            cout << "button 1" << out.s_button_1 << "\n";
            cout << "button 2" << out.s_button_2 << "\n";

			string input;
			cout << "d increase, a decrease, s idle, f to flip ";
			//cin >> input;
			if (input == "d")
			{
				speed = speed + 15;
			}
			else if (input == "a")
			{
				speed = speed - 15;
			}
			else if (input == "s")
			{
				speed = 0;
			}
            else if (input == "f")
            {
                flip();
                speed = 0;
            }
			else
			{
				speed = 0;
				stop_motors();
				//throw("Bad input");
			}
            move(speed, speed);
		}
	}
	catch (const std::exception&)
	{
		stop_motors();
	}
}

//
// Main loop 
//

int main()
{     
    gpioInitialise();
    // Arm motors & calibrate if required
    start_motors();
    /* update sonars 20 times a second, timer #0 */

    gpioSetTimerFunc(0, 50, sonarTrigger); /* every 50ms */

    /* monitor sonar echos */

    gpioSetAlertFunc(LEFT_DISTANCE_PIN_ECHO, sonarEcho);
	gpioSetAlertFunc(MIDDLE_LEFT_DISTANCE_PIN_ECHO, sonarEcho);
	gpioSetAlertFunc(MIDDLE_DISTANCE_PIN_ECHO, sonarEcho);
	gpioSetAlertFunc(MIDDLE_RIGHT_DISTANCE_PIN_ECHO, sonarEcho);
	gpioSetAlertFunc(RIGHT_DISTANCE_PIN_ECHO, sonarEcho);	 
    //maze = true;
    //mode_maze();   
    out.update(out);
    //follow();
    testing();    

    return 0;
}
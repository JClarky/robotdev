/**
 * @file simulation_interface.cpp 
 * @author Jayden Clark
 * 
 * @brief 
 * This code is the interface for the unity simulator
 * It uses JSON files as inputs/outputs for the sim
 * Sim Hardware
 *      Camera
 * 		Line sensors
 * 		Distance sensors
 * 		Motors 
 * 
 * @version 0.1
 * @date 2021-02-25 * 
 * @copyright Copyright (c) 2021
 * 
 */

// General libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include "main.h"

// Namespaces 
using namespace std;


/*			        			*/
/*			Definitions			*/
/*			        			*/

/* Move function; controls motor speeds */
void move(float left_motor, float right_motor)
{
    Json::Value root;
    root["left_motor"] = left_motor;
    root["right_motor"] = right_motor;

    ofstream input_file;
    input_file.open("C:/Users/jayde/OneDrive/Documents/Code/Robot_Development/robotdev/simulation/input.json");
    input_file << root;
    input_file.close();
}

/* Stop function; shuts off motors */
void stop_motors()
{
    move(0, 0);
}

/* Arm function; pretends to arm */
void start_motors()
{
    move(0, 0);
}

/* Output class update function; sim sensor update */
void Output::update(Output& out)
{
    try
    {
        Json::Value root2;
        ifstream output_file;
        output_file.open("C:/Users/jayde/OneDrive/Documents/Code/Robot_Development/robotdev/simulation/output.json");
        output_file >> root2;
        output_file.close();

        out.gamepad_left_x = root2["gamepad_left_x"].asFloat();
        out.gamepad_left_y = root2["gamepad_left_y"].asFloat();

        out.gamepad_right_x = root2["gamepad_right_x"].asFloat();
        out.gamepad_right_y = root2["gamepad_right_y"].asFloat();

        out.s_left_distance = root2["s_left_distance"].asFloat();
        out.s_middle_distance = root2["s_middle_distance"].asFloat();
        out.s_right_distance = root2["s_right_distance"].asFloat();

        out.s_left_line = root2["s_left_line"].asFloat();
        out.s_right_line = root2["s_right_line"].asFloat();

    }
    catch (exception& e)
    {
        // cout << e.what() << "\n";
    }
}




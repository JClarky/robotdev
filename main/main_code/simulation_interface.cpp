#include <iostream>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include "simulation_interface.h"

using namespace std;


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




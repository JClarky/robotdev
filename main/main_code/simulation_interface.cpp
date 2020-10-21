#include <iostream>
#include <fstream>
#include <sstream>
#include <jsoncpp/json/json.h>

using namespace std;

// NAME
// Simulation Interface

// DESCRIPTION
// Interface for the simulator

class output
{
    public:
        float s_left_line;
        float s_right_line;

        float s_left_distance;
        float s_middle_distance;
        float s_right_distance;

        float gamepad_right_y;
        float gamepad_right_x;

        float gamepad_left_y;
        float gamepad_left_x;
};

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

void update()
{
    Json::Value root2;
    ifstream output_file;
    output_file.open("C:/Users/jayde/OneDrive/Documents/Code/Robot_Development/robotdev/simulation/output.json");
    output_file >> root2;
    output_file.close();

    output out;

    out.gamepad_left_x = root2["gamepad_left_x"].asFloat();
    out.gamepad_left_y = root2["gamepad_left_y"].asFloat();

    out.gamepad_right_x = root2["gamepad_right_x"].asFloat();
    out.gamepad_right_y = root2["gamepad_right_y"].asFloat();

    out.s_left_distance = root2["s_left_distance"].asFloat();
    out.s_middle_distance = root2["s_middle_distance"].asFloat();
    out.s_right_distance = root2["s_right_distance"].asFloat();

    out.s_left_line = root2["s_left_line"].asFloat();
    out.s_right_line = root2["s_right_line"].asFloat();

    //cout << out;
}

// temp main

int main()
{
    while(true)
    {
        float l;
        float r;

        cout << "left motor:";
        cin >> l;

        cout << "right motor:";
        cin >> r;

        update();
        move(l, r);
    }    
}

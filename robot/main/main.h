/**
 * @file main.h 
 * @author Jayden Clark
 * 
 * @brief 
 * This code is the declerations for all shared functions
 * 
 * @version 0.3
 * @date 2021-02-26 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef MAIN_H 
#define MAIN_H

void move(float left_motor, float right_motor);

void stop_motors();

void start_motors();


class Output
{
    public:
        float s_left_line;
        float s_right_line;

        float s_button_1;
        float s_button_2;

        float s_left_distance;
        float s_middle_left_distance;
        float s_middle_distance;
        float s_middle_right_distance;
        float s_right_distance;

        float gamepad_right_y;
        float gamepad_right_x;

        float gamepad_left_y;
        float gamepad_left_x;

    void update(Output& out);
};

#endif
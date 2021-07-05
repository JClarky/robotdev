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

void sonarEcho(int gpio, int level, uint32_t tick);

void sonarTrigger(void);

// Left distance sensor
#define LEFT_DISTANCE_PIN_ECHO      17
#define LEFT_DISTANCE_PIN_TRIGGER    27
float left_distance = 0;

// Middle left distance sensor
#define MIDDLE_LEFT_DISTANCE_PIN_ECHO      18
#define MIDDLE_LEFT_DISTANCE_PIN_TRIGGER    23
float middle_left_distance = 0;

// Middle distance sensor
#define MIDDLE_DISTANCE_PIN_ECHO      24
#define MIDDLE_DISTANCE_PIN_TRIGGER    25
float middle_distance = 0;

// Middle right distance sensor
#define MIDDLE_RIGHT_DISTANCE_PIN_ECHO      11
#define MIDDLE_RIGHT_DISTANCE_PIN_TRIGGER    8
float middle_right_distance = 0;

// Right distance sensor
#define RIGHT_DISTANCE_PIN_ECHO      7
#define RIGHT_DISTANCE_PIN_TRIGGER    1
float right_distance = 0;

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
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

        float s_left_distance;
        float s_middle_distance;
        float s_right_distance;

        float gamepad_right_y;
        float gamepad_right_x;

        float gamepad_left_y;
        float gamepad_left_x;

    void update(Output& out);
};

#endif
#ifndef HARDWARE_INTERFACE.H
#define HARDWARE_INTERFACE.H

bool initalise();

void stop();

void move(float left, float right);

void arm();

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

    void update(output& out)
    {
		out.gamepad_left_x = 1;
		out.gamepad_left_y = 1;

		out.gamepad_right_x = 1;
		out.gamepad_right_y = 1;

		out.s_left_distance = 1;
		out.s_middle_distance = 1;
		out.s_right_distance = 1;

		out.s_left_line = 1;
		out.s_right_line = 1;
    }
};

#endif
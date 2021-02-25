#include <pigpio.h>
#include <unistd.h>
#include <iostream>
#include "hardware_interface.h"

using namespace std;

#define LEFT_MOTOR_PIN      12
#define RIGHT_MOTOR_PIN     13

#define MAX_THROTTLE        1960  
#define MIN_THROTTLE		1040
#define CENTER_THROTTLE		1500
#define ARM_THROTTLE		1400

//g++ -o main run.cpp -lpigpio

bool initalise_gpio()
{
	if (gpioInitialise() > 0)
	{
		// init fail
		return(false);
	}
	else
	{
		// init success
		return(true);
	}
}

void stop()
{
	gpioServo(LEFT_MOTOR_PIN, 0);
	gpioServo(RIGHT_MOTOR_PIN, 0);
}

void move(float left, float right) // value from -100 to 100
{
	float upper_range = MAX_THROTTLE - CENTER_THROTTLE;
	float lower_range = CENTER_THROTTLE - MIN_THROTTLE;

	float l_percentage = left / 100;
	float r_percentage = right / 100;

    float l_value;
    float r_value;

	if (l_percentage > 0)
	{
		l_value = l_percentage * upper_range;
		l_value = l_value + CENTER_THROTTLE;
	}
	else
	{
		l_value = l_percentage * lower_range;
		l_value = l_value + CENTER_THROTTLE;
	}

	if (r_percentage > 0)
	{
		r_value = r_percentage * upper_range;
		r_value = r_value + CENTER_THROTTLE;
	}
	else
	{
		r_value = r_percentage * lower_range;
		r_value = r_value + CENTER_THROTTLE;
	}

	gpioServo(LEFT_MOTOR_PIN, (int)l_value);
	gpioServo(RIGHT_MOTOR_PIN, (int)r_value);
    cout << l_value;
}

void arm()
{
	gpioServo(LEFT_MOTOR_PIN, ARM_THROTTLE);
	gpioServo(RIGHT_MOTOR_PIN, ARM_THROTTLE);
	sleep(2);
	gpioServo(LEFT_MOTOR_PIN, CENTER_THROTTLE);
	gpioServo(RIGHT_MOTOR_PIN, CENTER_THROTTLE);
}

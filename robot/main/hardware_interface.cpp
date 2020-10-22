#include <pigpio.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define LEFT_MOTOR_PIN      14
#define RIGHT_MOTOR_PIN     15

#define MAX_THROTTLE        1960  
#define MIN_THROTTLE		1040
#define CENTER_THROTTLE		1500

//g++ -o main run.cpp -lpigpio

bool initalise()
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
	gpioServo(LEFT_MOTOR_PIN, CENTER_THROTTLE);
	gpioServo(RIGHT_MOTOR_PIN, CENTER_THROTTLE);
	sleep(2);
}

int main()
{
	float speed = 0;
    initalise();
    arm();
	try
	{
		while (true)
		{
			string input;
			cout << "d to increase, a to decrease, s to idle: ";
			cin >> input;
			if (input == "d")
			{
				//speed = speed + 15;
                move(15,-15);
			}
			else if (input == "a")
			{
				//speed = speed - 15;
                move(-15,15);
			}
            else if (input == "w")
			{
				move(100,100);
			}
			else if (input == "s")
			{
				move(-10,-10);
			}
			else if (input == "f")
			{
				move(0,0);
			}
			else
			{
				throw("Bad input");
			}
            //cout << speed;
            //move(speed, speed);
		}
	}
	catch (const std::exception&)
	{
		stop();
		return 0;
	}
}
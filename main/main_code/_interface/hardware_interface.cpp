/**
 * @file hardware_interface.cpp 
 * @author Jayden Clark
 * 
 * @brief 
 * This code is the interface for all hardware on the PI.
 * Hardware
 * 		Camera
 * 		Line sensors
 * 		Distance sensors
 * 		Motors 
 * 
 * @version 1
 * @date 2021-02-25 * 
 * @copyright Copyright (c) 2021
 * 
 */

// General libraries
#include <pigpio.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include<cmath>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// Header files
#include "main.h"

// Namespaces
using namespace std;

// Variables definitions
#define LEFT_MOTOR_PIN      12
#define RIGHT_MOTOR_PIN     13

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

// Line sensors
#define LEFT_LINE_PIN    20
#define RIGHT_LINE_PIN   21

// Buttons
#define BUTTON_1_PIN   0
#define BUTTON_2_PIN   5

// ESC throttle values
#define MAX_THROTTLE        1960  
#define MIN_THROTTLE		1040
#define CENTER_THROTTLE		1500
#define ARM_THROTTLE		1400

void sonarTrigger(void);

void sonarEcho(int gpio, int level, uint32_t tick);

bool initalise_gpio()
{
	
	if (gpioInitialise() < 0)
	{
		// init fail
		cout << "failure";
		return(false);
		
	}

	// Distance sensors
	gpioSetMode(LEFT_LINE_PIN, PI_INPUT);
	gpioSetMode(RIGHT_LINE_PIN, PI_INPUT);

	// Buttons
	gpioSetMode(BUTTON_1_PIN, PI_INPUT);
	gpioSetMode(BUTTON_2_PIN, PI_INPUT);
	
	// Left distance sensor
	gpioSetMode(LEFT_DISTANCE_PIN_ECHO, PI_OUTPUT); 
	gpioSetMode(LEFT_DISTANCE_PIN_TRIGGER, PI_INPUT);
	gpioWrite(LEFT_DISTANCE_PIN_TRIGGER, PI_OFF);

	// Middle left distance sensor
	gpioSetMode(MIDDLE_LEFT_DISTANCE_PIN_ECHO, PI_OUTPUT); 
	gpioSetMode(MIDDLE_LEFT_DISTANCE_PIN_TRIGGER, PI_INPUT);
	gpioWrite(MIDDLE_LEFT_DISTANCE_PIN_TRIGGER, PI_OFF);

	// Middle distance sensor
	gpioSetMode(MIDDLE_DISTANCE_PIN_ECHO, PI_OUTPUT); 
	gpioSetMode(MIDDLE_DISTANCE_PIN_TRIGGER, PI_INPUT);
	gpioWrite(MIDDLE_DISTANCE_PIN_TRIGGER, PI_OFF);

	// Middle right distance sensor
	gpioSetMode(MIDDLE_RIGHT_DISTANCE_PIN_ECHO, PI_OUTPUT); 
	gpioSetMode(MIDDLE_RIGHT_DISTANCE_PIN_TRIGGER, PI_INPUT);
	gpioWrite(MIDDLE_RIGHT_DISTANCE_PIN_TRIGGER, PI_OFF);

	// Right distance sensor
	gpioSetMode(RIGHT_DISTANCE_PIN_ECHO, PI_OUTPUT); 
	gpioSetMode(RIGHT_DISTANCE_PIN_TRIGGER, PI_INPUT);
	gpioWrite(RIGHT_DISTANCE_PIN_TRIGGER, PI_OFF);

	/* update sonars 20 times a second, timer #0 */

    gpioSetTimerFunc(0, 50, sonarTrigger); /* every 50ms */

    /* monitor sonar echos */

    gpioSetAlertFunc(LEFT_DISTANCE_PIN_ECHO, sonarEcho);
	gpioSetAlertFunc(MIDDLE_LEFT_DISTANCE_PIN_ECHO, sonarEcho);
	gpioSetAlertFunc(MIDDLE_DISTANCE_PIN_ECHO, sonarEcho);
	gpioSetAlertFunc(MIDDLE_RIGHT_DISTANCE_PIN_ECHO, sonarEcho);
	gpioSetAlertFunc(RIGHT_DISTANCE_PIN_ECHO, sonarEcho);	 

	return(true);
}

void arm()
{
	gpioServo(LEFT_MOTOR_PIN, ARM_THROTTLE);
	gpioServo(RIGHT_MOTOR_PIN, ARM_THROTTLE);
	sleep(2);
	gpioServo(LEFT_MOTOR_PIN, CENTER_THROTTLE);
	gpioServo(RIGHT_MOTOR_PIN, CENTER_THROTTLE);
}

/*			        			*/
/*			Definitions			*/
/*			        			*/

void sonarTrigger(void)
{
    /* trigger a sonar reading */

	gpioInitialise();

    gpioWrite(LEFT_DISTANCE_PIN_TRIGGER, PI_ON);
	gpioDelay(10); /* 10us trigger pulse */
	gpioWrite(LEFT_DISTANCE_PIN_TRIGGER, PI_OFF);
	
	gpioDelay(100);

    gpioWrite(MIDDLE_LEFT_DISTANCE_PIN_TRIGGER, PI_ON);
	gpioDelay(10); /* 10us trigger pulse */
	gpioWrite(MIDDLE_LEFT_DISTANCE_PIN_TRIGGER, PI_OFF);

	gpioDelay(100);

	gpioWrite(MIDDLE_DISTANCE_PIN_TRIGGER, PI_ON);
	gpioDelay(10); /* 10us trigger pulse */
	gpioWrite(MIDDLE_DISTANCE_PIN_TRIGGER, PI_OFF);

	gpioDelay(100);

    gpioWrite(MIDDLE_RIGHT_DISTANCE_PIN_TRIGGER, PI_ON);
	gpioDelay(10); /* 10us trigger pulse */
	gpioWrite(MIDDLE_RIGHT_DISTANCE_PIN_TRIGGER, PI_OFF);

	gpioDelay(100);

	gpioWrite(RIGHT_DISTANCE_PIN_TRIGGER, PI_ON);
	gpioDelay(10); /* 10us trigger pulse */
	gpioWrite(RIGHT_DISTANCE_PIN_TRIGGER, PI_OFF);	
	
	gpioDelay(100);
}

void sonarEcho(int gpio, int level, uint32_t tick)
{
   static uint32_t startTick, firstTick=0;	
   gpioInitialise();

   int diffTick;

   if (!firstTick) firstTick = tick;

   if (level == PI_ON)
   {
      	startTick = tick;
   }
   else if (level == PI_OFF)
   {
      	diffTick = tick - startTick;
		float distance = (diffTick * 34300) / 2 / 100000;
		if(gpio == LEFT_DISTANCE_PIN_ECHO)
		{
			left_distance = distance;
		}
		else if(gpio == MIDDLE_LEFT_DISTANCE_PIN_ECHO)
		{
			middle_left_distance = distance;
		}
		else if(gpio == MIDDLE_DISTANCE_PIN_ECHO)
		{
			middle_distance = distance;
		}
		else if(gpio == MIDDLE_RIGHT_DISTANCE_PIN_ECHO)
		{
			middle_right_distance = distance;
		}
		else if(gpio == RIGHT_DISTANCE_PIN_ECHO)
		{
			right_distance = distance;
		}

      	//printf("%u %u\n", tick-firstTick, diffTick);
   }
}

/* Start function; initalises and arms */

void start_motors()
{
	initalise_gpio();
	arm();
}

/* Stop function; shuts off motors */

void stop_motors()
{
	gpioServo(LEFT_MOTOR_PIN, 0);
	gpioServo(RIGHT_MOTOR_PIN, 0);
	gpioTerminate();
}

/* Move function; controls motor speeds */

// Curent motor speeds -100 to 100
float current_left; 
float current_right;
// Target motor speeds -100 to 100
float target_left;
float target_right;

void move(float left, float right) // value from -100 to 100
{
	// Scale motor speeds (0-50)
	left = (left/100)*50;
	right = (right/100)*50; 

	// Set global target speed
	target_left = left;
	target_right = right;

	// Calculate increment (current speed to target speed)
	
	float increment_left;
	float increment_right;
	float difference_left = target_left - current_left;
	float difference_right = target_right - current_right;

	// If it is not from current speed of 0
	// Equation increment = 100(0.95^difference)+2
	if(current_left != 0)
	{
		if(difference_left < 0)
		{
			difference_left = abs(difference_left); 
			increment_left = 100*(pow(0.95,difference_left));
			increment_left = increment_left * -1;
		}
		else
		{
			increment_left = 100*(pow(0.95,difference_left));
		}	
	}
	else
	{
		if(target_left != 0)
		{
			if(difference_left < 0)
			{
				increment_left = -2;
			}
			else
			{
				increment_left = 2;
			}	
		}
		
	}
	if(current_right != 0)
	{
		if(difference_right < 0)
		{
			difference_right = abs(difference_right); 
			increment_right = 100*(pow(0.95,difference_right));
			increment_right = increment_right * -1;
		}
		else
		{
			increment_right = 100*(pow(0.95,difference_right));
		}		
	}
	else
	{
		if(target_right != 0)
		{
			if(difference_right < 0)
			{
				increment_right = -2;
			}
			else
			{
				increment_right = 2;
			}	
		}
	}

	// Filter increments
	if(0 < increment_right < 2)
	{
		increment_right = 2;
	}
	else if(-2 < increment_right < 0)
	{
		increment_right = -2;
	}
	if(0 < increment_left < 2)
	{
		increment_right = 2;
	}
	else if(-2 < increment_left < 0)
	{
		increment_right = -2;
	}

	if(difference_left > increment_left && increment_left > 0)
	{
		increment_left = difference_left;
	}
	else if(difference_left < increment_left && increment_left < 0)
	{
		increment_left = difference_left;
	}

	if(difference_right > increment_right && increment_right > 0)
	{
		increment_right = difference_right;
	}
	else if(difference_right < increment_right && increment_right < 0)
	{
		increment_right = difference_right;
	}

	// Calaculate new speed from increment
	cout << "\nIncrement Left: "<<increment_left<<" Increment Right: "<<increment_right<<"\n";
	cout << "\ncurrent_left: "<<current_left<<" current_right: "<<current_right<<"\n";
	left = current_left + increment_left;
	right = current_right + increment_right;


	cout << "\nNew Left: "<<left<<" New Right: "<<right<<"\n";

	// Ranges for forwards and backwards
	float upper_range = MAX_THROTTLE - CENTER_THROTTLE;
	float lower_range = CENTER_THROTTLE - MIN_THROTTLE;

	// Convert move values to percenatges
	float l_percentage = left / 100;
	float r_percentage = right / 100;

	// PWM output speeds
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

	float new_value_r = CENTER_THROTTLE;
	float new_value_l = CENTER_THROTTLE;

	if(current_left == 0 && current_right == 0)
	{		
		if(left != 0)
		{
			if(left < 0)
			{
				new_value_l = new_value_l + 50;
			}
			else
			{
				new_value_l = new_value_l - 50;
			}	
		}	
		if(right != 0)
		{
			if(right < 0)
			{
				new_value_r = new_value_r + 50;
			}
			else
			{
				new_value_r = new_value_r - 50;
			}	
		}	
		//gpioServo(RIGHT_MOTOR_PIN, (int)r_value-15);
		//gpioServo(LEFT_MOTOR_PIN, (int)l_value);
		usleep(100000); // sleep 0.1s
	}
	
	//gpioServo(RIGHT_MOTOR_PIN, (int)r_value-15);
	//gpioServo(LEFT_MOTOR_PIN, (int)l_value);
	current_left = left;
	current_right = right;

	
    cout << "\nLeft motor: " << l_value << "\n";
	cout << "\nRight motor: " << r_value << "\n";
}

/* Output class update function; sensor update */

void Output::update(Output& out)
{
	out.gamepad_left_x = 1;
	out.gamepad_left_y = 1;

	out.gamepad_right_x = 1;
	out.gamepad_right_y = 1;

	/* Read distance sensors */
	

	out.s_left_distance = left_distance;
	out.s_middle_left_distance = middle_left_distance;
	out.s_middle_distance = middle_distance;
	out.s_middle_right_distance = middle_right_distance;
	out.s_right_distance = right_distance;

	out.s_left_line = gpioRead(LEFT_LINE_PIN);
	out.s_right_line = gpioRead(RIGHT_LINE_PIN);

	out.s_button_1 = gpioRead(BUTTON_1_PIN);
	out.s_button_2 = gpioRead(BUTTON_2_PIN);
}
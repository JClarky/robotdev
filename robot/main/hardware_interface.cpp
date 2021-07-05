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
	
	if (gpioInitialise() > 0)
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

    //gpioWrite(LEFT_DISTANCE_PIN_TRIGGER, PI_ON);
    //gpioWrite(MIDDLE_LEFT_DISTANCE_PIN_TRIGGER, PI_ON);
	gpioWrite(MIDDLE_DISTANCE_PIN_TRIGGER, PI_ON);
    //gpioWrite(MIDDLE_RIGHT_DISTANCE_PIN_TRIGGER, PI_ON);
	//gpioWrite(RIGHT_DISTANCE_PIN_TRIGGER, PI_ON);

   	gpioDelay(10); /* 10us trigger pulse */

	gpioWrite(LEFT_DISTANCE_PIN_TRIGGER, PI_OFF);
    gpioWrite(MIDDLE_LEFT_DISTANCE_PIN_TRIGGER, PI_OFF);
	gpioWrite(MIDDLE_DISTANCE_PIN_TRIGGER, PI_OFF);
    gpioWrite(MIDDLE_RIGHT_DISTANCE_PIN_TRIGGER, PI_OFF);
	gpioWrite(RIGHT_DISTANCE_PIN_TRIGGER, PI_OFF);
}

void sonarEcho(int gpio, int level, uint32_t tick)
{
   static uint32_t startTick, firstTick=0;	

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
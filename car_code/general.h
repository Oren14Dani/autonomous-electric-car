#ifndef GENERAL_H
#define GENERAL_H

#include <Arduino.h>
#include <math.h>
#include <SoftwareSerial.h> 
#include <MotorDriver.h>
#include "String.h"
#include "Sensor_UltraSony.h"
#include "recevier.h"
#include "vehicle_control.h"

// --------- global var ---------
extern vehicle_control vehicle;
//extern int bluetoothTx;  // TX-O pin of bluetooth mate, Arduino D46
//extern int bluetoothRx;  // RX-I pin of bluetooth mate, Arduino D44
//extern SoftwareSerial bluetooth;
extern int      step; // for bypass
extern int      state;
extern bool     turned_once; // for bypass   
extern bool     fliped_once; // for bypass   
extern String   obstacle_side; // for bypass
extern bool     finish_attachment; // for bypass
extern bool     avoid_wall; // for bypass
extern bool            timeouted; // for bypass
extern int      time_counter;
extern unsigned long delay_time_counter_0;
extern unsigned long delay_time_counter_1;

#define BUTTON_START 37

// --------- movment ---------
#define motors_status_soft_right 'D'
#define motors_status_soft_left  'A'
#define motors_status_forward    'f'
#define motors_status_reverse    'v'
#define motors_status_right      'r'
#define motors_status_left       'l'
#define motors_status_stop       's'

#define DELAY_MOTOR_RESPOND 350
#define M_SPEED 100
#define MAX_SPEED 255 // change to 255


// --------- signal ---------
#define MINIMAL_PULSE_LEN 200 
#define MINIMUM_CORRECT_ERROR_MEAN 6
#define TEST_NUMBER 4
//#define ERROR_SAMPLE_OFFSET 60
#define SAMPLE_NUMBER 30
#define SIGNAL_TIME_OUT 15 // msec

#define AVG_PULSE_LEN 1000 // 1000 usec = 1 msec
#define ERROR_PULSE_LEN 100
#define RETURN_CURR_MEAN 1

// --------- ultra sony ---------
#define CENTER_ALERT_DISTANCE 45
#define LEFT_ALERT_DISTANCE   15
#define RIGHT_ALERT_DISTANCE  15
#define BACK_ALERT_DISTANCE   15

#define ALERT_DISTANCE_ERROR 5
#define MAX_DIS 300

#define US_echo_left    34
#define US_trig_left    32
#define US_vcc_left     30
#define US_LED_left     49

#define US_echo_center  35
#define US_trig_center  33
#define US_vcc_center   31
#define US_LED_center   51

#define US_echo_right   43
#define US_trig_right   41
#define US_vcc_right    39
#define US_LED_right    53

#define US_gnd_back     44
#define US_echo_back    46
#define US_trig_back    48
#define US_vcc_back     50
#define US_LED_back     40

// --------- reciver ---------
#define resiver_REDLED 42
#define resiver_GND 24
#define resiver_VCC 26


// --------- main states ---------
#define IDLE            1 // "idel"
#define SEARCH_SIGNAL   2 // "explor"
#define VALIDATE_SIGNAL 3 // "sync signal"
#define BYPASS_OBSTACLE 4 // "bypass obstical"
#define DRIVE           5 // "drive"

// --------- bypass ---------
#define PASS_CORNER_DISTANCE    30
#define TURN_DISTANCE_ERROR     10
#define SAFE_OBSTACLE_DISTANCE  20
#define ALPHA                   35000 
#define MIN_DELAY               30
#define TIMEOUT_BYPASS          4000
#define _90deg_motor_delay      ALPHA * (1 / (0.8 * M_SPEED)) + MIN_DELAY //[ms] //// 467
#define pass_corner_delay       (PASS_CORNER_DISTANCE / (0.8 * M_SPEED)) * 1000 // t = x/v  [ms] //// 375



//  bypass step states: 
#define START_BYPASS   1
#define DEAD_END       2
#define FRONT_OBSTACLE 3
#define RIGHT_OBSTACLE 4
#define LEFT_OBSTACLE  5
#define BACK_OBSTACLE  7
#define FLIP           8
#define STOP_BYPASS    9


// --------- dircetion ---------
#define FOR_WARD "forward"
#define LEFT "left"
#define RIGHT "right"




#endif
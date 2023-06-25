	
#include "general.h"

void Sensor_UltraSony::set_paramters(int pin_echo_new, int pin_trig_new,int pin_vcc_new,
										int led_pin_new, int new_alert_distance)
{
	pin_echo		=	pin_echo_new;
	pin_trig		=	pin_trig_new;
	pin_vcc			=	pin_vcc_new;
	led_pin			=	led_pin_new;
	alert_distance	= new_alert_distance;

	pinMode(pin_echo, INPUT);
	pinMode(pin_trig, OUTPUT);
	pinMode(pin_vcc, OUTPUT);
	pinMode(led_pin, OUTPUT);
    
	digitalWrite(pin_vcc, HIGH);
	digitalWrite(led_pin, LOW);
	
	distance_cm = 0;
	pulse_len_microsecond = 0;
	obstacle=false;				
}

bool Sensor_UltraSony:: sample_space()
{
	give_a_glimpse();
	if (distance_cm < alert_distance)
	{
		obstacle = true;
		digitalWrite(led_pin, HIGH); // turn on obstacle led
	}
	else
	{
		obstacle = false;
		digitalWrite(led_pin, LOW); // turn off obstacle led
	}
	return obstacle;
}
unsigned int& Sensor_UltraSony:: get_distance_cm() { return  distance_cm;}
bool Sensor_UltraSony:: get_obstacle(){ return obstacle;}

//measure distance
unsigned int& Sensor_UltraSony:: give_a_glimpse()
{
	digitalWrite(pin_trig, LOW);
	delayMicroseconds(5); // setup signal for 5usec
	digitalWrite(pin_trig, HIGH);
	delayMicroseconds(10); // hold signal for 10usec
	digitalWrite(pin_trig, LOW);
	pulse_len_microsecond = pulseIn(pin_echo, HIGH);

	microsecondsToCentimeters(); // save result in distance_cm fild
	delayMicroseconds(50); // avoid of repeting less then 50 us

	return distance_cm;
}



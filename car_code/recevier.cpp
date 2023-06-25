#include "general.h"

void recevier:: set_paramters(int pin_input_set)
{
	pin_input=pin_input_set;
	pinMode(pin_input, INPUT);
	pinMode(resiver_GND, OUTPUT);
	pinMode(resiver_VCC, OUTPUT);
	pinMode(resiver_REDLED, OUTPUT);
    digitalWrite(resiver_GND, LOW); 
	digitalWrite(resiver_VCC, HIGH); 
	digitalWrite(resiver_REDLED, LOW);
	_seen_signal = false;
}

void recevier:: reset_receiver() {_seen_signal= false;}

double recevier::calc_pulse_frequency()
{
	//int counter = 0;
	unsigned long time_tmp; // to mesure when reached to SIGNAL_TIME_OUT
	unsigned long time_0;
	unsigned long time_1;
	unsigned long time_2;
	int voltage_level;
	int first_voltage_level;

	/*
    .time_tmp
	         ___         
	________|   |________
	        ^time_0
	
	*/

	// check the first voltage level so we can start sample also when LOW or HIGH.
	if (digitalRead(pin_input) == LOW)
		voltage_level = LOW;
	else
		voltage_level = HIGH;
	first_voltage_level = voltage_level;

	//wait for signal to change or time out
	time_tmp = millis();
	while (digitalRead(pin_input) == voltage_level)
	{
		delay(1);
		if (millis() - time_tmp > SIGNAL_TIME_OUT) // time out
		{
			//Serial.print("timeout0\n");
			frequency = 0;
			signal_len_millisecond = 0;
			return 0;
		}
	}
	//save the time
	time_0 = millis();

	//swap the voltage level
	if (digitalRead(pin_input) == LOW)
		voltage_level = LOW;
	else
		voltage_level = HIGH;

	//wait for signal to change again
	time_tmp = millis();
	while (digitalRead(pin_input) == voltage_level)
	{
		delay(1);
		if (millis() - time_tmp > SIGNAL_TIME_OUT)
		{
			//Serial.print("timeout1\n");
			frequency = 0;
			signal_len_millisecond = 0;
			return 0;
		}
	}
	//save the time
	time_1 = millis();

	//swap the voltage level
	if (digitalRead(pin_input) == LOW)
		voltage_level = LOW;
	else
		voltage_level = HIGH;


	//wait for signal to change again
	time_tmp = millis();
	while (digitalRead(pin_input) == voltage_level)
	{
		delay(1);
		if (millis() - time_tmp > SIGNAL_TIME_OUT)
		{
			//Serial.print("timeout2\n");
			frequency = 0;
			signal_len_millisecond = 0;
			return 0;
		}
	}
	//save the time
	time_2 = millis();


	unsigned long time_on, time_off;
	if (first_voltage_level == LOW)
	{
	/*
		   time_on
			___ -------time_off---- ___                     ___
		___|   |___________________|   |___________________|   |__
			^   ^time_1             ^time_2
		time_0
	*/

		time_on = time_1 - time_0;
		time_off = time_2 - time_1;
	}
	else
	{
	/*
		        			  time_on
		___ -------time_off---- ___                     ___
	       |___________________|   |___________________|   |__
		   ^                   ^time_1             
		time_0                     ^time_2
	*/
		time_off = time_1 - time_0;
		time_on = time_2 - time_1;
	}


	double dutycycle;

	dutycycle = time_on / (1.0 * (time_on + time_off));
	frequency = (1.0 / (time_on + time_off)) * 1000.0;
	signal_len_millisecond = time_on + time_off;

	
	/*
	Serial.print("signal_len_microsecond is :");
	Serial.print(signal_len_millisecond);
	Serial.print(" frequency is :");
	Serial.print(frequency);
	Serial.print(" dutycycle is :");
	Serial.print(dutycycle);
	Serial.print("\n");
	
	*/
	

	return frequency;
}
double recevier::devider_calc_pulse_frequency()
{
	int counter_atteptes = 0;
	unsigned long time_on = 0, time_off = 0;
	double dutycycle;

	/*
			------time_off-----	    ------time_off-----
		___                     ___                     ___
	___|   |___________________|   |___________________|   |__
		^^^                     ^^^
		time_on                 time_on              
	*/

		
	//waits for the pin to go from LOW to HIGH, starts timing, then waits for the pin to go LOW and stops timing
	
	time_on = pulseIn(pin_input, HIGH, SIGNAL_TIME_OUT);

	delay(15);

	//waits for the pin to go from HIGH to LOW, starts timing, then waits for the pin to go HIGH and stops timing
	
	time_off = pulseIn(pin_input, LOW, SIGNAL_TIME_OUT);

	Serial.print("time on:");
	Serial.print(time_on);
	Serial.print("\n");

	Serial.print("time_off:");
	Serial.print(time_off);
	Serial.print("\n");



	dutycycle = time_on / (1.0 * (time_on + time_off));

	Serial.print("dutycycle:");
	Serial.print(dutycycle);
	Serial.print("\n");

	if (time_on != 0 && time_off != 0)
		frequency = (1.0 / (time_on + time_off)) * 1000000.0;
	else
		frequency = 0;

	// Serial.print("frequency:");
	// Serial.print(frequency);
	// Serial.print("\n");

	signal_len_millisecond = time_on + time_off;
	
	return frequency;
}


unsigned long& recevier:: calc_pulse_len_microsecond()
{
	signal_len_millisecond = pulseIn(pin_input, LOW, SIGNAL_TIME_OUT); // wait for signal get from high to low
	return signal_len_millisecond;
}
bool& recevier:: seen_signal() {return _seen_signal;}
int recevier:: get_pin_input() {return pin_input;}

unsigned long& recevier:: get_pulse_len_microsecond() {return signal_len_millisecond;}

bool recevier:: check_if_there_signal()
{
    time_counter=0;
	int mesure_error = 5;
	int num = 10;
	//calc_pulse_frequency();
	double mean_frequency = 0;
	for (int i = 0; i < num; i++)
	{
		calc_pulse_frequency();
		mean_frequency += frequency;
	}
	mean_frequency = mean_frequency / num;

	// Serial.print("mean_frequency:");
	// Serial.print(mean_frequency);
	// Serial.print("\n");

	if (mean_frequency < 100 + mesure_error && mean_frequency>100 - mesure_error || mean_frequency < 200 + mesure_error && mean_frequency>200 - mesure_error)
	{
		digitalWrite(resiver_REDLED, HIGH);
		_seen_signal = true;
		return true; // it`s ok, there is a signal ! go to sync it now.
	}
	else
	{
		digitalWrite(resiver_REDLED, LOW);
		_seen_signal = false;
		return false;
	}

	
	
}

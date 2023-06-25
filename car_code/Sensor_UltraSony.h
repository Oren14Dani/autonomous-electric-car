#ifndef SENSOR_ULTRASONY_H
#define SENSOR_ULTRASONY_H

class Sensor_UltraSony 
{	
	private:
		int pin_echo;
		int pin_trig;
		int pin_vcc;
		int led_pin;
		int alert_distance;

		unsigned long pulse_len_microsecond;
		void microsecondsToCentimeters() 
		{
			/* The speed of sound is 340 m/s or 29 microseconds per centimeter.
			The echo travels out and back, so to find the distance of the object we
			take half of the distance travelled. */
			distance_cm = (pulse_len_microsecond / 29) / 2;
		}
	public:
		bool obstacle; //TRUE indicate alert
		unsigned int distance_cm;
		void set_paramters(int pin_echo_new, int pin_trig_new, int pin_vcc_new,
		int led_pin_new, int new_alert_distance);
		bool sample_space();
		unsigned int& get_distance_cm();
		bool get_obstacle();
		unsigned int& give_a_glimpse();
		//void turn_obstacle_led();

};
#endif
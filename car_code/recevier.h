#ifndef RECEVIER_H
#define RECEVIER_H

#define REFERENCE_WITH_SIGNAL 1000 //1036

class recevier
{
	private:
		int pin_input;
		unsigned long signal_len_millisecond;
		double frequency;
		
		bool _seen_signal;

	public:
		void set_paramters(int pin_input_set);
		bool& seen_signal();
		int get_pin_input();
		unsigned long& calc_pulse_len_microsecond();
		double calc_pulse_frequency();
		void reset_receiver();
		double devider_calc_pulse_frequency();
		bool check_if_there_signal();
	;
	unsigned long& get_pulse_len_microsecond();


};
#endif


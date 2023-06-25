#ifndef VEHICLE_CONTROL_H
#define VEHICLE_CONTROL_H
class vehicle_control
{
	private:
		int front_right = 3;
		int front_left = 4;
		int back_right = 2;
		int back_left = 1;
		
		unsigned char motor_speed = 255;
		//int _90deg_motor_delay, pass_corner_delay, motor_stop_delay ;
		char motors_status;

		MotorDriver m;
			
		Sensor_UltraSony _left_sensor,_center_sensor,_right_sensor,_back_sensor;
		recevier my_recevier;
		int mean;

			
	public:

		void vehicle_control_set_paramters();
		// ----------- recevier -----------
		recevier& _recevier();
		void search_signal();

		// ----------- ultra sony -----------
		void found_obstacle();
		Sensor_UltraSony& left_sensor();
		Sensor_UltraSony& center_sensor();
		Sensor_UltraSony& right_sensor();
		Sensor_UltraSony& back_sensor();

		// ----------- movment and motors -----------
		char& get_motors_status();
		void drive();
		void move(String direction);
        void s_reverse(int time);
		void s_moveNdelay(String side, int time);
		void speed_control();
		void forward();
		void reverse();
		void right();
		void left();
		void stop_motor();
		void fix_direction();

        /// ----- bypass--- 
        int front_obstacle_routine();
        void left_obstacle_routine();
        void right_obstacle_routine();
        void drive_along_side();
        void flip();
        void print_step(int& step);
        void bypass_next_state(int& step, bool& turned_once, bool& fliped_once);
		void s_bypass_the_obstacle();
        void print_obsacle_cm();


};
	
#endif

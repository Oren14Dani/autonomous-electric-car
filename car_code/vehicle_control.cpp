#include "general.h"

String swap_side(String side)
{
	if (side == "left")
		return "right";
	else
		return "left";
}
void vehicle_control::vehicle_control_set_paramters()
{
	motor_speed=M_SPEED;

	_left_sensor.set_paramters(US_echo_left,
                                US_trig_left,
                                US_vcc_left,
								US_LED_left,
                                LEFT_ALERT_DISTANCE);
	_center_sensor.set_paramters(US_echo_center,
                                US_trig_center,
                                US_vcc_center,
								US_LED_center,
                                CENTER_ALERT_DISTANCE);
	_right_sensor.set_paramters(US_echo_right,
                                US_trig_right,
                                US_vcc_right,
								US_LED_right,
                                RIGHT_ALERT_DISTANCE);
	_back_sensor.set_paramters(US_echo_back,
								US_trig_back,
								US_vcc_back,
								US_LED_back,
                                BACK_ALERT_DISTANCE);
	my_recevier.set_paramters(22);

}


// ----------- recevier -----------
recevier& vehicle_control:: _recevier(){return my_recevier;}
void vehicle_control::search_signal()
{
    
    if(_center_sensor.get_obstacle()) // if front blocked we wont see the signal
        return;
    else if(my_recevier.check_if_there_signal())
        return;
    else if (!(_left_sensor.get_obstacle() || _right_sensor.get_obstacle()))
    {
        motor_speed = M_SPEED;
        forward();
    }
}

// ----------- ultra sony -----------
Sensor_UltraSony& vehicle_control:: left_sensor() { return _left_sensor; }
Sensor_UltraSony& vehicle_control:: center_sensor() { return _center_sensor; }
Sensor_UltraSony& vehicle_control:: right_sensor() { return _right_sensor; }
Sensor_UltraSony& vehicle_control:: back_sensor() { return _right_sensor; }

void vehicle_control::speed_control()
{
    if (state == DRIVE)
        motor_speed = MAX_SPEED;
    else
        motor_speed = M_SPEED;
    /*
    
    
    //motor_speed = M_SPEED;
    // speed control
    _center_sensor.sample_space();

    if (_center_sensor.get_distance_cm() < 1.35 * CENTER_ALERT_DISTANCE)
        motor_speed = 0.25 * M_SPEED;
    else if (_center_sensor.get_distance_cm() < 1.55 * CENTER_ALERT_DISTANCE)
        motor_speed = 0.50 * M_SPEED;
    else if (_center_sensor.get_distance_cm() < 1.85 * CENTER_ALERT_DISTANCE)
        motor_speed = 0.75 * M_SPEED;
    else if (_center_sensor.get_distance_cm() < 2.15 * CENTER_ALERT_DISTANCE)
        motor_speed = 0.95 * M_SPEED;
    else
    {
        if (motor_speed != MAX_SPEED)
            motor_speed = M_SPEED;
    }
       

    */

    Serial.print("_center_sensor.get_distance_cm():");
    Serial.print(_center_sensor.get_distance_cm());
    Serial.print("\n");

    Serial.print("motor_speed:");
    Serial.print(motor_speed);
    Serial.print("\n");
    
    // 3 m = v * 3.67sec
    // v = 0.82 m/sec
    // convert [kmh] to [motor speed]:
    // max speed = 7[kmh] = 255 [motor speed] // approx.
    // 2[m/sec] = 255[motor speed]
    // 200[cm/sec] = 255[motor speed]
    // 0.8[cm/SEC] = 1[motor speed]
 
    // _90deg_motor_delay = ALPHA * (1 / (0.8 * motor_speed)) + MIN_DELAY; //[ms]
    // pass_corner_delay = (PASS_CORNER_DISTANCE / (0.8 * motor_speed)) * 1000; // t = x/v  [ms]
    // motor_stop_delay = 1000 * ((CENTER_ALERT_DISTANCE - SAFE_OBSTACLE_DISTANCE) / (0.8 * motor_speed));
}
void vehicle_control::found_obstacle()
{
	_center_sensor.sample_space();
	_back_sensor.sample_space();
	_left_sensor.sample_space();
	_right_sensor.sample_space();

    speed_control();

}

// ----------- movment and motors -----------
void vehicle_control:: drive()
{
    if (my_recevier.check_if_there_signal())
    {
        motor_speed = MAX_SPEED;
        forward();
    }
    else
    {
        motor_speed = M_SPEED;
    }
}
char& vehicle_control:: get_motors_status(){return motors_status;}


void vehicle_control:: move(String direction)
{
	if(direction == LEFT)
		left();
	else if(direction == RIGHT)	
		right();
	else if(direction == FOR_WARD)
		forward();
}
void vehicle_control:: forward()
{
	m.motor(front_right, FORWARD, motor_speed);
	m.motor(front_left, FORWARD, motor_speed);
	m.motor(back_right, FORWARD, motor_speed);
	m.motor(back_left, FORWARD, motor_speed);
	motors_status= motors_status_forward;
}
void vehicle_control:: reverse()
{
	if(motors_status == motors_status_reverse )  
		return;	
	m.motor(front_right, BACKWARD, motor_speed);
	m.motor(front_left, BACKWARD, 1.1*motor_speed);
	m.motor(back_right, BACKWARD, motor_speed);
	m.motor(back_left, BACKWARD, motor_speed);
	motors_status = motors_status_reverse;
}
void vehicle_control:: right()
{
	if(motors_status ==motors_status_right )  
		return;	
	m.motor(front_right, BACKWARD, motor_speed);
	m.motor(front_left, FORWARD, motor_speed);
	m.motor(back_right, BACKWARD, motor_speed);
	m.motor(back_left, FORWARD, motor_speed);
	motors_status=motors_status_right;
}
void vehicle_control:: left()
{
	if(motors_status == motors_status_left )  
		return;	
	m.motor(front_right, FORWARD, motor_speed);
	m.motor(front_left, BACKWARD, motor_speed);
	m.motor(back_right, FORWARD, motor_speed);
	m.motor(back_left, BACKWARD, motor_speed);
	motors_status=motors_status_left;
} 
void vehicle_control:: stop_motor() 
{
	motor_speed = 0;
	m.motor(front_right, BRAKE, motor_speed);
	m.motor(front_left, BRAKE, motor_speed);
	m.motor(back_right, BRAKE, motor_speed);
	m.motor(back_left, BRAKE, motor_speed);
	motors_status=motors_status_stop;
}
void vehicle_control:: s_reverse(int time = DELAY_MOTOR_RESPOND)
{
    unsigned long time_0;
    bool delay_time_pass = false;
    time_0 = millis();
    unsigned long time_delay = 350;
    reverse();
    while (!(_back_sensor.sample_space() || delay_time_pass))
    {
        delay(1);
        delay_time_pass = ((millis() - time_0) > time_delay);
        
        Serial.print("am i stuck???????\n");
    } 
    stop_motor();

    _right_sensor.sample_space();
    _left_sensor.sample_space();

    Serial.print("      right:  ");
    Serial.print(_right_sensor.distance_cm);
    Serial.print("\n");
    Serial.print("      left:   ");
    Serial.print(_left_sensor.distance_cm);
    Serial.print("\n");

    // take a little turn to the open side so you wont go back to dead end agian
    if (_right_sensor.distance_cm < _left_sensor.distance_cm){
        left( );
        delay(_90deg_motor_delay/2); // 45 degrees
        Serial.print("need to turn left\n");
        stop_motor();
    }
    else if (_right_sensor.distance_cm > _left_sensor.distance_cm){
        right( );
        delay(_90deg_motor_delay/2); // 45 degrees
        Serial.print("need to turn right\n");
        stop_motor();
    }
}

void vehicle_control:: s_moveNdelay(String side, int time = DELAY_MOTOR_RESPOND)
{
	unsigned long time_0;
	unsigned long time_1;
	bool delay_time_pass = false;
	time_0 = millis();
	//time_1 = time_0;

	if (side == "back")
        reverse();
    else
        stop_motor();
	do
	{
		my_recevier.check_if_there_signal();
		if ((my_recevier.seen_signal()))
		{
			stop_motor();
			break;
		}
		if (side != "back")
		{
			if (side == LEFT && !_left_sensor.get_obstacle())
				move(side);
			else if (side == RIGHT && !_right_sensor.get_obstacle())
				move(side);
			else if (side == FOR_WARD && !_center_sensor.get_obstacle())
				move(side);

			found_obstacle();

			if (side == LEFT && _left_sensor.get_obstacle())
				stop_motor();
			else if (side == RIGHT && _right_sensor.get_obstacle())
				stop_motor();
			else if (side == FOR_WARD && _center_sensor.get_obstacle())
				stop_motor();
		}
		delay(1);
		time_1 = millis();

		delay_time_pass = (time_1 - time_0) > time;
		if (delay_time_pass)
		{
			stop_motor();
			break;
		}

	} while (true);
}
void vehicle_control:: fix_direction()
{
    Serial.print("fix_direction\n");
	// if (relative_angle > 0)
	// 	left();
	// else if (relative_angle < 0)
	// 	right();
	// // continue rotate until the car is sriaght
	// //while (!( relative_angle>= -ANGLE_ERROR &&  relative_angle<= ANGLE_ERROR));
	// stop_motor();

}

// ----------- bypass -----------


int vehicle_control::front_obstacle_routine() // need to modify while
{
    Serial.print("\nfront_obstacle_routine ");
    

    //unsigned long timeout = 5000;
    unsigned long time_0;
    // which side has more space to move? go this side (where is more free)
    if (_left_sensor.distance_cm < _right_sensor.distance_cm) //pass from the right 
    {
        //do 90 degrees rotation
        right();
        delay(_90deg_motor_delay);

        // measure the distance from the obstacle and if this distance 
        // increases (by at least TURN_DISTANCE_ERROR), then it means we
        // almost passed the obstacle, so we add a little more delay (PASS_CORNER_DELAY). 
        int x_left = _left_sensor.get_distance_cm();
        forward();
        time_0 = millis();
        while((_left_sensor.distance_cm < x_left + TURN_DISTANCE_ERROR) && !_center_sensor.obstacle )
        {
            //escape - timeout
            if (millis() - time_0 > TIMEOUT_BYPASS)
            {
                timeouted = true;
                stop_motor();
                return;
            }
            _left_sensor.sample_space();
            _center_sensor.sample_space();
            
            //// the obstacle is next to a wall, we need to flip and pass from the other side
            // if (_center_sensor.obstacle) 
            // {
            //    flip();
            //    // pass from the other side
            //    int x_right = _right_sensor.get_distance_cm();
            //    forward();
            //    time_0 = millis();
            //     while((_right_sensor.distance_cm < x_right + TURN_DISTANCE_ERROR) && !_center_sensor.obstacle )
            //     {
            //         if (millis() - time_0 > TIMEOUT_BYPASS)
            //         {
            //             timeouted = true;
            //             stop_motor();
            //             return;
            //         }
            //         _right_sensor.sample_space();
            //         _center_sensor.sample_space();
            //     }
            //     delay(pass_corner_delay); // for the car to pass the corner 
            //     right();
            //     delay(_90deg_motor_delay);
            //     stop_motor();
            //     return;
            // }
        }

        //// for the car to pass the corner
        // delay(pass_corner_delay); 
        time_0 = millis();
        forward();
        while((millis() - time_0 < pass_corner_delay) && !_center_sensor.sample_space()); 

        // turn - 90 degrees rotation
        left();
        delay(_90deg_motor_delay);
        stop_motor();
    }

    else if(_left_sensor.distance_cm > _right_sensor.distance_cm) //pass from the left
    {
        // turn - 90 degrees rotation
        left();
        delay(_90deg_motor_delay);

        int x_right = _right_sensor.get_distance_cm();
        forward();
        time_0 = millis();
        while((_right_sensor.distance_cm < x_right + TURN_DISTANCE_ERROR) && !_center_sensor.obstacle )
        {
            if (millis() - time_0 > TIMEOUT_BYPASS)
            {
                timeouted = true;
                stop_motor();
                return;
            }
            _right_sensor.sample_space();
            _center_sensor.sample_space();

            // if (_center_sensor.obstacle) // the obstacle is next to a wall, we need to flip and pass from the other side 
            // {
            //    flip();
            //    //pass from the other side
            //    int x_left = _left_sensor.get_distance_cm();
            //    forward();
            //    time_0 = millis();
            //     while((_left_sensor.distance_cm < x_left + TURN_DISTANCE_ERROR) && !_center_sensor.obstacle )
            //     {
            //         if (millis() - time_0 > TIMEOUT_BYPASS)
            //         {
            //             timeouted = true;
            //             stop_motor();
            //             return;
            //         }
            //         _left_sensor.sample_space();
            //         _center_sensor.sample_space();
            //     }
            //     delay(pass_corner_delay); // for the car to pass the corner 
            //     left();
            //     delay(_90deg_motor_delay);
            //     stop_motor();
            //     return;
            // }
        }

        //// for the car to pass the corner
        // delay(pass_corner_delay); 
        time_0 = millis();
        while((millis() - time_0 < pass_corner_delay) && !_center_sensor.sample_space()); 

        // turn - 90 degrees rotation
        right();
        delay(_90deg_motor_delay);
        stop_motor();
    }

}

void vehicle_control::left_obstacle_routine()
{
    Serial.print("\nleft_obstacle_routine ");
    //unsigned long timeout = 5000;
    unsigned long time_0 = millis();
    right();
    while (_center_sensor.obstacle || _left_sensor.obstacle)
    {
        if (millis() - time_0 > TIMEOUT_BYPASS)
        {
            timeouted = true;
            break;
        }
        found_obstacle();
    }
    stop_motor();
}
void vehicle_control::right_obstacle_routine()
{
    Serial.print("\nright_obstacle_routine ");
    //unsigned long timeout = 5000;
    unsigned long time_0 = millis();
    left();
    while (_center_sensor.obstacle || _right_sensor.obstacle)
    {
        if (millis() - time_0 > TIMEOUT_BYPASS)
        {
            timeouted = true;
            break;
        }
        found_obstacle();
    }
    stop_motor();
}

// drive along side of the object and finally reach corner of the object  
// drive while still attached to obstacle form right or left, and also the center(front) is free
void vehicle_control::drive_along_side()
{
    //unsigned long timeout = TIMEOUT_BYPASS;
    unsigned long time_0 = millis();
    while (!_center_sensor.get_obstacle() && (_right_sensor.get_obstacle() || _left_sensor.get_obstacle()))
    {
        if (millis() - time_0 > TIMEOUT_BYPASS)
        {
            timeouted = true;
            break;
        }
        forward();
        found_obstacle();
    }
    stop_motor();
}
// do a U-turn - we rotate toward the opposite side of the 
// obstacle, until the obstacle shows the opposite side,
// and the wall that was at the front is now at the back.
void vehicle_control::flip()
{
   left();
   delay(2 * _90deg_motor_delay);
}

void reset_bypass_parameters(){
    step                = START_BYPASS;
    turned_once         = false;
    fliped_once         = false;
    finish_attachment   = false;
    avoid_wall          = false;
    timeouted = false;
}

void vehicle_control:: print_obsacle_cm(){
    Serial.print("left:");
    Serial.print(_left_sensor.get_obstacle());
    Serial.print(" center:");
    Serial.print(_center_sensor.get_obstacle());
    Serial.print(" right:");
    Serial.print(_right_sensor.get_obstacle());
    Serial.print(" back:");
    Serial.print(_back_sensor.get_obstacle());
    Serial.print("\n");
} 

void vehicle_control::s_bypass_the_obstacle()
{
    int counter_attempt = 0;
    while(1)
    {
        motor_speed = M_SPEED;
        counter_attempt++;
        if (timeouted == true)
            step = STOP_BYPASS;
        if (step == START_BYPASS)
        {
            Serial.print("START_BYPASS\n");
            //unsigned char intial_motor_speed = motor_speed;
            stop_motor();
            //delay(motor_stop_delay); // time to enable the car to complete stop and preventing slippage
            found_obstacle();
            
            if (_center_sensor.get_obstacle() && _right_sensor.get_obstacle() && _left_sensor.get_obstacle())
                step = DEAD_END;
            else if (_center_sensor.get_obstacle())
                if (_left_sensor.get_obstacle())
                    step = LEFT_OBSTACLE;
                else if (_right_sensor.get_obstacle())
                    step = RIGHT_OBSTACLE;
                else // only center
                    step = FRONT_OBSTACLE;
            else if (_left_sensor.get_obstacle())
                step = LEFT_OBSTACLE;
            else if (_right_sensor.get_obstacle())
                step = RIGHT_OBSTACLE;
            else if (_back_sensor.get_obstacle())
                step = BACK_OBSTACLE;
            else 
                step = STOP_BYPASS;

            if (counter_attempt > 2)
                step = STOP_BYPASS;
            
        }
        else if (step == FRONT_OBSTACLE)
        {
            Serial.print("front obsactle \n");
            front_obstacle_routine();
            step = STOP_BYPASS;
        }
        else if (step == LEFT_OBSTACLE)
        {
            Serial.print("left obstacle\n");
            left_obstacle_routine();
            step = STOP_BYPASS;
        }
        else if (step == RIGHT_OBSTACLE)
        {
            Serial.print("right obstacle \n");
            right_obstacle_routine();
            step = STOP_BYPASS;
        }
        else if (step == BACK_OBSTACLE)
        {
            Serial.print("back obstacle\n");
            stop_motor();
            step = STOP_BYPASS;
        }
        else if (step == FLIP)
        {
            Serial.print("flip\n");
            if (!fliped_once) // avoid repeation - dont flip again
            {
                fliped_once = true;
                flip();
            }
        }
        else if (step == DEAD_END)
        {
            Serial.print("dead end \n");
            if (fliped_once == true) // the car is apparently not straight
            {
                fix_direction(); // get straight
                fliped_once = false;
            }
            s_reverse();
            step = STOP_BYPASS;
        }
        else if (step == STOP_BYPASS)
        {
            Serial.print("stop bypass \n");
            reset_bypass_parameters();
            return; // end loop
        }
        found_obstacle();
        //delay(1000);/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!////
    }


}
#include "general.h"


// --------- global vars ---------
vehicle_control vehicle;
//int bluetoothTx = 46;  // TX-O pin of bluetooth mate, Arduino D46
//int bluetoothRx = 44;  // RX-I pin of bluetooth mate, Arduino D44
//SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
int             state                = IDLE;
int             time_counter         = 0;
int             step                 = START_BYPASS; // for bypass
bool            turned_once          = false; // for bypass
bool            timeouted = false; // for bypass
bool            fliped_once          = false; // for bypass
bool            finish_attachment    = false; // for bypass
bool            avoid_wall           = false; // for bypass
unsigned long   delay_time_counter_0 = 0;
unsigned long   delay_time_counter_1 = 0;
unsigned long   delay_time_machine_0 = 0;
unsigned long   delay_time_machine_1 = 0;
String          obstacle_side        = ""; // LEFT or RIGHT

void setup()
{
    Serial.begin(9600);  // Begin the serial monitor at 9600bps
    Serial.print("setup\n");
    vehicle.vehicle_control_set_paramters();
    pinMode(BUTTON_START, INPUT);

	//bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
	//bluetooth.print("$");  // Print three times individually
	//bluetooth.print("$");
	//bluetooth.print("$");  // Enter command mode
	//delay(100);  // Short delay, wait for the Mate to send back CMD
	//bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
	// 115200 can be too fast at times for NewSoftSerial to relay the data reliably
	//bluetooth.begin(9600);  // Start bluetooth serial at 9600
}

void reset_parameters()
{
    int step = START_BYPASS; // for bypass
    digitalWrite(resiver_REDLED, LOW);
}

// --------- stat machine ---------
void print_state()
{
    switch (state)
    {
    case IDLE:
        Serial.print("IDLE");
        break;

    case SEARCH_SIGNAL:
        Serial.print("SEARCH_SIGNAL");
        break;

    case BYPASS_OBSTACLE: //try to keep the movmement forward and dodge obstcle.
        Serial.print("BYPASS_OBSTACLE");
        break;

    case DRIVE:
        Serial.print("DRIVE");
        break;
    }

}
void next_state()
{
    switch (state)
    {
        case IDLE:
            state = SEARCH_SIGNAL;
            // if (digitalRead(BUTTON_START))
            break;    
        
        case SEARCH_SIGNAL:
            if (vehicle._recevier().seen_signal())
                state = DRIVE;
            else if (vehicle.left_sensor().get_obstacle() || vehicle.center_sensor().get_obstacle() 
                        || vehicle.right_sensor().get_obstacle() || vehicle.back_sensor().get_obstacle())
                state = BYPASS_OBSTACLE;
            else 
                state = SEARCH_SIGNAL;
            break;  
     
        
        case BYPASS_OBSTACLE: //try to keep the movmement forward and dodge obstcle.
            if (vehicle.left_sensor().get_obstacle() || vehicle.center_sensor().get_obstacle() 
                    || vehicle.right_sensor().get_obstacle() || vehicle.back_sensor().get_obstacle() )
                state = BYPASS_OBSTACLE;
            else 
                state = IDLE;
            break;
        
        case DRIVE:
            if (vehicle.center_sensor().get_obstacle())
                state = BYPASS_OBSTACLE;
            else if (!vehicle._recevier().seen_signal())
                state = SEARCH_SIGNAL;
            else if (vehicle._recevier().seen_signal())
                state = DRIVE;
            else if (vehicle.left_sensor().get_obstacle() || vehicle.right_sensor().get_obstacle())
                state = BYPASS_OBSTACLE;
            else // back up
                state = IDLE;
            break;
    }
}
 
// --------- main ---------
void loop()
{        
    Serial.print(" _90deg_motor_delay ");
    Serial.print(_90deg_motor_delay);
    Serial.print(" pass_corner_delay ");
    Serial.print(pass_corner_delay);

    Serial.print("\nSTATE: ");
    print_state();

    delay_time_machine_0 = millis();
    Serial.print("      motors_status : ");
    Serial.print(vehicle.get_motors_status());
    Serial.print("\n");

    switch (state)
    {
        case IDLE: 
            vehicle.stop_motor();
            reset_parameters();
            break;
            
        case SEARCH_SIGNAL:
            vehicle.search_signal();
            // if (!vehicle._recevier().seen_signal())
            //     vehicle.fix_direction(); // get straight
            break;
          
        case BYPASS_OBSTACLE: //try to keep the movmement forward and dodge obstcle.
            vehicle.s_bypass_the_obstacle();
            //vehicle.fix_direction();
            break;
            
        case DRIVE:
            vehicle.drive();
            break;
    }
    vehicle.found_obstacle();
    next_state();
    //delay(1000);/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!////

    // delay_time_machine_1 = millis();
    // Serial.print("Time delay machine on ms between states:");
    // Serial.print(delay_time_machine_1 - delay_time_machine_0);
    // Serial.print("\n");
   

}

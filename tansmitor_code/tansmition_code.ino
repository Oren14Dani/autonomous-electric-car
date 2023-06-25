#define PIN_D5 5
#define PIN_D11 11
bool switch_state=false;

void setup() {
    pinMode(PIN_D5,INPUT_PULLUP);
    pinMode(PIN_D11,OUTPUT);
    digitalWrite(PIN_D11, LOW);
}

void loop() {
    // use tone(PIN_D11, 38000); for transmitting at 38kHz  
    switch_state = digitalRead(PIN_D5); 
    if(switch_state == true){ // modulation 100 Hz - bulid signal with T=10[msec] 
        tone(PIN_D11, 38000); 
        delay(8);
        noTone(PIN_D11);
        delay(2);
    }
    else{ // modulation 200 Hz - bulid signal with T=5[msec]
        tone(PIN_D11, 38000); 
        delay(4);
        noTone(PIN_D11);
        delay(1);   //4,2
    }
}

#include <Servo.h>


int doorpin = 9; //Servo connected to Pin 9//

Servo door; 

bool previous= false;  // made a variable to track if something detcted before 
bool current = false; // ariable whioch will detect if something now //
bool dooropen = false; //tracks door status//

void setup() {
 
  door.attach(doorpin);
  

}

void loop() {

 
    
    door.write(0);
    delay(3000);
  } 
  
 
    door.write(180);
    
    delay(3000);
  }

#include <Servo.h>
 
// setting up pins for IR Sensors
int IRS1 = 14;
int IRS2 = 15;
int IRS3 = 17;
int IRS4 = 16;
int IRS5 = 18;
 
// setting up index variables for IR and LED
int IR_index;
int led_index;
 
// setting up LED brightness
int bright_led = 255;
int dim_led = 255;
 
// setting up bool variables for IR
bool IR1_previous = 1;
bool IR1_current = 1;
bool IR2_previous = 1;
bool IR2_current = 1;
bool IR3_previous = 1;
bool IR3_current = 1;
bool IR4_previous = 1;
bool IR4_current = 1;
bool IR5_previous = 1;
bool IR5_current = 1;
 
// setting up Servo objects
Servo left_door1;
Servo left_door2;
Servo right_door1;
Servo right_door2;
 
// setting up pins for LED
int left_led1 = 13;
int left_led2 = 12;
int right_led1 = 8;
int right_led2 = 9;
 
// setting up pin for solenoid
int high_reward = 11;
int low_reward = 10;
 
// setting up variables that track the ROI crossed by mice
bool mice_ROI1 = false;
bool mice_ROI2 = false;
bool mice_ROI3 = false;
bool mice_ROI4 = false;
bool mice_ROI5 = false;
 
// setting up variables that turn to true when mice crosses ROI and operations associated with that particular ROI are performed
bool ROI1_status = false;
bool ROI2_status = false;
bool ROI3_status = false;
bool ROI4_status = false;
bool ROI5_status = false;
bool ROI1_led_status = false;
bool ROI1_door_status = false;
bool ROI2_port_status = false;
bool ROI2_door_operation = false;
bool ROI3_port_status = false;
bool ROI3_door_operation = false;
 
// setting up master variables to run the assay
bool start_trial = false;
bool reset_trial = false;
 
// variables to reset the trial
bool reset_condition = false;
bool trial_reset = false;
bool IR_values_reset = false;
bool IR_status_reset = false;
bool time_to_reset = false;
 
// variables for time
unsigned long start_time;
unsigned long current_time;
const unsigned long period = 5000;
unsigned long first_roi_timer_start;
unsigned long first_roi_timer_current;
unsigned long last_roi_timer_start;
unsigned long last_roi_timer_current;
unsigned long door_duration;

// variables for EXCEL data
unsigned int active_ROI = 0;
unsigned long checkpoint1;
unsigned long checkpoint2;
unsigned long data_start;
 
 
// helper functions START
 
bool ROI1_lights(int bright_led, int dim_led, int left_led1, int left_led2, int right_led1, int right_led2){
 
  // turning the led panels on
  analogWrite(left_led1, bright_led);
  analogWrite(left_led2, bright_led);
  analogWrite(right_led1, dim_led);
  analogWrite(right_led2, dim_led);
 
  return true;
 
}
 
bool ROI1_door_open(Servo& left_door1, Servo& right_door1){
 
  // left_door1 (servo1) and right_door1 (servo2) start rotating
  left_door1.write(0);
  right_door1.write(0);
 
  return true;
}
 
bool ROI1_door_close(Servo& left_door1, Servo& right_door1){
 
  // left_door1 (servo1) and right_door1 (servo2) rotate in the opposite direction (doors close)
  left_door1.write(180);
  right_door1.write(180);
 
  return true;
}
 
bool ROI1_door_hold(Servo& left_door1, Servo& right_door1){
 
  // left_door1 (servo1) and right_door1 (servo2) hold position
  left_door1.write(90);
  right_door1.write(90);
 
  return true;
}
 
 
 
bool ROI2_port(int high_reward){
  
  // turning the high reward solenoid on and off
  digitalWrite(high_reward, HIGH);
  delay(250);
  digitalWrite(high_reward, LOW);
 
  return true;
}
 
bool ROI2_doors(Servo& left_door1, Servo& right_door1, Servo& left_door2, unsigned long door_duration){
 
  // closing left_door1 and right_door1 | opening left_door2
  left_door1.write(180);
  right_door1.write(180);
  left_door2.write(0);
  delay(door_duration);
 
  left_door1.write(90);
  right_door1.write(90);
  left_door2.write(90);
 
  return true;
}
 
void ROI2_door_close(Servo& left_door1, Servo& right_door1){
 
  // closing left_door1 and right_door1
  left_door1.write(180);
  right_door1.write(180);
 
}
 
void ROI2_door_open(Servo& left_door2){
 
  // opening left_door2
  left_door2.write(0);
 
}
 
bool ROI2_door_hold(Servo& left_door1, Servo& right_door1, Servo& left_door2){
 
  left_door1.write(90);
  right_door1.write(90);
  left_door2.write(90);
 
  return true;
}
 
 
bool ROI3_port(int low_reward){
 
  // turning the low reward solenoid on and off
  digitalWrite(low_reward, HIGH);
  delay(250); 
  digitalWrite(low_reward, LOW);
 
  return true;
}
 
 
void ROI3_door_close(Servo& left_door1, Servo& right_door1){
 
  // closing left_door 1 and right_door1
  left_door1.write(180);
  right_door1.write(180);
 
}
 
void ROI3_door_open(Servo& right_door2){
  
  // opening right_door2
  right_door2.write(0);
 
}
 
bool ROI3_door_hold(Servo& left_door1, Servo& right_door1, Servo& right_door2){
 
  left_door1.write(90);
  right_door1.write(90);
  right_door2.write(90);
 
  return true;
}
 
 
 
void ROI4_door_close(Servo& left_door2){
 
  // closing left_door2
  left_door2.write(180);
 
}
 
bool ROI4_door_hold(Servo& left_door2){
 
  left_door2.write(90);
 
  return true;
}
 
void ROI5_door_close(Servo& right_door2){
 
  // closing left_door2
  right_door2.write(180);
 
}
 
bool ROI5_door_hold(Servo& right_door2){
 
  right_door2.write(90);
 
  return true;
}
 
bool resetting_trial(int left_led1, int left_led2, int right_led1, int right_led2){
 
  // closing off panels
  analogWrite(left_led1, 0);
  analogWrite(left_led2, 0);
  analogWrite(right_led1, 0);
  analogWrite(right_led2, 0);
 
  return true;
 
}
 
bool resetting_IRvalues(bool &IR1_previous, bool &IR1_current, bool &IR2_previous, bool &IR2_current, bool &IR3_previous, bool &IR3_current, bool &IR4_previous, bool &IR4_current, bool &IR5_previous, bool &IR5_current){
  IR1_previous = 1;
  IR1_current = 1;
  IR2_previous = 1;
  IR2_current = 1;
  IR3_previous = 1;
  IR3_current = 1;
  IR4_previous = 1;
  IR4_current = 1;
  IR5_previous = 1;
  IR5_current = 1;
 
  return true;
}
 
void time_variable_reset(unsigned long &start_time, unsigned long &current_time, unsigned long &door_duration, unsigned long &first_roi_timer_start, unsigned long &last_roi_timer_start, unsigned long &last_roi_timer_current, unsigned long &start_roi_timer_current){
 
  start_time = 0;
  current_time = 0;
  door_duration = 0;
  first_roi_timer_start = 0;
  last_roi_timer_start = 0;
  first_roi_timer_current = 0;
  last_roi_timer_current = 0;
 
  return true;
}
 
bool resetting_IRstatus(bool &mice_ROI1, bool &mice_ROI2, bool &mice_ROI3, bool &mice_ROI4, bool &mice_ROI5, bool &ROI1_status, bool &ROI2_status, bool &ROI3_status, bool &ROI4_status, bool &ROI5_status){
  mice_ROI1 = false;
  mice_ROI2 = false;
  mice_ROI3 = false;
  mice_ROI4 = false;
  mice_ROI5 = false;
 
  ROI1_status = false;
  ROI2_status = false;
  ROI3_status = false;
  ROI4_status = false;
  ROI5_status = false;
 
  return true;
}


void EXCEL_variables_reset(unsigned int &active_ROI, unsigned long &checkpoint1, unsigned long &checkpoint2){

  active_ROI = 0;
  checkpoint1 = 0;
  checkpoint2 = 0;
}
 
 
// helper functions END
 
void setup() {
 
  left_door1.attach(2);
  left_door2.attach(6);
  right_door1.attach(3);
  right_door2.attach(5);
 
  pinMode(IRS1, INPUT);
  pinMode(IRS2, INPUT);
  pinMode(IRS3, INPUT);
  pinMode(IRS4, INPUT);
  pinMode(IRS5, INPUT);
 
  pinMode(left_led1, OUTPUT);
  pinMode(left_led2, OUTPUT);
  pinMode(right_led1, OUTPUT);
  pinMode(right_led2, OUTPUT);
 
  pinMode(high_reward, OUTPUT);
  pinMode(low_reward, OUTPUT);
 
  Serial.begin(9600);
  Serial.println("CLEARSHEET");

  // defining columns for EXCEL sheet
  Serial.println("LABEL,Start Time,ROI,Checkpoint1,Checkpoint2");
 
}
 
void loop() {
 
  do{
  // taking reading from IRS1 (sensor for ROI1)
  IR1_current = digitalRead(IRS1);
 
  // checking condition of ROI1
  mice_ROI1 = (IR1_current == 0 && IR1_previous == 1);
 
  IR1_previous = IR1_current;
 
  if (mice_ROI1 == true){
    Serial.print("IR1 Current: ");
    Serial.print(IR1_current);
    Serial.print(" | IR1 Previous: ");
    Serial.println(IR1_previous);
  }
 
  } while (!mice_ROI1);

  checkpoint1 = millis();
  checkpoint2 = millis();
  data_start = millis();
  
  if (mice_ROI1){
    // mice has crossed ROI1
    ROI1_status = true;
    Serial.println("Mice has crossed ROI-1.");
  }
   
  // given that mice has crossed ROI1
  if (ROI1_status){
 
    Serial.println("ROI-1 activated.");
    start_trial = true;
    Serial.println("Trial has started.");
 
    if (start_trial){
 
    // operations associated with activation of ROI1 are performed
 
    ROI1_door_status = ROI1_door_open(left_door1, right_door1);
    Serial.println("Doors are opening.");
    start_time = millis();
 
    ROI1_led_status = ROI1_lights(bright_led, dim_led, left_led1, left_led2, right_led1, right_led2);
    Serial.println("LEDs are on.");
    }
 
    // add a while loop here that continously takes reading from ROI2 and ROI3 until mice crosses either
    do{
      // updating the sensors
      IR2_current = digitalRead(IRS2);
      IR3_current = digitalRead(IRS3);
      current_time = millis();
 
      // Check if mice entered ROI2 (sensor goes from not detected to detected)
      mice_ROI2 = (IR2_current == 0 && IR2_previous == 1) && (IR3_current == 1);
 
      // Check if mice entered ROI3 (sensor goes from not detected to detected)
      mice_ROI3 = (IR3_current == 0 && IR3_previous == 1) && (IR2_current == 1);
 
      if (current_time - start_time >= period){
 
        ROI1_door_status = ROI1_door_hold(left_door1, right_door1);
        Serial.println("Doors are being held open.");
        door_duration = period;
        current_time = 0;
      }
      else if(current_time != 0){
        door_duration = current_time - start_time;
      }
 
      // updating the variables
      IR2_previous = IR2_current;
      IR3_previous = IR3_current;
 
      if (mice_ROI2 == true || mice_ROI3 == true){
 
        Serial.print("IR2 Current: ");
        Serial.print(IR2_current);
        Serial.print(" | IR2 Previous: ");
        Serial.println(IR2_previous);
  
        Serial.print("IR3 Current: ");
        Serial.print(IR3_current);
        Serial.print(" | IR3 Previous: ");
        Serial.println(IR3_previous);
      }
 
    } while (!mice_ROI2 && !mice_ROI3);

    checkpoint1 = millis();
    // checking if mice has crossed ROI2
    if (mice_ROI2){
 
      // given that mice has crossed ROI2
      //Serial.println("ROI-2 has been activated.");
      active_ROI = 2;

 
      // performing operations associated with activation of ROI2
      ROI2_port_status = ROI2_port(high_reward);
 
      if (ROI2_port_status){
        Serial.println("High reward port has been activated!");
      }
 
      first_roi_timer_start = millis();
 
      do{
        
        first_roi_timer_current = millis();
        ROI2_door_close(left_door1, right_door1);
        ROI2_door_open(left_door2);
 
      } while ((first_roi_timer_current - first_roi_timer_start) <= door_duration);
 
      ROI2_door_operation = ROI2_door_hold(left_door1, right_door1, left_door2);
      ROI2_status = ROI2_port_status && ROI2_door_operation;
      Serial.println("LD1 closed | RD1 closed | LD2 open");
 
 
      // running a do-while loop to check mice has crossed ROI4
      do{

        IR4_current = digitalRead(IRS4);
        mice_ROI4 = (IR4_current == 0 && IR4_previous == 1);
 
        IR4_previous = IR4_current;
        
        if (mice_ROI4 == true){
          Serial.print("IR4 Current: ");
          Serial.print(IR4_current);
          Serial.print(" | IR4 Previous: ");
          Serial.println(IR4_previous);
        }
 
      } while (!mice_ROI4);
      
      // assuming mice has crossed ROI4
      if (mice_ROI4){
       
        Serial.println("Mice has crossed ROI4");

        checkpoint2 = millis();
        
        Serial.print("DATA,");
        Serial.print(data_start);
        Serial.print(",");
        Serial.print(active_ROI);
        Serial.print(",");
        Serial.print(checkpoint1);
        Serial.print(",");
        Serial.println(checkpoint2);
        
        // switching off leds
        trial_reset = resetting_trial(left_led1, left_led2, right_led1, right_led2);
        Serial.println("LEDs turned off.");
      
        // starting timer for closing left_door2
        last_roi_timer_start = millis();
 
        // closing left_door2
        do{
          
          last_roi_timer_current = millis();
          ROI4_door_close(left_door2);
 
        } while ((last_roi_timer_current - last_roi_timer_start) <= door_duration);
 
        // left_door2 has been closed, holding it in position
        ROI4_status = ROI4_door_hold(left_door2);
        Serial.println("ROI4 Operations completed.");
 
        Serial.print("ROI1: ");
        Serial.print(ROI1_status);
        Serial.print(" | ROI2: ");
        Serial.print(ROI2_status);
        Serial.print(" | ROI4: ");
        Serial.println(ROI4_status);
 
        // given ROI1 = 1 | ROI2 = 1 | ROI4 = 1 ---> we can reset trial
        reset_trial = (ROI4_status == 1) && (ROI2_status == 1) && (ROI1_status == 1);
        Serial.print("Reset Trial: ");
        Serial.println(reset_trial);

       
        if (reset_trial){
 
          // resets all IR readings to 1 (not detected)
          IR_values_reset = resetting_IRvalues(IR1_previous, IR1_current, IR2_previous, IR2_current, IR3_previous, IR3_current, IR4_previous, IR4_current, IR5_previous, IR5_current);
          //Serial.println("IR sensor values reset.");
          // resets all ROI status to false
          IR_status_reset = resetting_IRstatus(mice_ROI1, mice_ROI2, mice_ROI3, mice_ROI4, mice_ROI5, ROI1_status, ROI2_status, ROI3_status, ROI4_status, ROI5_status);
          //Serial.println("ROI status reset.");
          time_variable_reset(start_time, current_time, door_duration, first_roi_timer_start, last_roi_timer_start, first_roi_timer_current, last_roi_timer_current);
          //Serial.print("time variables have been reset.");
          EXCEL_variables_reset(active_ROI, checkpoint1, checkpoint2);
          time_to_reset = (trial_reset == 1) && (IR_values_reset == 1) && (IR_status_reset == 1);
 
          if (time_to_reset){
 
            start_trial = false;
            reset_trial = false;
            Serial.println("Trial has reset.");
 
            // trial resets here
 
          }
        }
      }
 
    // checking if mice has crossed ROI3
    } else if (mice_ROI3){
  
      // given that mice has crossed ROI3
      active_ROI = 3;
      Serial.println("ROI-3 has been activated.");
 
      // performing operations associated with activation of ROI3
      ROI3_port_status = ROI3_port(low_reward);
 
      if (ROI3_port_status){
        Serial.println("Low Reward port has been activated.");
      }
 
      first_roi_timer_start = millis();
 
      do{
 
        first_roi_timer_current = millis();
        ROI3_door_close(left_door1, right_door1);
        ROI3_door_open(right_door2);
 
      } while ((first_roi_timer_current - first_roi_timer_start) <= door_duration);
 
      ROI3_door_operation = ROI3_door_hold(left_door1, right_door1, right_door2);
      ROI3_status = ROI3_port_status && ROI3_door_operation;
      Serial.println("LD1 closed | RD1 closed | RD2 open");
 
      // running a do-while loop to check if mice has crossed ROI5
      do{
 
        IR5_current = digitalRead(IRS5);
        mice_ROI5 = (IR5_current == 0 && IR5_previous == 1);
 
        IR5_previous = IR5_current;
     
      } while (!mice_ROI5);
 
      // assuming mice has crossed ROI5
      if (mice_ROI5){
        
        Serial.println("ROI5 has been crossed by mice.");

        checkpoint2 = millis();
        
        Serial.print("DATA,");
        Serial.print(data_start);
        Serial.print(",");
        Serial.print(active_ROI);
        Serial.print(",");
        Serial.print(checkpoint1);
        Serial.print(",");
        Serial.println(checkpoint2);
        // switching off leds
        trial_reset = resetting_trial(left_led1, left_led2, right_led1, right_led2);
        Serial.println("leds have turned off.");
 
        // starting timer to close right_door2
        last_roi_timer_start = millis();
 
        do{
          last_roi_timer_current = millis();
          ROI5_door_close(right_door2);
 
        } while ((last_roi_timer_current - last_roi_timer_start) <= door_duration);
 
        ROI5_status = ROI5_door_hold(right_door2);
        Serial.println("ROI5 operations completed.");
       
        // given ROI1 = 1 | ROI3 = 1 | ROI5 = 1 ---> we can reset the trial
        reset_trial = (ROI5_status == 1) && (ROI3_status == 1) && (ROI1_status == 1);
        Serial.print("Reset Trial");
        Serial.println(reset_trial);
        
 
        if (reset_trial){
 
          // resets all IR readings to 1 (not detected)
          IR_values_reset = resetting_IRvalues(IR1_previous, IR1_current, IR2_previous, IR2_current, IR3_previous, IR3_current, IR4_previous, IR4_current, IR5_previous, IR5_current);
          Serial.println("IR values reset.");
          // resets all ROI status to false
          IR_status_reset = resetting_IRstatus(mice_ROI1, mice_ROI2, mice_ROI3, mice_ROI4, mice_ROI5, ROI1_status, ROI2_status, ROI3_status, ROI4_status, ROI5_status);
          Serial.println("ROI status reset.");
          // resetting time variables
          time_variable_reset(start_time, current_time, door_duration, first_roi_timer_start, last_roi_timer_start, first_roi_timer_current, last_roi_timer_current);
          Serial.println("time variables have been reset.");
          EXCEL_variables_reset(active_ROI, checkpoint1, checkpoint2);


 
          time_to_reset = (trial_reset == 1) && (IR_values_reset == 1) && (IR_status_reset == 1);
 
          if (time_to_reset){
            start_trial = false;
            reset_trial = false;
            Serial.println("Trail has been reset.");
 
            // trial resets here
 
          }
        }
      }
    }
 
  }
 
}

int IRS = 2; // Connected to PIN 2//
int LED = 13; //Arduino has LED at 13//
bool previous= false;  // made a variable to track if something detcted before 
bool current = false; // variable whioch will detect if something now //

void setup() {
  Serial.begin(9600); 

  pinMode(IRS, INPUT); //sets the sensor pin as input//
  pinMode(LED, OUTPUT); //led is now the output//
  Serial.println("Checking"); //just checking BKLD//

}

void loop() {
  current = !digitalRead(IRS); // Now og is  such that low is when object detected and high is when no object so now we will flip this shit and current variable will read the sensor such that High is ehn object deteced and low is when no object (makes sense) //
  
  if (current && !previous){
    // if now there is something when previously there was nothing//
    Serial.println("Detected"); // just prits whatever we want in serial monitor//
    digitalWrite(LED, HIGH); //intuitive//
    delay(100);
  } 
  
  else if (!current && previous){ 
    // if nothing now but there previously //
    Serial.println("Not deteced");
    digitalWrite(LED, LOW);
    delay(100);
  }

  previous = current; // we update our wha was before as now so that if there is something passing by it becomes previous and for further loop detction//

  delay(50);
}

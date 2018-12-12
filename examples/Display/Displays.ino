/*
  Control Chain - Hardware UI extender 

  -----------------------------
  this is a simple example of how to use LiquidCrystal objects with the Control Chain Arduino shield.
  In this example two 20x4 displays are used and 4 potentiometers as actuators. 
 
    For more information about Control Chain, please check:
    http://wiki.moddevices.com/wiki/Control_Chain
*/

#include <ControlChain.h>
#include <LiquidCrystal.h>

//amount of potentiometers connected
#define amountOfPorts 4 
//LCD's used
#define lines 4
#define charachters 20

float potValues[amountOfPorts], actuatorValues[amountOfPorts], maxValues[amountOfPorts], minValues[amountOfPorts];
//2D array for saving the actuator labels
char actuatorNames [amountOfPorts][20] ;

ControlChain cc;

//initialize displays (all pins are the same for both displays EXCEPT THE ENABLE PIN 5, 6)
LiquidCrystal lcd(7, 5, 9, 10, 11, 12);
LiquidCrystal lcd2(7, 6, 9, 10, 11, 12);

void setup() {
  //initialize control chain, this should always happen before initializing other components. 
  cc.begin();

  // define device name (1st parameter) and its URI (2nd parameter)
  // the URI must be an unique identifier for your device. A good practice
  // is to use a URL pointing to your project's code or documentation

  const char *uri = "https://github.com/moddevices/cc-arduino-lib/tree/master/examples/Displays";
  cc_device_t *device = cc.newDevice("UI_ext", uri);

  //initialize all available potentiometers 
  for (int i = 0; i < amountOfPorts; i++) {
    // configure actuator
    cc_actuator_config_t actuator_config;
    actuator_config.type = CC_ACTUATOR_CONTINUOUS;
    switch(i){
      case 0:
        actuator_config.name = ("pot #1");
      break;
      case 1:
        actuator_config.name = ("pot #2");
      break;
      case 2:
        actuator_config.name = ("pot #3");
      break;
      case 3:
        actuator_config.name = ("pot #4");
       break;
    }
  
    actuator_config.value = &potValues[i];
    actuator_config.min = 0.0;
    actuator_config.max = 1023.0;
    actuator_config.supported_modes = CC_MODE_REAL | CC_MODE_INTEGER;
    actuator_config.max_assignments = 1;

    // create and add actuator to device
    cc_actuator_t *actuator;
    actuator = cc.newActuator(&actuator_config);
    cc.addActuator(device, actuator);
  }

  //start both lcd's (20 character x 4 rows)
  lcd.begin(charachters, lines);
  lcd2.begin(charachters, lines);  
  //display startup message
  startupmessage(); 

  //set event callbacks
  cc.setEventCallback(CC_EV_UPDATE, updateValues);
  cc.setEventCallback(CC_EV_ASSIGNMENT, updateNames);
  cc.setEventCallback(CC_EV_UNASSIGNMENT, clearlcd);   
}

void loop() {
  // continually update potentiometer values, and run Control Chain
  readpots();
  cc.run();
}

//updates actuator value and calls the write function
void updateValues(cc_assignment_t *assignment){
  actuatorValues[assignment->actuator_id] = assignment->value;
  writeValues(assignment->actuator_id);
}

//when assigned to a actuator, gets the needed information and calls the write names function
void updateNames(cc_assignment_t *assignment){
  minValues[assignment->actuator_id] = assignment->min;
  maxValues[assignment->actuator_id] = assignment->max;
  for (int i = 0; i < assignment->label.size; i++){
    actuatorNames[assignment->actuator_id][i] = assignment->label.text[i];
  }
  writeNames(assignment->actuator_id, assignment->label.size, 0);
}

//when unassigned clear the display.
void clearlcd(cc_assignment_t *assignment){
  int num = assignment->actuator_id;
  switch (num){
    case 0:
      writeNames(num, 0, 1);
      lcd.setCursor(0,1);
      lcd.print("                    ");
    break;
    case 1:
      writeNames(num, 0, 1);
      lcd.setCursor(0,3);
      lcd.print("                    ");
    break;
    case 2:
      writeNames(num, 0, 1);
      lcd2.setCursor(0,1);
      lcd2.print("                    ");
    break;
    case 3:
      writeNames(num, 0, 1);
      lcd2.setCursor(0,3);
      lcd2.print("                    ");
    break;
  }
}

//displays startup message
void startupmessage(){
  lcd.setCursor(0,0);
  lcd.print ("MOD DEVICES");
  lcd.setCursor(0,2);
  lcd.print("Control Chain");
  
  lcd2.setCursor(0,0);
  lcd2.print ("UI Extension");
  lcd2.setCursor(0,2);
  lcd2.print("Vers: 0.0.1");
  delay(2000);
  lcd.clear();
  lcd2.clear();
  for (int i = 0; i < amountOfPorts; i++){
    writeNames (i, 0, 1);
  }
}

//reads all available potentiometers
void  readpots(){
  for (int i; i<amountOfPorts; i++){
    potValues[i]=analogRead(i);
  }
}

//write the name of the selected potentiometer
void writeNames (int num, int labelsize, int clr){
  switch (num){
  case 0:
    lcd.setCursor(0,0);
    lcd.print ("#Pot 1:              ");
    if ( clr != 1){
      lcd.setCursor(10,0);
      for (int i = 0; i < labelsize; i++){
        lcd.print (actuatorNames[num][i]);
      }
    }
  break;
  case 1:
    lcd.setCursor(0,2);
    lcd.print ("#Pot 2:              ");
    if ( clr != 1){
      lcd.setCursor(10,2);
      for (int i = 0; i < labelsize; i++){
        lcd.print (actuatorNames[num][i]);
      }
    }
  break;
  case 2:
    lcd2.setCursor(0,0);
    lcd2.print ("#Pot 3:              ");
    if ( clr != 1){
      lcd2.setCursor(10,0);
      for (int i = 0; i < labelsize; i++){
        lcd2.print (actuatorNames[num][i]);
      }
    }
  break;
  case 3:
    lcd2.setCursor(0,2);
    lcd2.print ("#Pot 4:              ");
    if ( clr != 1){
      lcd2.setCursor(10,2);
      for (int i = 0; i < labelsize; i++){
        lcd2.print (actuatorNames[num][i]);
      }
    }
    break;
  }
}

//selects a line acourding to the given potentiometer
void selectline (int potnumb){
  switch (potnumb){
  case 0:
    lcd.setCursor(0,1);
  break;
  case 1:
    lcd.setCursor(0,3);
  break;
  case 2:
    lcd2.setCursor(0,1);;
  break;
  case 3:
    lcd2.setCursor(0,3);
  break;   
  }
}

//scales the selected value, then compares it in a switch statement to select the right visual then prints it.
 void writeValues (int potnumb){
 int X = map((actuatorValues[potnumb]*10), (minValues[potnumb]*10), (maxValues[potnumb]*10), 0, 10);
 switch (X){
  case 0:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[O- - - - - ] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {
      lcd2.print("[O- - - - - ] ");
      lcd2.print(actuatorValues[potnumb]);
    }
  break;
  case 1:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[ O - - - - ] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {
      lcd2.print("[ O - - - - ] ");
      lcd2.print(actuatorValues[potnumb]);
    }
  break;
  case 2:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[ -O- - - - ] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {
      lcd2.print("[ -O- - - - ] ");
      lcd2.print(actuatorValues[potnumb]);
    }
  break;
  case 3:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[ - O - - - ] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {;
      lcd2.print("[ - O - - - ] ");
      lcd2.print(actuatorValues[potnumb]);
    }
  break;
  case 4:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[ - -O- - - ] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {
      lcd2.print("[ - -O- - - ] ");
      lcd2.print(actuatorValues[potnumb]);
    }
  break;
  case 5:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[ - - O - - ] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {
      lcd2.print("[ - - O - - ] ");
      lcd2.print(actuatorValues[potnumb]);
    }
  break;
  case 6:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[ - - -O- - ] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {
      lcd2.print("[ - - -O- - ] ");
      lcd2.print(actuatorValues[potnumb]);
    }
  break;
  case 7:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[ - - - O - ] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {
      lcd2.print("[ - - - O - ] ");
      lcd2.print(actuatorValues[potnumb]);
    }
  break;
  case 8:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[ - - - -O- ] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {
      lcd2.print("[ - - - -O- ] ");
      lcd2.print(actuatorValues[potnumb]);
    }
  break;
  case 9:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[ - - - - O ] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {
      lcd2.print("[ - - - - O ] ");
      lcd2.print(actuatorValues[potnumb]);
    }
  break;
  case 10:
    selectline(potnumb);
    if (potnumb  <=1 ){
      lcd.print("[ - - - - -O] ");
      lcd.print(actuatorValues[potnumb]);
    }
    else {
      lcd2.print("[ - - - - -O] ");
      lcd2.print(actuatorValues[potnumb]);
     }
  break;
 }
}




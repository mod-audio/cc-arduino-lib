/*
  Control Chain - Wireless strech sensor

  This is a simple example of how to use a wireless sensor with Control Chain. 
  This build consists of 2 modules, one ATTiny 85 bassed module and an Arduino bassed module
  This code is meanth for the Arduino side of this build, 

  The Arduino recieves its data from the ATTiny 85 over bleutooth and binds it to a Control Chain actuator. 
  
  the circuit:
    * connecting the serial port of the  BLE module to the Serial 3 port of the Arduino
    * power the BLE module with 5V. 

  After upload this example to your Arduino connect it to the MOD using
  the Control Chain shield.
    
  For more information about Control Chain, please check:
  http://wiki.moddevices.com/wiki/Control_Chain
*/

#include <ControlChain.h>

ControlChain cc;
float tmp, prevtmp, val;

void setup() {

  Serial3.begin(9600); 
  
  // initialize control chain
  // note that control chain requires the Serial 0 and pin 2, which means
  // these peripherals won't be available to be used in your program
  cc.begin();

  // define device name (1st parameter) and its URI (2nd parameter)
  // the URI must be an unique identifier for your device. A good practice
  // is to use a URL pointing to your project's code or documentation
  const char *uri = "https://github.com/moddevices/cc-arduino-lib/tree/master/examples/Strech-Strap";
  cc_device_t *device = cc.newDevice("BT-Strap", uri);

  // configure actuator
  cc_actuator_config_t actuator_config;
  actuator_config.type = CC_ACTUATOR_CONTINUOUS;
  actuator_config.name = "Strech Sensor";
  actuator_config.value = &val;
  actuator_config.min = 0.0;
  actuator_config.max = 25.0;
  actuator_config.supported_modes = CC_MODE_REAL | CC_MODE_INTEGER;
  actuator_config.max_assignments = 1;

  // create and add actuator to device
  cc_actuator_t *actuator;
  actuator = cc.newActuator(&actuator_config);
  cc.addActuator(device, actuator);
}

void loop() {

  if (Serial3.available()!=0){
    tmp = Serial3.read();
    if (tmp > prevtmp){
      if ((tmp -  prevtmp) > 1){
      //the mapping of the value is dependent on the used sensor
        val = map (tmp, 135, 175, 0, 25); 
      }
    }
    if (tmp < prevtmp){
      if ((prevtmp - tmp) > 1){
      //the mapping of the value is dependent on the used sensor
        val = map (tmp, 135, 175, 0, 25); 
      }
    }
  }
  prevtmp=tmp;

  // Note: The code of your device should not block the loop or have long delays (> 1ms)

  // this function always must be placed in your program loop
  // it's responsible for the control chain processing
  cc.run();
}

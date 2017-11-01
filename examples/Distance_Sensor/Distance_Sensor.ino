/*
	Control Chain - HC-SR04 Distance sensor
	
	this is a simple example of how to use a HC-SR04 distance sensor together with the Control Chain Arduino shield.
	the values of the sensor are used to control a linear continuous parameter of any effect running on the MOD.
	a potentiometer is used to control the 'sensitivity' of the sensor by using a weighted average.  
	(based on notes by David Schultz at http://home.earthlink.net/~david.schultz/rnd/2002/KalmanApogee.pdf)
	
	the circuit:
		* connecting the sensor: connect the trigger pin to Arduino pin D10 and the echo pin to Arduino pin D11
		* connect the center pin of the potentiometer to pin A0(sensitivity), A1(minimum distance) and A2(maximum distance)
		
	there is also the possibility to use the sensor without the extra potentiometers. 
	this can be done by replacing the analogRead() function by any number between 0 and 1024.
	(this number will set your 'sensitivity')

  After upload this example to your Arduino connect it to the MOD using
  the Control Chain shield.
    
  For more information about Control Chain, please check:
  http://wiki.moddevices.com/wiki/Control_Chain
*/

//use this line to invert the result (0 sets the largest distance as largest value of the actuator,
//1 sets the largest distance as smallest value of the actuator)
#define inv 1

#include <ControlChain.h>

ControlChain cc;

float MINDISTANCE = 0, MAXDISTANCE = 50;//distance parameters (with default values)
int tp = 10;//appoint trigger pin
int ep = 11;//appoint echo pin
float PotValue; //value of sensitivity potentiometer
float distance = 0, tempDistance = 0, smoothDistance = 0, scalledDistance = 0;//distance parameters
int cmDivisor = 54; //for calculation of the value's

void setup()
{ 
  //start CC, set uri and device name
  cc.begin();

  const char *uri = "https://github.com/moddevices/cc-arduino-lib/tree/master/examples/Distance_Sensor";
  cc_device_t *device = cc.newDevice("UltraSonic-Sensor", uri);

  //config pins for the ultrasonic sensor
  pinMode(tp, OUTPUT);
  pinMode(ep, INPUT);

  //config for Control Chain
  cc_actuator_config_t actuator_config;
  actuator_config.type = CC_ACTUATOR_CONTINUOUS;
  actuator_config.name = "DistanceSens";
  actuator_config.value = &smoothDistance;
  actuator_config.min = 5.0;
  actuator_config.max = 100;
  actuator_config.supported_modes = CC_MODE_REAL | CC_MODE_INTEGER;
  actuator_config.max_assignments = 1;
  
  // create and add actuator to device
  cc_actuator_t *actuator;
  actuator = cc.newActuator(&actuator_config);
  cc.addActuator(device, actuator);
}

void loop()
{ 
  //read sensor value (and convert it to CM)
  digitalWrite(tp,LOW);
  delayMicroseconds(10);
  digitalWrite(tp,HIGH);
  delayMicroseconds(50);
  digitalWrite(tp,LOW);
  //waits for a response, if none was giving (distance to large) it will return 0
  long microsec = pulseIn(ep,HIGH, 5000);
  
  //if the measurement was not completed, or is outside of the set boundaries it will not update the value (keep its last value)
  if ((microsec != 0)&&(microsec < (MAXDISTANCE*cmDivisor))&&(microsec>(MINDISTANCE*cmDivisor)))
  {
    tempDistance = ((microsec) / cmDivisor);
  }
  
  //read the 2 potentiometers who set the minimum and maximum value
  readminmax();

  scalledDistance = (float)scale(tempDistance);

  //potentiometer for setting sensitivity of  the sensor (if no potentiometer is connected enter a value between 0 and 1023)
  PotValue = (((analogRead(A0))/1023.0));
  //apply all in a weighted average filter 
  distance = filter(scalledDistance, PotValue, smoothDistance);

  //save value 
  smoothDistance = distance;
  
  //run control chain (should not be blocked for to long (<5ms)
  cc.run();
}

// filter the current result using a weighted average filter:
float filter(float rawValue, float weight, float lastValue) 
{
  //filter equation
  float result = (weight * rawValue) + (1.0-weight)*lastValue;
  return result;
}

//scale the filtered value for Control Chain (0-100)
float scale (float value)
{
  //check if the values need to be inverted
  switch (inv)
  {
  case 0:
    value = map(value, MINDISTANCE, MAXDISTANCE, 0, 100);
  break;
  case 1:
    value = map(value, MINDISTANCE, MAXDISTANCE, 100, 0);
  break;
  }
  
  return value;
}


//read the potentiometers and scales the value within set boundaries 
//these boundaries are set by both the sensor and Control Chain 
void readminmax()
{
  MINDISTANCE =map(analogRead(A1), 0, 1023, 5, 20);
  MAXDISTANCE =map(analogRead(A2), 0, 1023, 20, 65);
}



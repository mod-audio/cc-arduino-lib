/*
    Control Chain - Potentiometer

    This is a very simple example of how to use a potentiometer to control a
    linear continuous parameter of any effect running in your MOD.

    The circuit:
        * Connect the center pin of the potentiometer to the pin A0
        * Connect another pin (anyone) to IOREF pin and the last one to GND

    There are two options to connect the Arduino to your MOD:
        1. Using a Control Chain Arduino shield and an Ethernet cable
        2. Using the regular Arduino USB cable

    You should use the last case only in the development stage.

    For more information about Control Chain, please check:
    http://wiki.moddevices.com/wiki/Control_Chain

*/

#include <ControlChain.h>

ControlChain cc;
float potValue;
int ledPin = 13, potPin = A0;

void setup() {
    // configure led
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // initialize control chain
    // note that control chain requires the Serial 0 and pin 2, which means
    // these peripherals won't be available to be used in your program
    cc.init();

    // define device name (1st parameter) and its URI (2nd parameter)
    // the URI must be an unique identifier for your device. A good practice
    // is to use a URL pointing to your project's code or documentation
    const char *uri = "https://github.com/moddevices/cc-arduino-lib/tree/master/examples/Pot";
    cc_device_t *device = cc.newDevice("Pot", uri);

    // configure actuator
    cc_actuator_config_t actuator_config;
    actuator_config.type = CC_ACTUATOR_CONTINUOUS;
    actuator_config.name = "TurnMe";
    actuator_config.value = &potValue;
    actuator_config.min = 0.0;
    actuator_config.max = 1023.0;
    actuator_config.supported_modes = CC_MODE_REAL | CC_MODE_INTEGER;
    actuator_config.max_assignments = 1;

    // create and add actuator to device
    cc_actuator_t *actuator;
    actuator = cc.newActuator(&actuator_config);
    cc.addActuator(device, actuator);
}

void loop() {
    potValue = analogRead(potPin);

    // Note: The code of your device should not block the loop or have long delays (> 1ms)

    // this function always must be placed in your program loop
    // it's responsible for the control chain processing
    cc.run();
}

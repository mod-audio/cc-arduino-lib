/*
    Control Chain - Expression Pedal

    In this example, code is presented that allows you to connect either
    potentiometers, or TRS jack sockets (for the purpose of connecting to
    an expression pedal) to the Arduino CC shield. Simply adapt the define
    to the desired amount of ports and follow the directions below:

    The circuit:
        * Connect up to 4 (Arduino Uno) or 8 (Arduino Due) TRS jack sockets
        * T: Connect to 5V for all ports
        * R: Connect to analog input N where N = port index
        * S: Connect to ground for all ports

    For further information check:
      http://blog.moddevices.com/2017/05/06/tutorial-arduino-control-chain/
*/

#include <ControlChain.h>
#define amountOfPorts 1

ControlChain cc;
float portValues[amountOfPorts];

void setup() {
    // initialize control chain
    // note that control chain requires the Serial 0 and pin 2, which means
    // these peripherals won't be available to be used in your program
    cc.init();

    // define device name (1st parameter) and its URI (2nd parameter)
    // the URI must be an unique identifier for your device. A good practice
    // is to use a URL pointing to your project's code or documentation
    const char *uri =
      "https://github.com/moddevices/cc-arduino-lib/tree/master/examples/ExpressionPedalToCC";
    cc_device_t *device = cc.newDevice("EPtoCC", uri);

    // configure actuators
    for (int i = 0; i < amountOfPorts; i++) {
        cc_actuator_config_t actuator_config;
        actuator_config.type = CC_ACTUATOR_CONTINUOUS;

        switch (i) {
            case 0:
                actuator_config.name = "Chan #1";
                break;
            case 1:
                actuator_config.name = "Chan #2";
                break;
            case 2:
                actuator_config.name = "Chan #3";
                break;
            case 3:
                actuator_config.name = "Chan #4";
                break;
            case 4:
                actuator_config.name = "Chan #5";
                break;
            case 5:
                actuator_config.name = "Chan #6";
                break;
            case 6:
                actuator_config.name = "Chan #7";
                break;
            case 7:
                actuator_config.name = "Chan #8";
                break;
        }

        actuator_config.value = &portValues[i];
        actuator_config.min = 0.0;
        actuator_config.max = 1023.0;
        actuator_config.supported_modes = CC_MODE_REAL | CC_MODE_INTEGER;
        actuator_config.max_assignments = 1;

        // create and add actuator to device
        cc_actuator_t *actuator;
        actuator = cc.newActuator(&actuator_config);
        cc.addActuator(device, actuator);
    }
}

void loop() {
    // read and set port values
    for (int i = 0; i < amountOfPorts; i++) {
        portValues[i] = analogRead(i);
    }

    // Note: The code of your device should not block the loop or have long delays (> 1ms)

    // this function always must be placed in your program loop
    // it's responsible for the control chain processing
    cc.run();
}

#include <ControlChain.h>

ControlChain cc;
float buttonValue;
int ledPin = 13, buttonPin = 7;

void setup()
{
    // configure led
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // configure button
    pinMode(buttonPin, INPUT);

    // initialize control chain
    // note that control chain requires the Serial 0 and pin 2, which means
    // these peripherals won't be available to be used in your program
    cc.init();

    // define device name (1st parameter) and its URI (2nd parameter)
    // the URI must be an unique identifier for your device. A good practice
    // is to use a URL pointing to your project's code or documentation
    cc_device_t *device = cc.newDevice("Button", "uri:Button");

    // configure actuator
    cc_actuator_config_t actuator_config;
    actuator_config.type = CC_ACTUATOR_MOMENTARY;
    actuator_config.name = "PressMe";
    actuator_config.value = &buttonValue;
    actuator_config.min = 0.0;
    actuator_config.max = 1.0;
    actuator_config.supported_modes = CC_MODE_TOGGLE | CC_MODE_TRIGGER;
    actuator_config.max_assignments = 1;

    // create and add actuator to device
    cc_actuator_t *actuator;
    actuator = cc.newActuator(&actuator_config);
    cc.addActuator(device, actuator);
}

void loop()
{
    // read the button
    if (digitalRead(buttonPin) == LOW) {
        delay(10);
        if (digitalRead(buttonPin) == LOW) {
            buttonValue = 1.0;
            digitalWrite(ledPin, HIGH);
        }
    } else if (buttonValue == 1.0) {
        delay(5);
        if (digitalRead(buttonPin) == HIGH) {
            buttonValue = 0.0;
            digitalWrite(ledPin, LOW);
        }
    }

    // this function always must be placed in your program loop
    // it's responsible for the control chain processing
    cc.run();
}

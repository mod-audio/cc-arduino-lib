/*
    Control Chain - Button

    This is a very simple example of how to use a push button to control a
    toggle, trigger or bypass parameter of any effect running in your MOD.

    The circuit:
        * Connect a push button from pin 7 to GND

    There are two options to connect the Arduino to your MOD:
        1. Using a Control Chain Arduino shield and an Ethernet cable
        2. Using the regular Arduino USB cable

    You should use the last case only in the development stage.

    For more information about Control Chain, please check:
    http://wiki.moddevices.com/wiki/Control_Chain

    Note: Debounce code from: https://www.arduino.cc/en/tutorial/debounce
*/

#include <ControlChain.h>

ControlChain cc;
float buttonValue;
int ledPin = 13, buttonPin = 7;

int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;  // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 20;    // the debounce time; increase if the output flickers

void setup() {
    // configure led
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // configure button pin as input and enable internal pullup
    pinMode(buttonPin, INPUT);
    digitalWrite(buttonPin, HIGH);

    // initialize control chain
    // note that control chain requires the Serial 0 and pin 2, which means
    // these peripherals won't be available to be used in your program
    cc.begin();

    // define device name (1st parameter) and its URI (2nd parameter)
    // the URI must be an unique identifier for your device. A good practice
    // is to use a URL pointing to your project's code or documentation
    const char *uri = "https://github.com/moddevices/cc-arduino-lib/tree/master/examples/Button";
    cc_device_t *device = cc.newDevice("Button", uri);

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

    // set a callback function for the update event
    // this means that the updateLED function will be called by the
    // library every time there is an assignment update on this device
    cc.setEventCallback(CC_EV_UPDATE, updateLED);

    // the currently possible event callbacks are:
    // CC_EV_ASSIGNMENT, CC_EV_UNASSIGNMENT and CC_EV_UPDATE
}

void updateLED(cc_assignment_t *assignment) {
    // check if assignment mode is toggle
    // turn led on/off according the assignment value
    if (assignment->mode & CC_MODE_TOGGLE) {
        if (assignment->value > 0.0)
            digitalWrite(ledPin, HIGH);
        else
            digitalWrite(ledPin, LOW);
    }
}

int readButton(void) {
    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH),  and you've waited
    // long enough since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != buttonState) {
            buttonState = reading;

            // save button last state
            lastButtonState = reading;

            // button pressed
            if (buttonState == LOW) {
                return 1;
            // button released
            } else {
                return -1;
            }
        }
    }

    lastButtonState = reading;
    return 0;
}

void loop() {
    // read button state
    // state =  1 -> button pressed
    // state = -1 -> button released
    // state =  0 -> same state as before (no change)
    int state = readButton();
    if (state == 1) {
        buttonValue = 1.0;
    } else if (state == -1) {
        buttonValue = 0.0;
    }

    // Note: The code of your device should not block the loop or have long delays (> 1ms)

    // this function always must be placed in your program loop
    // it's responsible for the control chain processing
    cc.run();
}

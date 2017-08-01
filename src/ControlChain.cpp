#include <Arduino.h>
#include "ControlChain.h"

void (*ControlChain::assignment_cb)(cc_assignment_t *) = 0;
void (*ControlChain::unassignment_cb)(int) = 0;
void (*ControlChain::update_cb)(cc_assignment_t *) = 0;

void ControlChain::begin() {
    // pin 2 is used to enable transceiver
    pinMode(TX_DRIVER_PIN, OUTPUT);
    digitalWrite(TX_DRIVER_PIN, LOW);

    // generate seed by reading ADC
    int seed = 0;
    for (int i = 0; i < 5; i++)
        seed ^= analogRead(i);

    // init random generator
    srand(seed);

    Serial.begin(CC_BAUD_RATE_FALLBACK);
    cc_init(responseCB, eventsCB);
}

void ControlChain::run() {
    cc_process();
}

cc_device_t* ControlChain::newDevice(const char *name, const char *uri) {
    return cc_device_new(name, uri);
}

cc_actuator_t* ControlChain::newActuator(cc_actuator_config_t *actuator_config) {
    return cc_actuator_new(actuator_config);
}

void ControlChain::addActuator(cc_device_t *device, cc_actuator_t *actuator) {
    cc_device_actuator_add(device, actuator);
}

void ControlChain::setEventCallback(int event_id, void (*function_cb)(void *arg)) {
    if (event_id == CC_EV_ASSIGNMENT) {
        assignment_cb = (void (*)(cc_assignment_t*)) function_cb;
    } else if (event_id == CC_EV_UNASSIGNMENT) {
        unassignment_cb = (void (*)(int)) function_cb;
    } else if (event_id == CC_EV_UPDATE) {
        update_cb = (void (*)(cc_assignment_t*)) function_cb;
    }
}

void ControlChain::responseCB(void *arg) {
    // enable driver
    digitalWrite(TX_DRIVER_PIN, HIGH);

    // wait driver to enable
    for (volatile int delay = 0; delay < 100; delay++);

    cc_data_t *response = (cc_data_t *) arg;
    for (int i = 0; i < response->size; i++) {
        Serial.write(response->data[i]);
        Serial.flush();
    }

    // disable driver
    digitalWrite(TX_DRIVER_PIN, LOW);
}

void ControlChain::eventsCB(void *arg) {
    cc_event_t *event = (cc_event_t *) arg;

    if (event->id == CC_EV_ASSIGNMENT) {
        cc_assignment_t *assignment = (cc_assignment_t *) event->data;

        if (assignment_cb)
            assignment_cb(assignment);
    } else if (event->id == CC_EV_UNASSIGNMENT) {
        int *act_id = (int *) event->data;
        int actuator_id = *act_id;

        if (unassignment_cb)
            unassignment_cb(actuator_id);
    } else if (event->id == CC_EV_UPDATE) {
        cc_assignment_t *assignment = (cc_assignment_t *) event->data;

        if (update_cb)
            update_cb(assignment);
    }
}

void serialEvent(void) {
    uint8_t buffer[32];

    cc_data_t received;
    received.data = buffer;
    received.size = Serial.available();

    if (received.size > sizeof(buffer))
        received.size = sizeof(buffer);

    if (received.size > 0) {
        Serial.readBytes(received.data, received.size);
        cc_parse(&received);
    }
}

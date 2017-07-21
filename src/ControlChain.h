#include <stdint.h>
#include "control_chain.h"

#define TX_DRIVER_PIN   2

class ControlChain {
    public:
        // to keep backward compatible
        void init() {
            begin();
        }

        void begin() {
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
            cc_init(responseCB, 0);
        }

        void run() {
            cc_process();
        }

        cc_device_t* newDevice(const char *name, const char *uri) {
            return cc_device_new(name, uri);
        }

        cc_actuator_t* newActuator(cc_actuator_config_t *actuator_config) {
            return cc_actuator_new(actuator_config);
        }

        void addActuator(cc_device_t *device, cc_actuator_t *actuator) {
            cc_device_actuator_add(device, actuator);
        }

    private:
        static void responseCB(void *arg) {
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
};

void serialEvent(void)
{
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

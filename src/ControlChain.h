#include <stdint.h>
#include "control_chain.h"

class ControlChain {
    public:
        void init()
        {
            // pin 2 is used to enable transceiver
            pinMode(2, OUTPUT);
            digitalWrite(2, HIGH);
            delay(1);

            Serial.begin(CC_BAUD_RATE);
            cc_init(responseCB, 0);
        }

        void run()
        {
            cc_process();
        }

        cc_device_t* newDevice(const char *name, const char *uri)
        {
            return cc_device_new(name, uri);
        }

        cc_actuator_t* newActuator(cc_actuator_config_t *actuator_config)
        {
            return cc_actuator_new(actuator_config);
        }

        void addActuator(cc_device_t *device, cc_actuator_t *actuator)
        {
            cc_device_actuator_add(device, actuator);
        }

    private:
        static void responseCB(void *arg)
        {
            cc_data_t *response = arg;
            Serial.write(response->data, response->size);
        }
};

void serialEvent(void)
{
    uint8_t buffer[128];

    cc_data_t received;
    received.data = buffer;
    received.size = Serial.available();

    if (received.size > 0)
    {
        Serial.readBytes(received.data, received.size);
        cc_parse(&received);
    }
}

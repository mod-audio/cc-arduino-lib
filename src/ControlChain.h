#include <stdint.h>
#include "control_chain.h"

#define TX_DRIVER_PIN   2

class ControlChain {
    public:
        // to keep backward compatible
        void init() {
            begin();
        }

        void begin();
        void run();

        cc_device_t* newDevice(const char *name, const char *uri);
        cc_actuator_t* newActuator(cc_actuator_config_t *actuator_config);
        void addActuator(cc_device_t *device, cc_actuator_t *actuator);

    private:
        static void responseCB(void *arg);
};

#ifndef CONFIG_H
#define CONFIG_H

// define firmware version
#define CC_FIRMWARE_MAJOR   0
#define CC_FIRMWARE_MINOR   4
#define CC_FIRMWARE_MICRO   1

////////// Arduino Uno
#ifdef ARDUINO_AVR_UNO

// maximum number of devices that can be created
#define CC_MAX_DEVICES      1
// maximum number of actuators that can be created per device
#define CC_MAX_ACTUATORS    4
// maximum number of assignments that can be created per actuator
#define CC_MAX_ASSIGNMENTS  1

// define the size of the queue used to store the updates before send them
#define CC_UPDATES_FIFO_SIZE    10

// disable string support
#define CC_STRING_NOT_SUPPORTED

////////// Arduino Due
#elif defined (ARDUINO_SAM_DUE)

// maximum number of devices that can be created
#define CC_MAX_DEVICES      1
// maximum number of actuators that can be created per device
#define CC_MAX_ACTUATORS    8
// maximum number of assignments that can be created per actuator
#define CC_MAX_ASSIGNMENTS  1

// define the size of the queue used to store the updates before send them
#define CC_UPDATES_FIFO_SIZE    20

////////// All other Arduinos
#else

// maximum number of devices that can be created
#define CC_MAX_DEVICES      1
// maximum number of actuators that can be created per device
#define CC_MAX_ACTUATORS    2
// maximum number of assignments that can be created per actuator
#define CC_MAX_ASSIGNMENTS  1

// define the size of the queue used to store the updates before send them
#define CC_UPDATES_FIFO_SIZE    5

// disable string support
#define CC_STRING_NOT_SUPPORTED

#endif

#endif

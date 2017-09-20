# Control Chain Arduino Library

## Installation

#### Dependencies

* Arduino IDE v1.5 or greater
* TimerOne (required for Arduino Uno)
* DueTimer (required for Arduino Due)

See [here](https://www.arduino.cc/en/guide/libraries#toc3) how to install the dependencies
libraries using the Library Manager.

#### This library

Go to [releases](https://github.com/moddevices/cc-arduino-lib/releases) and download
the last version of the **ControlChain.zip** file.
After download it, check [this](https://www.arduino.cc/en/guide/libraries#toc4)
Arduino documentation to see how to import a new library.

## Supported boards

This library has been tested only with the Arduino Uno but it should work for most of AVR
boards as Mega, Mini, Nano, Ethernet and LilyPad.

It is also possible to use the library with the Arduino Due, however there is a slight change
that need to be done on the Arduino's code. See below how to do it.

#### Fixing Arduino Due code

The fix is necessary because the Arduino core does not allow users to control interruptions by
themselves. This library need to control the UART interruption in order to work properly and also
to have the code as optimized as possible.

There is an ongoing [pull request](https://github.com/arduino/ArduinoCore-sam/pull/1) which add
such functionality.

You can check [here](https://github.com/arduino/ArduinoCore-sam/pull/1/files)
and manually apply the required changes while the pull request isn't merged to the main stream.

You will find that file in the Arduino installation folder, which depends on the OS you have. Once
you find out, the path to the file should be something similar to
**packages/hardware/sam/1.6.11/variants/arduino_due_x/variant.cpp**.

## How to use

Once the library is installed, go to `File > Examples > Control Chain`, choose one
of the examples and follow the instructions on the header of the file.

## Taken resources (peripherals)

This library make use of the pins 0 to 2, where pins 0 and 1 are used as `Serial` and
pin 2 as digital I/O. Timer 1 is also used by this library.

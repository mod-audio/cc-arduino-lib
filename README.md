# Control Chain Arduino Library

## Installation

#### Dependencies

* Arduino IDE v1.5 or greater
* [TimerOne](http://playground.arduino.cc/Code/Timer1) (required for Arduino Uno)
* [DueTimer](https://github.com/ivanseidel/DueTimer) (required for Arduino Due)

See [here](https://www.arduino.cc/en/guide/libraries#toc3) how to install those
libraries using the Library Manager.

#### This library

Go to [releases](https://github.com/moddevices/cc-arduino-lib/releases) and download
the last version of the **ControlChain.zip** file.
After download it, check [this](https://www.arduino.cc/en/guide/libraries#toc4)
Arduino documentation to see how to import a new library.

## How to use

Once the library is installed, go to `File > Examples > Control Chain`, choose one
of the examples and follow the instructions on the header of the file.

## Taken resources (peripherals)

This library make use of the pins 0 to 2, where pins 0 and 1 are used as `Serial` and pin 2 as digital I/O. Timer 1 is also used by this library.

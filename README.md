# Control Chain Arduino Library

This library is an implementation of the Control Chain protocol for Arduino platforms. The Control
Chain Arduino Library along with the Control Chain Arduino shield allow you to create your own
controller devices and use them on MOD devices (e.g.: MOD Duo). Read this blog
[post](https://www.moddevices.com/blog/2017/05/06/tutorial-arduino-control-chain) for more
information.

## Installation

#### Dependencies

* Arduino IDE v1.5 or newer
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
**packages/arduino/hardware/sam/1.6.11/variants/arduino_due_x/variant.cpp**.

## How to use

Once the library is installed, go to `File > Examples > Control Chain`, choose one
of the examples and follow the instructions on the header of the file.

## Used resources (peripherals)

This library make use of the pins 0 to 2, where pins 0 and 1 are used as `Serial` and
pin 2 as digital I/O. Timer 1 is also used by this library. This means that those resources
won't be available for you to use in your application.

## Debugging your device

For any new project is very usual that a design or implementation does not work out and you have to
spend many hours debugging your setup. The steps described here aims to help you to understand
the complete workflow of a Control Chain device in order to enable you to isolate the source of the
problem and debug it properly.

```
+----------------------------------------
| 1. Software (CC Device)
+----------------------------------------
|   a) Application code (aka, your code)
|   b) Control Chain library
|   c) Arduino core code
+----------------------------------------
| 2. Hardware
+----------------------------------------
|   a) Arduino platform
|   b) Arduino shield
|   c) your circuit
|   d) patch cable
|   e) MOD Duo
+----------------------------------------
| 3. Software (MOD Duo)
+----------------------------------------
|   a) cc-master
|   b) mod-host | mod-ui
+----------------------------------------
```

Before anything else make sure you have the most recent version of the Control Chain library and the
MOD software to make sure you are not facing a bug that was already been fixed and released.

Now, the first step is to check if the 'reference test setup' is working. Upload the Pot example
code to the Arduino Uno and connect it to the MOD Duo. Make sure you see the popup message showing
that the device was connected/disconnected and that the assignment is working. Even you are not
using the Uno, do execute the same test for the platform you are using.

If the test has failed the next step is to change the setup eliminating a few items showed in the
previous diagram and redo the test. As you might have noted the 'reference test setup' already
eliminates the item 1a, which in general is the main source of problems, your code (no offenses).

This next test eliminates the items: 1a, 2b, 2c, 2d, 2e and 3b. The only hardware items here are the
Arduino and your laptop. Make sure to physically remove the Control Chain shield before proceed.
To execute this test, install and run the [cc-master](https://github.com/moddevices/cc-master)
in your laptop. Adjust the serial port as needed and enable the debug mode as following.

```bash
export LIBCONTROLCHAIN_DEBUG=2
controlchaind /dev/ttyACM0 -f
```

If everything is good, you should see a few messages in the terminal showing that a Control Chain
device was connected. If you don't see them, try to swap your Arduino for another one, remember that
you are supposed to be running the Pot example code. If it's still not working, although there are a
few other elements in the setup as USB cable, arduino core code and cc-master, is more likely to be
a bug in the Control Chain library. In that case please make a bug report stating the Arduino
platform you are using and also that you have followed and executed the tests described here.

# Semi-Autonomous Vehicle with Driving Assistant

**Bachelor’s Degree Project – Faculty of Mathematics and Computer Science**

## Project Overview

This repository contains my Bachelor’s degree project developed at the Faculty of Mathematics and Computer Science. The project represents a semi-autonomous vehicle system composed of four fully integrated components, all designed and built by me:

* The vehicle
* The remote controller
* The driving track
* Traffic signs

The goal of the project is to demonstrate the integration of hardware, embedded systems, radio communication, and computer vision into a functional semi-autonomous driving assistant.

## Vehicle Design and Hardware Architecture

**Chassis and Steering Mechanism**

The vehicle chassis is entirely hand-built from wooden plywood. On top of the chassis, I mounted an Ackermann steering mechanism, fully designed by me in Fusion 360 and later 3D printed.
The steering system is controlled by an MG996R 180° servo motor, mounted at the center of the chassis, ensuring precise left–right steering control.

**Drive System**

The vehicle uses four low-speed, high-torque DC motors to provide sufficient traction, as the overall weight of the vehicle requires increased pulling force.
To control these motors efficiently, I used two L298N motor drivers:

* One mounted at the front of the vehicle
* One mounted at the rear

**Sensors and Communication**

The vehicle is equipped with two infrared (IR) sensors, mounted on the front left and right sides. These sensors detect color changes from white to black and vice versa, allowing the system to identify lane boundaries and pedestrian crossings.

For wireless communication, I used an NRF24L01 radio module, which transmits traffic-related data from the vehicle to the remote controller.

All vehicle components are managed by an Arduino Mega, which acts as the main controller.

**Computer Vision System**

The vehicle also includes a Raspberry Pi 5, connected to a front-facing camera. The camera streams real-time video and enables high-accuracy detection of four traffic signs:

* STOP
* Priority Road
* Yield (Give Way)
* Pedestrian Crossing

The Raspberry Pi 5 is powered via USB Type-C from an external power bank. The same power bank supplies the Arduino Mega through a USB A–B cable, which is also used for serial communication to transmit detected traffic signs from the Raspberry Pi to the Arduino.

**Power Supply**

Servo motor: powered by 6 rechargeable AA batteries

Each L298N motor driver: powered by 4 AA batteries

Raspberry Pi & Arduino Mega: powered by an external battery

## Remote Controller

The remote controller is built around an Arduino Uno, powered by 4 AA batteries mounted inside the controller casing.

**Input Controls**

Two joysticks:

* One for left–right steering
* One for forward–backward movement

Joystick commands are transmitted wirelessly to the vehicle using an NRF24L01 radio module.

**Driving Assistant Control**

The right joystick button is used to toggle the Driving Assistant:

* ON
* OFF

This state is also transmitted wirelessly to the vehicle.

**Display**

An I2C LCD display is mounted on the remote controller and shows:

* Detected traffic signs
* Driving assistant intervention messages
* Error or warning information when the assistant intervenes

## Track and Traffic Signs

**Driving Track**

The track is constructed from cardboard pieces fixed together with tape, over which I applied black cardboard sheets to simulate asphalt.

* Lane markings are made using white insulating tape
* A pedestrian crossing is included to allow the driving assistant to recognize stopping zones

**Traffic Signs**

* Sign supports are made from bamboo sticks
* Sign panels are color-printed and glued manually

## System Functionality

**Startup**

Pressing the power button on the remote controller starts the Arduino Uno and initializes bidirectional data exchange between the remote and the vehicle.

**Manual Mode (Assistant OFF)**

* The vehicle is controlled 100% manually via the remote controller
* The driving assistant does not intervene, regardless of lane departure or traffic signs

**Assisted Mode (Assistant ON)**

When the assistant is active, the vehicle is still manually guided, but the system intervenes in critical situations:

**Lane Departure**

* If the vehicle exits the lane, the IR sensors detect the boundary
* The assistant immediately stops the vehicle
* To resume movement:
 * The assistant must be turned OFF
 * The vehicle repositioned
 * The assistant can then be reactivated while driving

**STOP Sign**

* If a STOP sign is detected and the driver does not stop voluntarily, the assistant forcibly stops the vehicle after passing the sign
* Restarting requires disabling the assistant
* If the driver stops before the sign, the assistant does not intervene, considering the situation safe

**Pedestrian Crossing**

* When the assistant is ON, the vehicle stops before the crossing 
* The stop is triggered when IR sensors detect the white boundary line

**Other Traffic Signs**

* Priority Road and Yield signs are detected with high accuracy
* These signs do not trigger special behavior, but are displayed on the remote controller

**Feedback and Error Reporting**

* Detected traffic signs are sent wirelessly to the remote controller
* When the assistant intervenes, an error message is transmitted
* The LCD display shows the reason for the intervention

![documentatie licenta](https://github.com/user-attachments/assets/629061a4-d30e-4030-865c-7c0606a545ff)

video: https://drive.google.com/drive/u/0/folders/10o-34vNvR9-_KFNNf6wCC1isO5EidKFW

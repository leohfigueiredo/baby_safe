# baby_safe
The Project was developed from the challenge of creating a product applying software engineering methods and techniques, for the IoT discipline

# Child Presence Detection System

This repository contains the code for a Child Presence Detection System implemented using an ESP32 microcontroller and various sensors. The system is designed to prevent instances of leaving a child unattended inside a car by providing alerts and notifications to the driver.

## Features

- Uses an ultrasonic sensor to measure the distance between the child and the car seat.
- Detects the presence of a child and the driver's state using a button and an LED indicator.
- Sends notifications to the driver's phone using the Blynk IoT platform.
- Provides visual alerts through different LED colors to indicate the system's state.
- Includes a buzzer for audio alerts in case of emergency.

## Requirements

- ESP32 microcontroller board
- Ultrasonic sensor (HC-SR04)
- Button and LED indicators
- Buzzer
- Arduino IDE

## Installation

1. Clone or download this repository to your local machine.
2. Open the Arduino IDE and ensure that you have the ESP32 board manager installed.
3. Connect the ESP32 board to your computer.
4. Open the `child_presence_detection_system.ino` file in the Arduino IDE.
5. Modify the configuration variables in the code, such as Wi-Fi credentials and Blynk authentication token.
6. Compile and upload the code to the ESP32 board.
7. Follow the wiring instructions in the code comments to connect the sensors and indicators to the ESP32 board.
8. Power on the system and observe the LED and buzzer behavior based on the child's presence and the driver's state.

## Usage

1. When the child is detected in the car seat, the system will monitor the distance using the ultrasonic sensor.
2. The LED indicator will change color depending on the distance and the state of the driver.
3. If the child is left unattended in the car, the system will send a notification to the driver's phone using the Blynk platform and activate the buzzer for audio alerts.
4. The system will continuously monitor the child's presence and update the LED and buzzer status accordingly.

## Contributing

Contributions to improve and enhance the Child Presence Detection System are welcome! If you have any ideas, suggestions, or bug reports, please open an issue or submit a pull request.

Feel free to modify and customize the README according to your needs and additional information about your project.

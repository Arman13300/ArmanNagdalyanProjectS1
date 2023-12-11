# ArmanNagdalyanProjectS1
# Smart Car Security and Monitoring System

## Overview

This project implements a Smart Car Security and Monitoring System in C. The system includes features such as random sensor event generation, alarm responses, and basic car functionalities like engine control, trunk manipulation, and system diagnostics.

## Features

- Sensor Event Generation: Simulates random sensor events, such as door/window/motion/tyre/battery/hijack sensors activation.

- Alarm Responses: Responds to sensor events by triggering appropriate actions, such as sounding the siren, turning on headlights, and sending notifications.

- Car Functionality: Provides basic car functionalities like arming/disarming the system, starting/stopping the engine, and opening/closing the trunk.

- System Diagnostics: Allows users to check tire pressure, fuel level, car diagnostics, and engine temperature.

## Project Structure

The project is divided into two main parts:

1. car_alarm_event_generator.c: Generates random sensor events at specified intervals and logs them to a file (events.txt).

2. car_alarm_system_controller.c: Controls the car alarm system, processes events, and performs actions based on user input. It also provides a menu for system operations and diagnostics.

## How to Use

1. Compile the Code: Compile both car_alarm_event_generator.c and car_alarm_system_controller.c using a C compiler (e.g., gcc).

2. Run the Programs: Execute the compiled binaries to run the event generator and the system controller.

3. Interact with the System: Use the interactive menu to perform various operations, such as arming/disarming the system, starting/stopping the engine, and checking system diagnostics.

4. Review Event Responses: Check the answers.txt file to see the system's responses to generated sensor events.

## Dependencies

- The code is written in C and requires a C compiler for compilation.

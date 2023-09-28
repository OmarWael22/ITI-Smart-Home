# Smart Home System with AVR Microcontroller

![Project Image](project_image.jpg) <!-- Replace with an image representing your project, if available -->

## Description

This project is a Smart Home System implemented using an AVR microcontroller (Atmega32). It consists of three main components:

1. **Locker System with EEPROM**: Allows users to set and enter a password to control access to a locker. Passwords are saved in EEPROM.

2. **Bluetooth-Controlled Lights**: Uses UART communication to control room lights via a Bluetooth module. Users can turn lights on and off for different rooms.

3. **Temperature-Based Fan and AC Control**: Monitors room temperature using a heat sensor and controls a fan and AC unit accordingly.

## Key Features

- C programming proficiency for embedded systems.
- AVR microcontroller interfacing expertise.
- Communication protocols: SPI, I2C, UART.
- Password-protected locker system.
- Bluetooth control for room lights.
- Temperature-based fan and AC control.

## Usage

1. **Locker System**:
   - To set a new password, enter your old password and press 'clear.'
   - To enter the locker, input the correct password and press 'enter.'

2. **Bluetooth-Controlled Lights**:
   - Use the format `*i*x*x#` to control room lights.
   - Replace `i` with room number (1, 2, or 3).
   - Replace `x` with light state (0 for off, 1 for on).

3. **Temperature-Based Fan and AC Control**:
   - Monitors temperature and controls a fan and AC unit.

## Setup

- Configure the AVR microcontroller environment.
- Connect the required components, including sensors, lights, and the Bluetooth module.
- Flash the program onto the microcontroller.
- Run the system.

## License

This project is open-source and available under the [MIT License](LICENSE).

## Acknowledgments

Special thanks to [Your Name] for their contributions to this project.


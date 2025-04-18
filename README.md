# Can_car-s-diagnostic_STM32_MCP2515

 MCP2515 CAN Driver for STM32F103C8T6

This project demonstrates how to interface an STM32F103C8T6 (Blue Pill) with the MCP2515 CAN controller using SPI. It includes:

- Clean and modular `C` drivers
- Initialization, transmit, and receive CAN messages
- Ready for integration in Keil uVision projects

---

 Features

- SPI communication with MCP2515
- Send standard 11-bit CAN frames
- Receive and decode messages from RXB0
- Configurable speed (default: 500kbps @ 16MHz)

---

 Files

| File        | Description |
|-------------|-------------|
| `mcp2515.h` | Header file with commands and function declarations |
| `mcp2515.c` | MCP2515 driver implementation |
| `main.c`    | Sample usage: init + transmit + receive |
| `README.md` | You're reading it! |

---

Hardware Setup

- MCU: STM32F103C8T6 (Blue Pill)
- MCP2515 CAN Module with TJA1050
- Wiring:

| STM32       | MCP2515   |
|-------------|-----------|
| PA5 (SCK)   | SCK       |
| PA6 (MISO)  | SO        |
| PA7 (MOSI)  | SI        |
| PA4 (GPIO)  | CS        |
| 3.3V        | VCC       |
| GND         | GND       |

---

 Configuration

- SPI: SPI1
- CS:  PA4 (can be changed)
- CAN Speed: 500 kbps (set via CNF1/2/3)

---

 Example

```c
uint8_t data[] = {0x01, 0x02, 0x03};
MCP2515_SendMessage(0x7DF, data, 3);

uint8_t buffer[8];
uint32_t id;
if (MCP2515_ReceiveMessage(buffer, &id)) {
    // process buffer
}


Build Environment
IDE: Keil uVision 5
MCU Pack: STM32F1 Series
Clock: 72 MHz, 16MHz crystal

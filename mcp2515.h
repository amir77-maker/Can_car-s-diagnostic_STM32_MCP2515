#ifndef MCP2515_H
#define MCP2515_H

#include "stm32f10x.h"

// === MCP2515 SPI Command Set ===
#define MCP_RESET       0xC0
#define MCP_READ        0x03
#define MCP_WRITE       0x02
#define MCP_RTS_TX0     0x81
#define MCP_READ_STATUS 0xA0
#define MCP_BIT_MODIFY  0x05

// === MCP2515 Register Addresses ===
#define CANCTRL         0x0F
#define CNF1            0x2A
#define CNF2            0x29
#define CNF3            0x28
#define RXB0CTRL        0x60
#define RXB0SIDH        0x61
#define TXB0SIDH        0x31
#define TXB0SIDL        0x32
#define TXB0DLC         0x35
#define TXB0D0          0x36
#define CANINTF         0x2C

// === MCP2515 Functions ===
void MCP2515_Reset(void);
uint8_t MCP2515_Read(uint8_t address);
void MCP2515_Write(uint8_t address, uint8_t data);
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data);
void MCP2515_Init(void);
void MCP2515_SendMessage(uint32_t id, uint8_t* data, uint8_t len);
uint8_t MCP2515_ReceiveMessage(uint8_t* data, uint32_t* id);

#endif

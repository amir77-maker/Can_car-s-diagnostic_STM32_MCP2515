#include "mcp2515.h"

// Define Chip Select Pin (CS - PA4)
#define MCP_CS_HIGH()   GPIOA->BSRR = GPIO_BSRR_BS4
#define MCP_CS_LOW()    GPIOA->BRR  = GPIO_BRR_BR4

// External SPI instance
extern SPI_TypeDef *CAN_SPI;

// Write a byte via SPI
void SPI_Write(uint8_t data) {
    while (!(CAN_SPI->SR & SPI_SR_TXE));
    CAN_SPI->DR = data;
    while (!(CAN_SPI->SR & SPI_SR_RXNE));
    (void)CAN_SPI->DR; // Clear RX buffer
}

// Send and receive a byte simultaneously (Full Duplex)
uint8_t SPI_ReadWrite(uint8_t data) {
    while (!(CAN_SPI->SR & SPI_SR_TXE));
    CAN_SPI->DR = data;
    while (!(CAN_SPI->SR & SPI_SR_RXNE));
    return (uint8_t)CAN_SPI->DR;
}

// Reset MCP2515
void MCP2515_Reset(void) {
    MCP_CS_LOW();
    SPI_Write(MCP_RESET);
    MCP_CS_HIGH();
}

// Read a register from MCP2515
uint8_t MCP2515_Read(uint8_t address) {
    MCP_CS_LOW();
    SPI_Write(MCP_READ);
    SPI_Write(address);
    uint8_t data = SPI_ReadWrite(0xFF);
    MCP_CS_HIGH();
    return data;
}

// Write a register in MCP2515
void MCP2515_Write(uint8_t address, uint8_t data) {
    MCP_CS_LOW();
    SPI_Write(MCP_WRITE);
    SPI_Write(address);
    SPI_Write(data);
    MCP_CS_HIGH();
}

// Modify specific bits in a register
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data) {
    MCP_CS_LOW();
    SPI_Write(MCP_BIT_MODIFY);
    SPI_Write(address);
    SPI_Write(mask);
    SPI_Write(data);
    MCP_CS_HIGH();
}

// Initialize MCP2515 in Normal Mode (500kbps @ 16MHz)
void MCP2515_Init(void) {
    MCP2515_Reset();

    // CAN Timing Configuration
    MCP2515_Write(CNF1, 0x00); // BRP=0 (16 TQ), SJW=1 TQ
    MCP2515_Write(CNF2, 0x90); // BTLMODE=1, PHSEG1=3 TQ, PRSEG=1 TQ
    MCP2515_Write(CNF3, 0x02); // PHSEG2=3 TQ

    // Enable normal mode
    MCP2515_Write(CANCTRL, 0x00);

    // Enable receive buffer to accept all messages
    MCP2515_Write(RXB0CTRL, 0x60);
}

// Send a standard CAN message using TXB0
void MCP2515_SendMessage(uint32_t id, uint8_t* data, uint8_t len) {
    uint8_t sid_high = (id >> 3) & 0xFF;
    uint8_t sid_low  = (id << 5) & 0xE0;

    MCP2515_Write(TXB0SIDH, sid_high);
    MCP2515_Write(TXB0SIDL, sid_low);
    MCP2515_Write(TXB0DLC, len & 0x0F);

    for (int i = 0; i < len; i++) {
        MCP2515_Write(TXB0D0 + i, data[i]);
    }

    MCP_CS_LOW();
    SPI_Write(MCP_RTS_TX0);
    MCP_CS_HIGH();
}

// Receive a CAN message if available
uint8_t MCP2515_ReceiveMessage(uint8_t* data, uint32_t* id) {
    if ((MCP2515_Read(CANINTF) & 0x01) == 0) return 0;

    uint8_t sidh = MCP2515_Read(RXB0SIDH);
    uint8_t sidl = MCP2515_Read(RXB0SIDH + 1);
    *id = (sidh << 3) | (sidl >> 5);

    uint8_t len = MCP2515_Read(RXB0SIDH + 4) & 0x0F;

    for (int i = 0; i < len; i++) {
        data[i] = MCP2515_Read(RXB0SIDH + 5 + i);
    }

    MCP2515_BitModify(CANINTF, 0x01, 0x00); // Clear RX0IF flag

    return len;
}

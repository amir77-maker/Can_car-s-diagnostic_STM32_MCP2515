#include "mcp2515.h"

SPI_TypeDef *CAN_SPI = SPI1; // Define which SPI to use (SPI1)

// Initialize SPI1 peripheral
void SPI1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI |
                SPI_CR1_BR_1 | SPI_CR1_SPE;
}

// Initialize GPIOA pin PA4 for CS
void GPIO_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // PA4 as push-pull output (CS)
    GPIOA->CRL &= ~GPIO_CRL_CNF4;
    GPIOA->CRL |= GPIO_CRL_MODE4_0;
    MCP_CS_HIGH();
}

int main(void) {
    GPIO_Init();
    SPI1_Init();
    MCP2515_Init();

    uint8_t tx_data[] = {0xDE, 0xAD, 0xBE, 0xEF};
    MCP2515_SendMessage(0x123, tx_data, 4);

    while (1) {
        uint8_t rx_data[8];
        uint32_t id;
        uint8_t len = MCP2515_ReceiveMessage(rx_data, &id);
        if (len > 0) {
            // Process or print received CAN frame here
            // For example: send via UART or toggle LED
        }
    }
}

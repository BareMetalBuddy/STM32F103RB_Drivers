#ifndef USART_H_
#define USART_H_

#include <stdint.h>
#include <string.h>

#define USART1_BASE_ADDRESS (0x40013800)
#define USART2_BASE_ADDRESS (0x40004400)
#define USART3_BASE_ADDRESS (0x40004800)

typedef struct{
	volatile uint32_t USART_SR;
	volatile uint32_t USART_DR;
	volatile uint32_t USART_BRR;
	volatile uint32_t USART_CR1;
	volatile uint32_t USART_CR2;
	volatile uint32_t USART_CR3;
	volatile uint32_t USART_GTPR;
}USART_Reg_t;

typedef struct{
	uint16_t baudrate;
	uint8_t parity;
	uint8_t usart_mode;
	uint8_t data_bit;
	uint8_t stop_bit;
	USART_Reg_t *usart;
}USART_Handler_t;


void usart_init(USART_Handler_t *usart_handler,uint32_t base_address, uint16_t baudrate, uint8_t parity, uint8_t usart_mode, uint8_t data_bit, uint8_t stop_bit);
void usart_config(USART_Handler_t *usart_handler);
void usart_tx_frame(USART_Handler_t *usart_handler, uint8_t * frame);
static void usart_tx(USART_Handler_t *usart_handler, uint8_t data);


#endif /* USART_H_ */

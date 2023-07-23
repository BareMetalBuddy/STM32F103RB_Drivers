#include "usart.h"


void usart_init(USART_Handler_t *usart_handler,uint32_t base_address,uint16_t baudrate, uint8_t parity, uint8_t usart_mode, uint8_t data_bit, uint8_t stop_bit)
{


}
void usart_config(USART_Handler_t *usart_handler)
{

}

static void usart_tx(USART_Handler_t *usart_handler, uint8_t data)
{

}


void usart_tx_frame(USART_Handler_t *usart_handler, uint8_t * frame)
{
	for (uint8_t j = 0; j < strlen((const char * )frame); j++){
		usart_tx(usart_handler,*(frame + j));
	}
}



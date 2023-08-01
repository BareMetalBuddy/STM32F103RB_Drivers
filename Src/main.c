#include <stdint.h>
#include "timx.h"
#include "gpio.h"
#include "rcc.h"
#include "nvic.h"
#include "usart.h"
#include "bsp.h"


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

GPIO_Handler_t status_led;
GPIO_Handler_t navigation_led;

GPIO_Handler_t modem_tx;
GPIO_Handler_t modem_rx;
GPIO_Handler_t cli_tx;
GPIO_Handler_t cli_rx;

TIMx_Handler_t timx_handler;
USART_Handler_t usart2;
USART_Handler_t usart1;

int main(void)
{


	RCC_GPIOA_CLOCK_ENABLE();

	gpio_init(&status_led,GPIOA_BASE_ADDRESS,GPIO_pin_5,GPIO_General_purpose_output_push_pull,GPIO_Output_mode_max_speed_10_MHz);
	gpio_config(&status_led);

	gpio_init(&navigation_led,GPIOA_BASE_ADDRESS,GPIO_pin_1,GPIO_General_purpose_output_push_pull,GPIO_Output_mode_max_speed_10_MHz);
	gpio_config(&navigation_led);

	gpio_write_pin(&status_led,0);
	gpio_write_pin(&navigation_led,0);

	RCC_TIM2_CLOCK_ENABLE();

	timx_init(&timx_handler,TIM2_BASE_ADDRESS,TIM2_PRESCALER_VALUE_FOR_100MS,TIM2_RELOAD_VALUE_FOR_100MS); // values for 100ms
	timx_config(&timx_handler);
	timx_update_interrupt(&timx_handler, 1);

	// in order to configure pins TX and RX , the selection for the register GPIOx_CRL and GPIOx_CRH have to be:
	// TX -> alternative function push pull
	// RX -> input float or input pull up
	gpio_init(&modem_tx,GPIOA_BASE_ADDRESS,GPIO_pin_2,GPIO_Alternate_function_output_Push_pull,GPIO_Output_mode_max_speed_10_MHz);
	gpio_config(&modem_tx);
	gpio_init(&modem_rx,GPIOA_BASE_ADDRESS,GPIO_pin_3,GPIO_Floating_input,GPIO_Input_mode);
	gpio_config(&modem_rx);

	RCC_USART2_CLOCK_ENABLE();  // enable clock access to UART2
	usart_init(&usart2,USART2_BASE_ADDRESS,BAUDRATE,0,0,0,0);
	usart_config(&usart2);

	gpio_init(&cli_tx,GPIOA_BASE_ADDRESS,GPIO_pin_9,GPIO_Alternate_function_output_Push_pull,GPIO_Output_mode_max_speed_10_MHz);
	gpio_config(&cli_tx);
	gpio_init(&cli_rx,GPIOA_BASE_ADDRESS,GPIO_pin_10,GPIO_Floating_input,GPIO_Input_mode);
	gpio_config(&cli_rx);

	RCC_USART1_CLOCK_ENABLE();  // enable clock access to USART1
	usart_init(&usart1,USART1_BASE_ADDRESS,BAUDRATE,0,0,0,0);
	usart_config(&usart1);
	usart_interrupt_enable(&usart1);

	NVIC_SET_INTERRUPT_PRIORITY_USART1(2);
	NVIC_ENABLE_INTERRUPT_USART1();

	uint8_t input_data_from_user[32];
	uint8_t index = 0;
	input_data_from_user[index] = '\0';

	//uint8_t key;

    while(1)
    {
    	while(usart_available(&usart1)>0){
    		input_data_from_user[index] = usart_read(&usart1);
    		if(input_data_from_user[index] != '\r'){
    			index++;
    		}
    		else
    		{
    			input_data_from_user[index] = '\0';
    			index = 0;
    			usart_tx_frame(&usart1,input_data_from_user);
    		}
    	}

    	/*key = usart_rx(&usart1);
    	if(key== '1'){
    		gpio_toogle_pin(&status_led);
    	}*/

    	/*if((timx_handler.timx->TIMx_SR) & ((uint16_t)(0x0001))){
    		timx_handler.timx->TIMx_SR &= ~((uint16_t)(0x0001));
    		gpio_toogle_pin(&status_led);
    		usart_tx_frame(&usart2, "Hello From Usart2");
    		usart_tx_frame(&usart1, "Hello From Usart1");
    	}*/
    }
}

void USART1_IRQHandler()
{
	usart_receive(&usart1);
}


#include <stdint.h>
#include <timx.h>
#include "gpio.h"
#include "rcc.h"
#include "nvic.h"
#include "usart.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

GPIO_Handler_t status_led;
GPIO_Handler_t navigation_led;
TIMx_Handler_t timx_handler;

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

	timx_init(&timx_handler,TIM2_BASE_ADDRESS,31999, 24); // values for 100ms
	timx_config(&timx_handler);
	timx_update_interrupt(&timx_handler, 1);

    while(1)
    {
    	if((timx_handler.timx->TIMx_SR) & ((uint16_t)(0x0001))){
    		timx_handler.timx->TIMx_SR &= ~((uint16_t)(0x0001));
    		gpio_toogle_pin(&status_led);
    	}
    }
}

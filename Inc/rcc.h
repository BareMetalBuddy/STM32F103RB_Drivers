
#ifndef DRIVERS_INCLUDE_RCC_H_
#define DRIVERS_INCLUDE_RCC_H_

#include <stdint.h>

typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;

}RCC_Reg_t;

#define RCC_BASE_ADDRESS (0x40021000)
#define RCC_AHBENR  (RCC_BASE_ADDRESS + 0x14)
#define RCC_APB1ENR (RCC_BASE_ADDRESS + 0x1C)
#define RCC_APB2ENR (RCC_BASE_ADDRESS + 0x18)

// GPIO's enable clock
#define RCC_GPIOA_CLOCK_ENABLE() (*((uint32_t*)RCC_APB2ENR) |= (1<<2))
#define RCC_GPIOB_CLOCK_ENABLE() (*((uint32_t*)RCC_APB2ENR) |= (1<<3))
#define RCC_GPIOC_CLOCK_ENABLE() (*((uint32_t*)RCC_APB2ENR) |= (1<<4))
#define RCC_GPIOD_CLOCK_ENABLE() (*((uint32_t*)RCC_APB2ENR) |= (1<<5))
#define RCC_GPIOE_CLOCK_ENABLE() (*((uint32_t*)RCC_APB2ENR) |= (1<<6))

// Basic timers (TIM6 and TIM7) enable clock
#define RCC_TIM6_CLOCK_ENABLE() (*((uint32_t*)RCC_APB1ENR) |=(1<<4))
#define RCC_TIM7_CLOCK_ENABLE() (*((uint32_t*)RCC_APB1ENR) |=(1<<5))
#define RCC_TIM2_CLOCK_ENABLE() (*((uint32_t*)RCC_APB1ENR) |=(1<<0))

// USARTs enable clock
#define RCC_USART1_CLOCK_ENABLE() (*((uint32_t*)RCC_APB2ENR) |=(1<<14))
#define RCC_USART2_CLOCK_ENABLE() (*((uint32_t*)RCC_APB1ENR) |=(1<<17))
#define RCC_USART3_CLOCK_ENABLE() (*((uint32_t*)RCC_APB1ENR) |=(1<<18))

// I2C bus enable clock
#define RCC_I2C1_CLOCK_ENABLE() (*((uint32_t*)RCC_APB1ENR) |=(1<<21))
#define RCC_I2C2_CLOCK_ENABLE() (*((uint32_t*)RCC_APB1ENR) |=(1<<22))

#endif /* DRIVERS_INCLUDE_RCC_H_ */

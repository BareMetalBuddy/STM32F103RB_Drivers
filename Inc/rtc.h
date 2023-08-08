#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

#define RTC_BASE_ADDRESS (0x40002800)

typedef struct{
	volatile uint32_t RTC_CRH;
	volatile uint32_t RTC_CRL;
	volatile uint32_t RTC_PRLH;
	volatile uint32_t RTC_PRLL;
	volatile uint32_t RTC_DIVH;
	volatile uint32_t RTC_DIVL;
	volatile uint32_t RTC_CNTH;
	volatile uint32_t RTC_CNTL;
	volatile uint32_t RTC_ALRH;
	volatile uint32_t RTC_ALRL;
}RTC_Reg;

typedef struct{
	volatile RTC_Reg * rtc;
}RTC_Handler;

void rtc_init(RTC_Handler* rtc_handler,uint32_t base_address);
void rtc_registers_synchronized_flag(RTC_Handler* rtc_handler);
void rtc_set_time(RTC_Handler* rtc_handler, uint32_t time);
uint32_t rtc_get_time(RTC_Handler* rtc_handler);

#endif /* RTC_H_ */

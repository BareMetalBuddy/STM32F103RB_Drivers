#include "rtc.h"

void rtc_init(RTC_Handler* rtc_handler, uint32_t base_address)
{
	rtc_handler->rtc = (RTC_Reg*)base_address;
}

/* Wait for Registers synchronized flag
0: Registers not yet synchronized.
1: Registers synchronized */
void rtc_registers_synchronized_flag(RTC_Handler* rtc_handler)
{
	rtc_handler->rtc->RTC_CRL &= ~(1<<3);
	while((rtc_handler->rtc->RTC_CRL & (1<<3))==0);
}

/*With this bit the RTC reports the status of the last write operation performed on its registers,
indicating if it has been completed or not. If its value is ‘0’ then it is not possible to write to any
of the RTC registers. This bit is read only.*/
/*
  0: Last write operation on RTC registers is still ongoing.
  1: Last write operation on RTC registers terminated.
*/
void rtc_set_time(RTC_Handler* rtc_handler, uint32_t time)
{
	while((rtc_handler->rtc->RTC_CRL & (1<<5)) == 0); 	// Wait for Last write operation on RTC registers terminated.
	rtc_handler->rtc->RTC_CRL |= (1<<4);   				// CNF: Configuration flag . Set to 1 to Enter configuration mode.

	rtc_handler->rtc->RTC_PRLH = 0x0000;
	rtc_handler->rtc->RTC_PRLL = 0x7FFF; // If clock 32.768 kHz, write 7FFFh in this register to get a signal period of 1 second.

	rtc_handler->rtc->RTC_CNTH = (uint16_t)(time >> 16);
	rtc_handler->rtc->RTC_CNTL = (uint16_t)(time & 0xFFFF);
	rtc_handler->rtc->RTC_CRL &= ~(1<<4); 					// Exit configuration mode (start update of RTC registers).

	while((rtc_handler->rtc->RTC_CRL & (1<<5)) == 0);
}

uint32_t rtc_get_time(RTC_Handler* rtc_handler)
{
	return ((uint32_t)(rtc_handler->rtc->RTC_CNTH<<16 | rtc_handler->rtc->RTC_CNTL));
}

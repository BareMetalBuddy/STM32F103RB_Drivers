
#ifndef PWR_H_
#define PWR_H_
#include <stdint.h>

#define PWR_BASE_ADDRESS (0x40007000)
#define PWR_CR (*(volatile uint32_t*)PWR_BASE_ADDRESS)
#define PWR_CSR (*(volatile uint32_t*)(PWR_BASE_ADDRESS + 0x04)

/*After reset, access to the Backup registers and RTC is disabled and the Backup domain
(BKP) is protected against possible parasitic write access. To enable access to the Backup
registers and the RTC, proceed as follows:

- enable the power and backup interface clocks by setting the PWREN and BKPEN bits
in the RCC_APB1ENR register
- set the DBP bit the Power Control register (PWR_CR) to enable access to the Backup
registers and RTC.*/

// set to 1 -> Access to RTC and Backup registers enabled
#define PWR_DISABLE_BKD_WR_PROTECTION()  (PWR_CR |= (1<<8))

#endif /* PWR_H_ */

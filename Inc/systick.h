#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

#define SYSTICK_BASE_ADDRESS (0xE000E010)
#define CLKSOURCE 	(2)
#define TICKINT 	(1)
#define ENABLE      (0)

typedef struct{
	volatile uint32_t SYST_CSR;
	volatile uint32_t SYST_RVR;
	volatile uint32_t SYST_CVR;
	volatile uint32_t SYST_CALIB;
}SYSTICK_Reg_t;





#endif /* SYSTICK_H_ */

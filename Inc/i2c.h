
#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>

#define I2C1_BASE_ADDRESS (0x40005400)
#define I2C2_BASE_ADDRESS (0x40005800)

typedef struct
{
	volatile uint32_t I2C_CR1;
	volatile uint32_t I2C_CR2;
	volatile uint32_t I2C_OAR1;
	volatile uint32_t I2C_OAR2;
	volatile uint32_t I2C_DR;
	volatile uint32_t I2C_SR1;
	volatile uint32_t I2C_SR2;
	volatile uint32_t I2C_CCR;
	volatile uint32_t I2C_TRISE;
}I2C_Reg_t;



#endif /* I2C_H_ */

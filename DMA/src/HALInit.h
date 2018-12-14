#ifndef __HALINIT_H_
#define __HALINIT_H_

void clockConfiguration(void);
void RCCInitialize(void);
void gpioInitialize(void);
void usartInitialize(void);
void spiInitailize(void);

/*sample code*/
uint8_t dataRead(void);
void wrDisable(void);
void wrEnable(void);
void rdDisable(void);
void rdEnable(void);

void FSMCLowSpeed();
void FSMCHighSpeed();

extern volatile unsigned long globalTimer;

void TIM2_settimer(void);
unsigned long TIM2_gettimer(void);

#endif

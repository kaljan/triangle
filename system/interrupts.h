#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);

#endif // INTERRUPTS_H

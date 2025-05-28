#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include <idtLoader.h>
#define PIC_MASTER_MASK_NULL 0xFF
#define PIC_MASTER_MASK_ONLY_TIMER 0xFE
#define PIC_MASTER_MASK_ONLY_KEYBOARD 0xFD
#define PIC_MASTER_MASK_ALL 0xFC

void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);

void _int80Handler(void);

void _exception0Handler(void);
void _exception6Handler(void);

void _cli(void);

void _sti(void);

void _hlt(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

//Termina la ejecución de la cpu.
void haltcpu(void);

void get_registers(uint64_t * regs);

#endif /* INTERRUPS_H_ */
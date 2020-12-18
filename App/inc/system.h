#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "stm32f446xx.h"
#include "types.h"

#define CLK_SRC_HSI 0
#define CLK_PLLM    0x08
#define CLK_PLLN    0x168
#define CLK_PLLP    0x04
#define CLK_PLLQ    0x0F
#define CLK_PLLR    0x04

#define AHB_DIVIDE_BY_2 0x04
#define AHB_DIVIDE_BY_4 0x05
#define AHB_NOT_DIVIDED 0x00
#define SYSCLK_SRC_PLLP 0x02

#define SYSTICK_1MS_LOAD 0x0002C410

enum SystemTimeConstants
{
    SYSTIME_1MS   = 1,
    SYSTIME_10MS  = 10,
    SYSTIME_100MS = 100,
    SYSTIME_500MS = 500,
    SYSTIME_1SEC  = 1000,
    SYSTIME_1MIN  = 60000,
    SYSTIME_1HR   = 3600000,
};

typedef enum TSystemState
{
    SYS_STATE_INIT,
    SYS_STATE_IDLE,
} TSystemState;


uint32_t SYS_GetSystemTime(void);
void SYS_InitClocks(uint32_t clkSource, bool pllOn);
void SYS_InitSysTick(uint32_t ticks);
void SYS_PeriphClkEn(volatile uint32_t * port, uint32_t periph, bool enable);

#endif

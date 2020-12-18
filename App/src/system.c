#include "system.h"


static void ConfigureCoreClock(uint32_t sysClkSrc, uint32_t ahbPre, uint32_t apb1Pre, uint32_t apb2Pre);
static void EnableCoreClock(uint32_t clkSrc, bool pllOn);
static void InitPlls(uint32_t pllSrc, uint32_t pllM, uint32_t pllN, uint32_t pllP, uint32_t pllQ, uint32_t pllR);
static void ResetCoreClock(void);
static void SetFlashLatency(uint32_t latency);


static void ConfigureCoreClock(uint32_t sysClkSrc, uint32_t ahbPre, uint32_t apb1Pre, uint32_t apb2Pre)
{
    //uint32_t mco2SrcReg   = (uint32_t)mco2Src << 30;
    //uint32_t mco2PreReg   = (uint32_t)mco2Pre << 27;
    //uint32_t mco1PreReg   = (uint32_t)mco1Pre << 24;
    //uint32_t mco1SrcReg   = (uint32_t)mco1Src << 21;
    //uint32_t rtcPreReg    = (uint32_t)rtcPre  << 16;
    uint32_t apb2PreReg   = (uint32_t)apb2Pre << 13;
    uint32_t apb1PreReg   = (uint32_t)apb1Pre << 10;
    uint32_t ahbPreReg    = (uint32_t)ahbPre  << 4;
    uint32_t sysClkSrcReg = (uint32_t)sysClkSrc;

    RCC->CFGR |= apb2PreReg | apb1PreReg | ahbPreReg | sysClkSrcReg;
    //RCC->CFGR |= mco2SrcReg | mco2PreReg | mco1PreReg | mco1SrcReg | rtcPreReg | apb2PreReg | apb1PreReg | ahbPreReg | sysClkSrcReg;
    //return ((uint8_t)(RCC->CFGR & RCC_CFGR_SWS_Msk) >> 2);
}

static void EnableCoreClock(uint32_t clkSrc, bool pllOn)
{
    uint32_t clkSrcReg = (clkSrc == CLK_SRC_HSI) ? BIT(0) : BIT(16);
    //uint32_t hseBypReg = (hseByp)                ? BIT(18) : 0x00000000;
    //uint32_t cssOnReg  = (cssOn)                 ? BIT(19) : 0x00000000;
    uint32_t pllReg    = (pllOn)                 ? BIT(24) : 0x00000000;
    //uint32_t i2sReg    = (i2sOn)                 ? BIT(26) : 0x00000000;
    //uint32_t saiReg    = (saiOn)                 ? BIT(28) : 0x00000000;
    RCC->CR |= clkSrcReg | pllReg;

    //RCC->CR |= clkSrcReg | hseBypReg | cssOnReg | pllReg | i2sReg | saiReg;
    if (pllOn)
    {
        while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY); // Wait for PLL to lock
    }
    //if (i2sOn)
    //{
    //    while ((RCC->CR & RCC_CR_PLLI2SRDY) != RCC_CR_PLLI2SRDY); // Wait for PLL to lock
    //}
    //if (saiOn)
    //{
    //    while ((RCC->CR & RCC_CR_PLLSAIRDY) != RCC_CR_PLLSAIRDY); // Wait for PLL to lock
    //}
}

// Configure PLLs based on input parameters
static void InitPlls(uint32_t pllSrc, uint32_t pllM, uint32_t pllN, uint32_t pllP, uint32_t pllQ, uint32_t pllR)
{
    uint32_t pllRReg   = (uint32_t)pllR << 28;
    uint32_t pllQReg   = (uint32_t)pllQ << 24;
    uint32_t pllSrcReg = (pllSrc == 0) ? 0x00000000 : BIT(22);
    uint32_t pllPReg   = ((uint32_t)(pllP - 1) / 2) << 16;
    uint32_t pllNReg   = (uint32_t)pllN << 6;
    uint32_t pllMReg   = (uint32_t)pllM;
    RCC->PLLCFGR = pllRReg | pllQReg | pllSrcReg | pllPReg | pllNReg | pllMReg;
}


static void ResetCoreClock(void)
{
    RCC->CR &= ~(RCC_CR_PLLSAION | RCC_CR_PLLI2SON | RCC_CR_PLLON | RCC_CR_CSSON | RCC_CR_HSEBYP | RCC_CR_HSEON | RCC_CR_HSION);
}


static void SetFlashLatency(uint32_t latency)
{
    FLASH->ACR &= (0xFFFFFFF0 | (uint32_t)latency); // Clear old bits
    FLASH->ACR |= (uint32_t)latency; // Set new bits
    while ((FLASH->ACR & 0x0000000F) != (uint32_t)latency);
}


void SYS_InitClocks(uint32_t clkSource, bool pllOn)
{
    ResetCoreClock();
    ConfigureCoreClock(SYSCLK_SRC_PLLP, AHB_NOT_DIVIDED, AHB_DIVIDE_BY_2, AHB_DIVIDE_BY_4);
    InitPlls(clkSource, CLK_PLLM, CLK_PLLN, CLK_PLLP, CLK_PLLQ, CLK_PLLR);
    SetFlashLatency(5);
    EnableCoreClock(clkSource, true);
}


void SYS_PeriphClkEn(volatile uint32_t * port, uint32_t periph, bool enable)
{
    *port = enable ? *port | BIT(periph) : *port & ~(BIT(periph));
}














/*
 * SYSTEM TICK Functions and Interrupt
 */
void SYS_InitSysTick(uint32_t ticks)
{
    SysTick_Config(ticks);
}


volatile uint32_t gSysTimer = 0; // 1ms Resolution rolls over at ~49.71 days
uint32_t SYS_GetSystemTime(void)
{
    __disable_irq();
    uint32_t tmp = gSysTimer;
    __enable_irq();
    return tmp;
}


// 1ms System Tick Interrupt
void SysTick_Handler(void)
{
    // Interrupts should execute extremely quickly, do not perform processing here.
    gSysTimer++;
}

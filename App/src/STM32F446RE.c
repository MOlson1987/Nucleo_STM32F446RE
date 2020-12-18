#include "system.h"
#include "types.h"

// Forward Declarations
void SystemFSM(void);
void TimerEvents(void);
void SystemEvents(void);


int main(void)
{
    while (1)
    {
        SystemFSM();
        TimerEvents();
        SystemEvents();
    }
}

/*
 * System Finite State Machine (FSM)
 * The System FSM controls the system, it must first initialize the system.
 * Following initialization, the System FSM will handle System and Timer events based on flags and priority.
 */
void SystemFSM(void)
{
    static TSystemState sysState = SYS_STATE_INIT;

    switch (sysState)
    {
        case SYS_STATE_INIT: // System requires initialization.
        {
            // Initialize CPU clock to 180MHz using PLL'd HSI clock.
            SYS_InitClocks(CLK_SRC_HSI, true);

            // Enable the SysTick Interrupt to run at a 1ms rate.
            // This will allow our timer to keep track of our System runtime.
            SYS_InitSysTick(SYSTICK_1MS_LOAD);

            // Initialization complete, goto IDLE.
            sysState = SYS_STATE_IDLE;
            break;
        }
        case SYS_STATE_IDLE: // System is idle.
        {
            // In this state we will be looking for flags set to perform System related tasks.
            break;
        }
        default: // Unknown System State, should never get here.
        {
            // If we get here, there is some fault in the System. HALT Operation.
            while (1);
        }
    }
}

/*
 * Timer Events (based on SysTick Timer which runs at a 1ms rate)
 */
void TimerEvents(void)
{
    // Maintain the second count for comparison.
    static uint32_t secondCount = 0;

    // Get the current System time.
    uint32_t sysTime = SYS_GetSystemTime();

    if ((sysTime / SYSTIME_1SEC) > secondCount) // Do this every second.
    {
        secondCount++;
    }

    if (!(secondCount % 60)) // Do this every minute.
    {

    }

    if (!(secondCount % 3600)) // Do this every hour.
    {

    }
}

/*
 * System Events
 */
void SystemEvents(void)
{

}

/**
 * timer: general-purpose timers(lm3s811)
 * Programmable timers can be used to count or time external events that
 * drive the Timer input pins. In addition, timers can be used to trigger 
 * analog-to-digital conversions (ADC). Other timer resources include the 
 * System Timer and the PWM timer in the PWM module.
 */

/**
 * Initialization and Configuration: 
 * To use the general-purpose timers, the peripheral clock must be enabled by 
 * setting the TIMER0, TIMER1, and TIMER2 bits in the RCGC1 register.
 * 
 * The GPTM is configured for 32-bit One-Shot and Periodic modes by
 * the following sequence:
 * 1. Ensure the timer is disabled (the TAEN bit in the GPTMCTL register
 *    is cleared) before making any changes.
 * 2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0.
 * 3. Set the TAMR field in the GPTM TimerA Mode Register (GPTMT AMR):
 *		a. Write a value of 0x1 for One-Shot mode.
 *		b. Write a value of 0x2 for Periodic mode.
 * 4. Load the start value into the GPTM TimerA Interval Load Register
 *    (GPTMT AILR).
 * 5. If interrupts are required, set the TATOIM bit in the GPTM Interrupt
 *    Mask Register (GPTMIMR).
 * 6. Set the TAEN bit in the GPTMCTL register to enable the timer and
 *    start counting.
 * 7. Poll the TATORIS bit in the GPTMRIS register or wait for the interrupt
 *   to be generated (if enabled). In both cases, the status flags are cleared
 *   by writing a 1 to the TATOCINT bit of the GPTM Interrupt
 *   Clear Register (GPTMICR).
 */
 
/** 
 * SysCtlPeripheralEnable() - Enables a peripheral.
 * @ulPeripheral:		the peripheral will be enabled.
 *
 * Peripherals are enabled with this function.  At power-up, all peripherals
 * are disabled; they must be enabled in order to operate or respond to
 * register reads/writes.
 *
 * Return: none.
 */
void SysCtlPeripheralEnable(unsigned long ulPeripheral)
{
    //
    // Check the arguments.
    //
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    //
    // Enable this peripheral.
    //
    HWREG(g_pulRCGCRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) |=
        SYSCTL_PERIPH_MASK(ulPeripheral);
}

/**
 * TimerLoadSet() - Set the timer load value.
 * @ulBase:			the base address of the timer module.
 * @ulTimer:		specifies the timer(s) to adjust.
 * @ulValue:		the load value.
 * 
 * This function sets the timer load value; if the timer is running then the
 * value will be immediately loaded into the timer.
 *
 * Return:	none.
 */
void TimerLoadSet(unsigned long ulBase, unsigned long ulTimer,
             unsigned long ulValue)
{
    //
    // Check the arguments.
    //
    ASSERT(TimerBaseValid(ulBase));
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));

    //
    // Set the timer A load value if requested.
    //
    if(ulTimer & TIMER_A)
    {
        HWREG(ulBase + TIMER_O_TAILR) = ulValue;
    }

    //
    // Set the timer B load value if requested.
    //
    if(ulTimer & TIMER_B)
    {
        HWREG(ulBase + TIMER_O_TBILR) = ulValue;
    }
}

/**
 * TimerConfigure() - Configures the timer(s).
 * @ulBase:			the base address of the timer module.
 * @ulConfig:		the configuration for the timer.
 *
 * This function configures the operating mode of the timer(s).  The timer
 * module is disabled before being configured, and is left in the disabled
 * state.  The configuration is specified values.
 *
 * Return:	none.
 */
void TimerConfigure(unsigned long ulBase, unsigned long ulConfig)
{
    //
    // Check the arguments.
    //
    ASSERT(TimerBaseValid(ulBase));
    ASSERT((ulConfig == TIMER_CFG_32_BIT_OS) ||
           (ulConfig == TIMER_CFG_32_BIT_OS_UP) ||
           (ulConfig == TIMER_CFG_32_BIT_PER) ||
           (ulConfig == TIMER_CFG_32_BIT_PER_UP) ||
           (ulConfig == TIMER_CFG_32_RTC) ||
           ((ulConfig & 0xff000000) == TIMER_CFG_16_BIT_PAIR));
    ASSERT(((ulConfig & 0xff000000) != TIMER_CFG_16_BIT_PAIR) ||
           ((((ulConfig & 0x000000ff) == TIMER_CFG_A_ONE_SHOT) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_ONE_SHOT_UP) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_PERIODIC) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_PERIODIC_UP) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_CAP_COUNT) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_CAP_TIME) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_PWM)) &&
            (((ulConfig & 0x0000ff00) == TIMER_CFG_B_ONE_SHOT) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_ONE_SHOT_UP) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_PERIODIC) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_PERIODIC_UP) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_CAP_COUNT) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_CAP_TIME) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_PWM))));

    //
    // Disable the timers.
    //
    HWREG(ulBase + TIMER_O_CTL) &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN);

    //
    // Set the global timer configuration.
    //
    HWREG(ulBase + TIMER_O_CFG) = ulConfig >> 24;

    //
    // Set the configuration of the A and B timers.  Note that the B timer
    // configuration is ignored by the hardware in 32-bit modes.
    //
    HWREG(ulBase + TIMER_O_TAMR) = ulConfig & 255;
    HWREG(ulBase + TIMER_O_TBMR) = (ulConfig >> 8) & 255;
}

/**
 * IntEnable() - Enables an interrupt.
 * @ulInterrupt:	specifies the interrupt to be enabled.
 *
 * The specified interrupt is enabled in the interrupt controller.  Other
 * enables for the interrupt (such as at the peripheral level) are unaffected
 * by this function.
 *
 * Return:	none.
 */
IntEnable(unsigned long ulInterrupt)
{
    //
    // Check the arguments.
    //
    ASSERT(ulInterrupt < NUM_INTERRUPTS);

    //
    // Determine the interrupt to enable.
    //
    if(ulInterrupt == FAULT_MPU)
    {
        //
        // Enable the MemManage interrupt.
        //
        HWREG(NVIC_SYS_HND_CTRL) |= NVIC_SYS_HND_CTRL_MEM;
    }
    else if(ulInterrupt == FAULT_BUS)
    {
        //
        // Enable the bus fault interrupt.
        //
        HWREG(NVIC_SYS_HND_CTRL) |= NVIC_SYS_HND_CTRL_BUS;
    }
    else if(ulInterrupt == FAULT_USAGE)
    {
        //
        // Enable the usage fault interrupt.
        //
        HWREG(NVIC_SYS_HND_CTRL) |= NVIC_SYS_HND_CTRL_USAGE;
    }
    else if(ulInterrupt == FAULT_SYSTICK)
    {
        //
        // Enable the System Tick interrupt.
        //
        HWREG(NVIC_ST_CTRL) |= NVIC_ST_CTRL_INTEN;
    }
    else if((ulInterrupt >= 16) && (ulInterrupt <= 47))
    {
        //
        // Enable the general interrupt.
        //
        HWREG(NVIC_EN0) = 1 << (ulInterrupt - 16);
    }
    else if(ulInterrupt >= 48)
    {
        //
        // Enable the general interrupt.
        //
        HWREG(NVIC_EN1) = 1 << (ulInterrupt - 48);
    }
}

/**
 * TimerIntEnable() - Enables individual timer interrupt sources.
 * @ulBase:			the base address of the timer module.
 * @ulIntFlags:		the bit mask of the interrupt sources to be enabled.
 *
 * Enables the indicated timer interrupt sources.  Only the sources that are
 * enabled can be reflected to the processor interrupt; disabled sources have
 * no effect on the processor.
 *
 * Return:	none.
 */
void TimerIntEnable(unsigned long ulBase, unsigned long ulIntFlags)
{
    //
    // Check the arguments.
    //
    ASSERT(TimerBaseValid(ulBase));

    //
    // Enable the specified interrupts.
    //
    HWREG(ulBase + TIMER_O_IMR) |= ulIntFlags;
}

TimerEnable(unsigned long ulBase, unsigned long ulTimer)
{
    //
    // Check the arguments.
    //
    ASSERT(TimerBaseValid(ulBase));
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));

    //
    // Enable the timer(s) module.
    //
    HWREG(ulBase + TIMER_O_CTL) |= ulTimer & (TIMER_CTL_TAEN | TIMER_CTL_TBEN);
}

/* use of the timers to generate periodic interrupts. */
int main(void)
{
    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_6MHZ);

    //
    // Initialize the OLED display and write status.
    //
    Display96x16x1Init(false);
    Display96x16x1StringDraw("Timers example", 6, 0);
    Display96x16x1StringDraw("T1: 0  T2: 0", 12, 1);

    //
    // Enable the peripherals used by this example.
	
    /** the peripheral clock must be enabled  */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //
    // Configure the two 32-bit periodic timers.
    //
	/** steps 1 - 3 */
    TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_32_BIT_PER);
	/** step 4 */
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() / 2);

    //
    // Setup the interrupts for the timer timeouts.
    //
	/** step 5 */
    IntEnable(INT_TIMER0A);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the timers.
    //
	/** step 6 */
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerEnable(TIMER1_BASE, TIMER_A);

    //
    // Loop forever while the timers run.
    // step 7
    while(1)
    {
    }
} 


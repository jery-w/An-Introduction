/**
 * Pulse Width Modulator (PWM)
 * Pulse width modulation (PWM) is a powerful technique for digitally encoding
 * analog signal levels. High-resolution counters are used to generate 
 * a square wave, and the duty cycle of the square wave is modulated
 * to encode an analog signal. Typical applications include switching
 * power supplies and motor control. 
 */

/** Functional Description */
/** 
 * PWM Timer
 * The timer in each PWM generator runs in one of two modes: Count-Down mode
 * or Count-Up/Down mode. In Count-Down mode, the timer counts from the
 * load value to zero, goes back to the load value, and continues counting down.
 * In Count-Up/Down mode, the timer counts from zero up to the load value, 
 * back down to zero, back up to the load value, and so on. Generally, 
 * Count-Down mode is used for generating left- or right-aligned PWM signals,
 * while the Count-Up/Down mode is used for generating 
 * center-aligned PWM signals.
 * The timers output three signals that are used in the PWM generation process:
 * the direction signal (this is always Low in Count-Down mode, but alternates
 * between Low and High in Count-Up/Down mode), a single-clock-cycle-width
 * High pulse when the counter is zero, and a single-clock-cycle-width 
 * High pulse when the counter is equal to the load value. Note that in
 * Count-Down mode, the zero pulse is immediately followed by the load pulse.
 */
/**
 * PWM Comparators
 * There are two comparators in each PWM generator that monitor the value of
 * the counter; when either match the counter , they output a 
 * single-clock-cycle-width High pulse. When in Count-Up/Down mode, these 
 * comparators match both when counting up and when counting down; they are
 * therefore qualified by the counter direction signal. These qualified pulses
 * are used in the PWM generation process. If either comparator match value is
 * greater than the counter load value, then that comparator never outputs
 * a High pulse.
 */

/**
 * PWM Signal Generator
 * The PWM generator takes these pulses (qualified by the direction signal), 
 * and generates two PWM signals. In Count-Down mode, there are four events 
 * that can affect the PWM signal: zero, load, match A down, and match B down. 
 * In Count-Up/Down mode, there are six events that can affect the PWM signal:
 * zero, load, match A down, match A up, match B down, and match B up. The match
 * A or match B events are ignored when they coincide with the zero or load 
 * events. If the match A and match B events coincide, the first signal, PWMA, 
 * is generated based only on the match A event, and the second signal, PWMB, is
 * generated based only on the match B event.
 *
 * For each event, the effect on each output PWM signal is programmable: it can
 * be left alone (ignoring the event), it can be toggled, it can be driven Low, 
 * or it can be driven High. These actions can be used to generate a pair of PWM
 * signals of various positions and duty cycles, which do or do not overlap. 
 * Figure 16-5 on page 400 shows the use of Count-Up/Down mode to generate a 
 * pair of center-aligned, overlapped PWM signals that have different duty 
 * cycles.
 */

/**
 * Dead-Band Generator
 * The two PWM signals produced by the PWM generator are passed to the dead-band
 * generator. If disabled, the PWM signals simply pass through unmodified. If 
 * enabled, the second PWM signal is lost and two PWM signals are generated
 * based on the first PWM signal. The first output PWM signal is the input
 * signal with the rising edge delayed by a programmable amount. The second
 * output PWM signal is the inversion of the input signal with a programmable
 * delay added between the falling edge of the input signal and the rising edge
 * of this new signal.
 * This is therefore a pair of active High signals where one is always High,
 * except for a programmable amount of time at transitions where both are Low.
 * These signals are therefore suitable for driving a half-H bridge, with the
 * dead-band delays preventing shoot-through current from damaging the
 * power electronics. Figure 16-6 on page 400 shows the effect of the dead-band
 * generator on an input PWM signal.
 */
 
/**
 * Interrupt/ADC-T rigger Selector
 * The PWM generator also takes the same four (or six) counter events and uses
 * them to generate an interrupt or an ADC trigger . Any of these events or
 * a set of these events can be selected as a source for an interrupt; when
 * any of the selected events occur, an interrupt is generated. Additionally,
 * the same event, a different event, the same set of events, or a different
 * set of events can be selected as a source for an ADC trigger; when any of
 * these selected events occur, an ADC trigger pulse is generated.
 * The selection of events allows the interrupt or ADC trigger to occur at
 * a specific position within the PWM signal. Note that interrupts and
 * ADC triggers are based on the raw events; delays in the PWM signal edges
 * caused by the dead-band generator are not taken into account.
 */

/**
 * Initialization and Configuration
 * The following example shows how to initialize the PWM Generator 0 with
 * a 25-KHz frequency , and with a 25% duty cycle on the PWM0 pin and a 75%
 * duty cycle on the PWM1 pin. This example assumes the system clock is 20 MHz.
 *
 * 1. Enable the PWM clock by writing a value of 0x0010.0000 to the RCGC0
 * register in the System Control module.
 * 2. In the GPIO module, enable the appropriate pins for their alternate
 * function using the GPIOAFSEL register .
 * 3. Configure the Run-Mode Clock Configuration (RCC) register in the System
 * Control module to use the PWM divide (USEPWMDIV) and set the divider
 * (PWMDIV) to divide by 2 (000).
 * 4. Configure the PWM generator for countdown mode with immediate updates
 * to the parameters.
 * 5. Set the period. For a 25-KHz frequency , the period = 1/25,000, or
 * 40 microseconds. The PWM clock source is 10 MHz; the system clock divided
 * by 2. This translates to 400 clock ticks per period. Use this value to
 * set the PWM0LOAD register . In Count-Down mode, set the Load field 
 * in the PWM0LOAD register to the requested period minus one.
 *   ¡ö Write the PWM0LOAD register with a value of 0x0000.018F .
 * 6. Set the pulse width of the PWM0 pin for a 25% duty cycle.
 *   ¡ö Write the PWM0CMP A register with a value of 0x0000.012B.
 * 7. Set the pulse width of the PWM1 pin for a 75% duty cycle.
 *   ¡ö Write the PWM0CMPB register with a value of 0x0000.0063.
 * 8. Start the timers in PWM generator 0.
 *   ¡ö Write the PWM0CTL register with a value of 0x0000.0001.
 * 9. Enable PWM outputs.
 *   ¡ö Write the PWMENABLE register with a value of 0x0000.0003.
 */
 
/**
 * This example application utilizes the PWM peripheral to output a 25% duty
 * cycle PWM signal and a 75% duty cycle PWM signal, both at 50 kHz.  Once
 * configured, the application enters an infinite loop, doing nothing while
 * the PWM peripheral continues to output its signals.
 */
int main(void)
{
    unsigned long ulPeriod;

    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_6MHZ);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    //
    // Initialize the OLED display.
    //
    Display96x16x1Init(false);

    //
    // Clear the screen and thell the user what is happening.
    //
    Display96x16x1StringDraw("Generating PWM", 6, 0);
    Display96x16x1StringDraw("on PD0 and PD1", 6, 1);

    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    //
    // Set GPIO D0 and D1 as PWM pins.  They are used to output the PWM0 and
    // PWM1 signals.
    //
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Compute the PWM period based on the system clock.
    //
    ulPeriod = SysCtlClockGet() / 50000;

    //
    // Set the PWM period to 50 kHz.
    //
    PWMGenConfigure(PWM_BASE, PWM_GEN_0,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, ulPeriod);

    //
    // Set PWM0 to a duty cycle of 25% and PWM1 to a duty cycle of 75%.
    //
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, ulPeriod / 4);
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, ulPeriod * 3 / 4);

    //
    // Enable the PWM0 and PWM1 output signals.
    //
    PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);

    //
    // Enable the PWM generator.
    //
    PWMGenEnable(PWM_BASE, PWM_GEN_0);

    //
    // Loop forever while the PWM signals are generated.
    //
    while(1)
    {
    }
}

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
 *
/**
 * 对于MCU的peripheral进行操作，尤其是使能或配置外设时，通常会对相应的寄存器进行
 * 操作，而且经常会是“位”操作，即bit-band操作。
 * 举例如下，以后有相关的程序也可参考：
 */
 
/**
 * 例1, 对于寄存器内容的访问是使用指针来完成的，MCU芯片中有相应外设的基地址base
 * 和偏移地址，通过对该值强制转换为指针型（reference），再取指针的值
 *（dereference）来使用。
 *
 * 外设peripheral的使能是对寄存器相应位进行操作或配置的结果，位操作常有 &，|，~
 * >>, <<, ^, 等等。而寄存器配置常有
 * 1，置位操作：使相应的位与mask掩码（对应位是1，其它位是0）进行“或”操作。
 * 		ulX = SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_M;
 * 2, 复位操作：使相应的位与mask掩码进行“ & ~ ”操作。
 * 		ulX = (HWREG(SYSCTL_RCC) &
 *                        ~(SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_M);
 *						  
 */

/** Macros for hardware access, both direct and via the bit-band region. */ 
#define HWREG(x)	(*((volatile unsigned long *)(x)))

/** enable a peripheral */
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

/** Sets the PWM clock configuration. */
void SysCtlPWMClockSet(unsigned long ulConfig)
{
    //
    // Check the arguments.
    //
    ASSERT((ulConfig == SYSCTL_PWMDIV_1) ||
           (ulConfig == SYSCTL_PWMDIV_2) ||
           (ulConfig == SYSCTL_PWMDIV_4) ||
           (ulConfig == SYSCTL_PWMDIV_8) ||
           (ulConfig == SYSCTL_PWMDIV_16) ||
           (ulConfig == SYSCTL_PWMDIV_32) ||
           (ulConfig == SYSCTL_PWMDIV_64));

    //
    // Check that there is a PWM block on this part.
    //
    ASSERT(HWREG(SYSCTL_DC1) & SYSCTL_DC1_PWM);

    //
    // Set the PWM clock configuration into the run-mode clock configuration
    // register.
    //
    HWREG(SYSCTL_RCC) = ((HWREG(SYSCTL_RCC) &
                          ~(SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_M)) |
                         ulConfig);
}
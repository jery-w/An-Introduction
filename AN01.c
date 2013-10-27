/**
 * ����MCU��peripheral���в�����������ʹ�ܻ���������ʱ��ͨ�������Ӧ�ļĴ�������
 * ���������Ҿ������ǡ�λ����������bit-band������
 * �������£��Ժ�����صĳ���Ҳ�ɲο���
 */
 
/**
 * ��1, ���ڼĴ������ݵķ�����ʹ��ָ������ɵģ�MCUоƬ������Ӧ����Ļ���ַbase
 * ��ƫ�Ƶ�ַ��ͨ���Ը�ֵǿ��ת��Ϊָ���ͣ�reference������ȡָ���ֵ
 *��dereference����ʹ�á�
 *
 * ����peripheral��ʹ���ǶԼĴ�����Ӧλ���в��������õĽ����λ�������� &��|��~
 * >>, <<, ^, �ȵȡ����Ĵ������ó���
 * 1����λ������ʹ��Ӧ��λ��mask���루��Ӧλ��1������λ��0�����С��򡱲�����
 * 		ulX = SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_M;
 * 2, ��λ������ʹ��Ӧ��λ��mask������С� & ~ ��������
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
/**
 * �ж� - ÿ��MCU�����д����жϵĹ��ܣ�������ôʵ����Ҫ����оƬ�������򵥵�˵��
 * �ж������������û�����֮���һЩ����/�쳣������һЩоƬϵͳ���쳣��������
 * �жϡ�
 * �жϷ������Interrupt Service Routine (ISR)�����ж�������interrupt vector
 * table, �жϴ����interrupt handler���ж��ȼ� priority of interrupt.
 * 
 * ��LM3S811Ϊ����
 * The interrupt controller API provides a set of functions for dealing with the
 * Nested Vectored Inter-rupt Controller (NVIC). Functions are provided to 
 * enable and disable interrupts, register interrupt handlers, and set the 
 * priority of interrupts.
 * The NVIC provides global interrupt masking, prioritization, and handler
 * dispatching. This version of the Stellaris family supports thirty-two
 * interrupt sources and eight priority levels. Individual inter-rupt sources
 * can be masked, and the processor interrupt can be globally masked as well
 * (without affecting the individual source masks).
 * The NVIC is tightly coupled with the Cortex-M3 microprocessor. When the 
 * processor responds to an interrupt, NVIC will supply the address of the
 * function to handle the interrupt directly to the processor. This eliminates 
 * the need for a global interrupt handler that queries the interrupt controller
 *to determine the cause of the interrupt and branch to the appropriate handler,
 * reducing interrupt response time.
 * If two interrupts with the same priority (and subpriority if so configured)
 * are asserted at the same time, the one with the lower interrupt number will
 * be processed first. NVIC keeps track of the nesting of interrupt handlers,
 * allowing the processor to return from interrupt context only once all nested
 * and pending interrupts have been handled.
 *
 * Interrupt handlers can be configured in one of two ways; statically at 
 * compile time or dynamically at run time. Static configuration of interrupt
 * handlers is accomplished by editing the interrupt handler table in the
 * application��s startup code. When statically configured, the interrupts must
 * be explicitly enabled in NVIC via IntEnable() before the processor will 
 * respond to the interrupt (in addition to any interrupt enabling required 
 * within the peripheral itself).
 * Alternatively, interrupts can be configured at run-time using IntRegister()
 * (or the analog in each individual driver). When using IntRegister(), the
 * interrupt must also be enabled as before; when using the analogue in each
 * individual driver, IntEnable() is called by the driver and does not need
 * to be call by the application.
 */
 
/** 
 * The following example shows how to use the Interrupt Controller API to 
 * register an interrupt handler and enable the interrupt.
 */
//
// The interrupt handler function.
extern void IntHandler(void);
//
// Register the interrupt handler function for interrupt 5.
IntRegister(5, IntHandler);
//
// Enable interrupt 5.
IntEnable(5);
//
// Enable interrupt 5.
IntMasterEnable();
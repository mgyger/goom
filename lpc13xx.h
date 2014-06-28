#ifndef __LPC13xx_H__
#define __LPC13xx_H__

typedef unsigned int ui32;

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/

#define  Reset_IRQn                    -15      /*!< 1 Reset Vector, invoked on PowerUp and warm reset*/
#define  NonMaskableInt_IRQn           -14      /*!< 2 Non Maskable Interrupt                         */
#define  HardFault_IRQn                -13      /*!< 3  Hard Fault, all classes of Fault              */
#define  MemoryManagement_IRQn         -12      /*!< 4 Cortex-M3 Memory Management Interrupt          */
#define  BusFault_IRQn                 -11      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
#define  UsageFault_IRQn               -10      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
#define  SVCall_IRQn                   -5       /*!< 11 Cortex-M3 SV Call Interrupt                   */
#define  DebugMonitor_IRQn             -4       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
#define  PendSV_IRQn                   -2       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
#define  SysTick_IRQn                  -1       /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC13xx Specific Interrupt Numbers *******************************************************/
#define  WAKEUP0_IRQn                  0        /*!< All I/O pins can be used as wakeup source.       */
#define  WAKEUP1_IRQn                  1        /*!< There are 40 pins in total for LPC17xx           */
#define  WAKEUP2_IRQn                  2
#define  WAKEUP3_IRQn                  3
#define  WAKEUP4_IRQn                  4
#define  WAKEUP5_IRQn                  5
#define  WAKEUP6_IRQn                  6
#define  WAKEUP7_IRQn                  7
#define  WAKEUP8_IRQn                  8
#define  WAKEUP9_IRQn                  9
#define  WAKEUP10_IRQn                 10
#define  WAKEUP11_IRQn                 11
#define  WAKEUP12_IRQn                 12
#define  WAKEUP13_IRQn                 13
#define  WAKEUP14_IRQn                 14
#define  WAKEUP15_IRQn                 15
#define  WAKEUP16_IRQn                 16
#define  WAKEUP17_IRQn                 17
#define  WAKEUP18_IRQn                 18
#define  WAKEUP19_IRQn                 19
#define  WAKEUP20_IRQn                 20
#define  WAKEUP21_IRQn                 21
#define  WAKEUP22_IRQn                 22
#define  WAKEUP23_IRQn                 23
#define  WAKEUP24_IRQn                 24
#define  WAKEUP25_IRQn                 25
#define  WAKEUP26_IRQn                 26
#define  WAKEUP27_IRQn                 27
#define  WAKEUP28_IRQn                 28
#define  WAKEUP29_IRQn                 29
#define  WAKEUP30_IRQn                 30
#define  WAKEUP31_IRQn                 31
#define  WAKEUP32_IRQn                 32
#define  WAKEUP33_IRQn                 33
#define  WAKEUP34_IRQn                 34
#define  WAKEUP35_IRQn                 35
#define  WAKEUP36_IRQn                 36
#define  WAKEUP37_IRQn                 37
#define  WAKEUP38_IRQn                 38
#define  WAKEUP39_IRQn                 39
#define  I2C_IRQn                      40       /*!< I2C Interrupt                                    */
#define  TIMER_16_0_IRQn               41       /*!< 16-bit Timer0 Interrupt                          */
#define  TIMER_16_1_IRQn               42       /*!< 16-bit Timer1 Interrupt                          */
#define  TIMER_32_0_IRQn               43       /*!< 32-bit Timer0 Interrupt                          */
#define  TIMER_32_1_IRQn               44       /*!< 32-bit Timer1 Interrupt                          */
#define  SSP0_IRQn                     45       /*!< SSP Interrupt                                    */
#define  UART_IRQn                     46       /*!< UART Interrupt                                   */
#define  USB_IRQn                      47       /*!< USB Regular Interrupt                            */
#define  USB_FIQn                      48       /*!< USB Fast Interrupt                               */
#define  ADC_IRQn                      49       /*!< A/D Converter Interrupt                          */
#define  WDT_IRQn                      50       /*!< Watchdog timer Interrupt                         */
#define  BOD_IRQn                      51       /*!< Brown Out Detect(BOD) Interrupt                  */
#define  EINT3_IRQn                    53       /*!< External Interrupt 3 Interrupt                   */
#define  EINT2_IRQn                    54       /*!< External Interrupt 2 Interrupt                   */
#define  EINT1_IRQn                    55       /*!< External Interrupt 1 Interrupt                   */
#define  EINT0_IRQn                    56       /*!< External Interrupt 0 Interrupt                   */
#define  SSP1_IRQn                     57       /*!< SSP1 Interrupt                                   */


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */

/*------------- System Control (SYSCON) --------------------------------------*/

#define SYSCON_SYSMEMREMAP   (*(volatile ui32*)0x40048000)
#define SYSCON_PRESETCTRL    (*(volatile ui32*)0x40048004)
#define SYSCON_SYSPLLCTRL    (*(volatile ui32*)0x40048008)
#define SYSCON_SYSPLLSTAT    (*(volatile ui32*)0x4004800C)
#define SYSCON_USBPLLCTRL    (*(volatile ui32*)0x40048010)
#define SYSCON_USBPLLSTAT    (*(volatile ui32*)0x40048014)
#define SYSCON_SYSOSCCTRL    (*(volatile ui32*)0x40048020)
#define SYSCON_WDTOSCCTRL    (*(volatile ui32*)0x40048024)
#define SYSCON_IRCCTRL       (*(volatile ui32*)0x40048028)
#define SYSCON_SYSRSTSTAT    (*(volatile ui32*)0x40048030)
#define SYSCON_SYSPLLCLKSEL  (*(volatile ui32*)0x40048040)
#define SYSCON_SYSPLLCLKUEN  (*(volatile ui32*)0x40048044)
#define SYSCON_USBPLLCLKSEL  (*(volatile ui32*)0x40048048)
#define SYSCON_USBPLLCLKUEN  (*(volatile ui32*)0x4004804c)
#define SYSCON_MAINCLKSEL    (*(volatile ui32*)0x40048070)
#define SYSCON_MAINCLKUEN    (*(volatile ui32*)0x40048074)
#define SYSCON_SYSAHBCLKDIV  (*(volatile ui32*)0x40048078)
#define SYSCON_SYSAHBCLKCTRL (*(volatile ui32*)0x40048080)
#define SYSCON_SSP0CLKDIV    (*(volatile ui32*)0x40048094)
#define SYSCON_UARTCLKDIV    (*(volatile ui32*)0x40048098)
#define SYSCON_SSP1CLKDIV    (*(volatile ui32*)0x4004809C)
#define SYSCON_TRACECLKDIV   (*(volatile ui32*)0x400480ac)
#define SYSCON_SYSTICKCLKDIV (*(volatile ui32*)0x400480B0)
#define SYSCON_USBCLKSEL     (*(volatile ui32*)0x400480C0)
#define SYSCON_USBCLKUEN     (*(volatile ui32*)0x400480C4)
#define SYSCON_USBCLKDIV     (*(volatile ui32*)0x400480C8)
#define SYSCON_WDTCLKSEL     (*(volatile ui32*)0x400480D0)
#define SYSCON_WDTCLKUEN     (*(volatile ui32*)0x400480D4)
#define SYSCON_WDTCLKDIV     (*(volatile ui32*)0x400480D8)
#define SYSCON_CLKOUTCLKSEL  (*(volatile ui32*)0x400480E0)
#define SYSCON_CLKOUTUEN     (*(volatile ui32*)0x400480E4)
#define SYSCON_CLKOUTDIV     (*(volatile ui32*)0x400480E8)
#define SYSCON_PIOPORCAP0    (*(volatile ui32*)0x40048100)
#define SYSCON_PIOPORCAP1    (*(volatile ui32*)0x40048104)
#define SYSCON_BODCTRL       (*(volatile ui32*)0x40048150)
#define SYSCON_SYSTCKCAL     (*(volatile ui32*)0x40048158)
#define SYSCON_STARTAPRP0    (*(volatile ui32*)0x40048200)
#define SYSCON_STARTERP0     (*(volatile ui32*)0x40048204)
#define SYSCON_STARTRSRP0CLR (*(volatile ui32*)0x40048208)
#define SYSCON_STARTSRP0     (*(volatile ui32*)0x4004820C)
#define SYSCON_STARTAPRP1    (*(volatile ui32*)0x40048210)
#define SYSCON_STARTERP1     (*(volatile ui32*)0x40048214)
#define SYSCON_STARTRSRP1CLR (*(volatile ui32*)0x40048218)
#define SYSCON_STARTSRP1     (*(volatile ui32*)0x4004821C)
#define SYSCON_PDSLEEPCFG    (*(volatile ui32*)0x40048230)
#define SYSCON_PDAWAKECFG    (*(volatile ui32*)0x40048234)
#define SYSCON_PDRUNCFG      (*(volatile ui32*)0x40048238)
#define SYSCON_DEVICE_ID     (*(volatile ui32*)0x400483F4)


/*------------- Pin Connect Block (IOCON) --------------------------------*/

#define IOCON_PIO2_6        (*(volatile ui32*)0x40044000)
#define IOCON_PIO2_0        (*(volatile ui32*)0x40044008)
#define IOCON_PIO0_0        (*(volatile ui32*)0x4004400C)
#define IOCON_PIO0_1        (*(volatile ui32*)0x40044010)
#define IOCON_PIO1_8        (*(volatile ui32*)0x40044014)
#define IOCON_PIO0_2        (*(volatile ui32*)0x4004401C)
#define IOCON_PIO2_7        (*(volatile ui32*)0x40044020)
#define IOCON_PIO2_8        (*(volatile ui32*)0x40044024)
#define IOCON_PIO2_1        (*(volatile ui32*)0x40044028)
#define IOCON_PIO0_3        (*(volatile ui32*)0x4004402C)
#define IOCON_PIO0_4        (*(volatile ui32*)0x40044030)
#define IOCON_PIO0_5        (*(volatile ui32*)0x40044034)
#define IOCON_PIO1_9        (*(volatile ui32*)0x40044038)
#define IOCON_PIO3_4        (*(volatile ui32*)0x4004403C)
#define IOCON_PIO2_4        (*(volatile ui32*)0x40044040)
#define IOCON_PIO2_5        (*(volatile ui32*)0x40044044)
#define IOCON_PIO3_5        (*(volatile ui32*)0x40044048)
#define IOCON_PIO0_6        (*(volatile ui32*)0x4004404C)
#define IOCON_PIO0_7        (*(volatile ui32*)0x40044050)
#define IOCON_PIO2_9        (*(volatile ui32*)0x40044054)
#define IOCON_PIO2_10       (*(volatile ui32*)0x40044058)
#define IOCON_PIO2_2        (*(volatile ui32*)0x4004405C)
#define IOCON_PIO0_8        (*(volatile ui32*)0x40044060)
#define IOCON_PIO0_9        (*(volatile ui32*)0x40044064)
#define IOCON_PIO0_10       (*(volatile ui32*)0x40044068)
#define IOCON_PIO1_10       (*(volatile ui32*)0x4004406C)
#define IOCON_PIO2_11       (*(volatile ui32*)0x40044070)
#define IOCON_PIO0_11       (*(volatile ui32*)0x40044074)
#define IOCON_PIO1_0        (*(volatile ui32*)0x40044078)
#define IOCON_PIO1_1        (*(volatile ui32*)0x4004407C)
#define IOCON_PIO1_2        (*(volatile ui32*)0x40044080)
#define IOCON_PIO3_0        (*(volatile ui32*)0x40044084)
#define IOCON_PIO3_1        (*(volatile ui32*)0x40044088)
#define IOCON_PIO2_3        (*(volatile ui32*)0x4004408C)
#define IOCON_PIO1_3        (*(volatile ui32*)0x40044090)
#define IOCON_PIO1_4        (*(volatile ui32*)0x40044094)
#define IOCON_PIO1_11       (*(volatile ui32*)0x40044098)
#define IOCON_PIO3_2        (*(volatile ui32*)0x4004409C)
#define IOCON_PIO1_5        (*(volatile ui32*)0x400440A0)
#define IOCON_PIO1_6        (*(volatile ui32*)0x400440A4)
#define IOCON_PIO1_7        (*(volatile ui32*)0x400440A8)
#define IOCON_PIO3_3        (*(volatile ui32*)0x400440AC)
#define IOCON_SCK_LOC       (*(volatile ui32*)0x400440B0)
#define IOCON_DSR_LOC       (*(volatile ui32*)0x400440B4)
#define IOCON_DCD_LOC       (*(volatile ui32*)0x400440B8)
#define IOCON_RI_LOC        (*(volatile ui32*)0x400440BC)


/*------------- Power Management Unit (PMU) --------------------------*/

#define PMU_PCON   (*(volatile ui32*)0x40038000)
#define PMU_GPREG0 (*(volatile ui32*)0x40038004)
#define PMU_GPREG1 (*(volatile ui32*)0x40038008)
#define PMU_GPREG2 (*(volatile ui32*)0x4003800C)
#define PMU_GPREG3 (*(volatile ui32*)0x40038010)
#define PMU_GPREG4 (*(volatile ui32*)0x40038014)

/*------------- General Purpose Input/Output (GPIO) --------------------------*/

#define GPIO0_MASKED_ACCESS (*(volatile ui32*)0x50000000)
#define GPIO0_DATA          (*(volatile ui32*)0x50003FFC)
#define GPIO0_DIR           (*(volatile ui32*)0x50008000)
#define GPIO0_IS            (*(volatile ui32*)0x50008004)
#define GPIO0_IBE           (*(volatile ui32*)0x50008008)
#define GPIO0_IEV           (*(volatile ui32*)0x5000800C)
#define GPIO0_IE            (*(volatile ui32*)0x50008010)
#define GPIO0_RIS           (*(volatile ui32*)0x50008014)
#define GPIO0_MIS           (*(volatile ui32*)0x50008018)
#define GPIO0_IC            (*(volatile ui32*)0x5000801C)

#define GPIO1_MASKED_ACCESS (*(volatile ui32*)0x50010000)
#define GPIO1_DATA          (*(volatile ui32*)0x50013FFC)
#define GPIO1_DIR           (*(volatile ui32*)0x50018000)
#define GPIO1_IS            (*(volatile ui32*)0x50018004)
#define GPIO1_IBE           (*(volatile ui32*)0x50018008)
#define GPIO1_IEV           (*(volatile ui32*)0x5001800C)
#define GPIO1_IE            (*(volatile ui32*)0x50018010)
#define GPIO1_RIS           (*(volatile ui32*)0x50018014)
#define GPIO1_MIS           (*(volatile ui32*)0x50018018)
#define GPIO1_IC            (*(volatile ui32*)0x5001801C)

#define GPIO2_MASKED_ACCESS (*(volatile ui32*)0x50020000)
#define GPIO2_DATA          (*(volatile ui32*)0x50023FFC)
#define GPIO2_DIR           (*(volatile ui32*)0x50028000)
#define GPIO2_IS            (*(volatile ui32*)0x50028004)
#define GPIO2_IBE           (*(volatile ui32*)0x50028008)
#define GPIO2_IEV           (*(volatile ui32*)0x5002800C)
#define GPIO2_IE            (*(volatile ui32*)0x50028010)
#define GPIO2_RIS           (*(volatile ui32*)0x50028014)
#define GPIO2_MIS           (*(volatile ui32*)0x50028018)
#define GPIO2_IC            (*(volatile ui32*)0x5002801C)

#define GPIO3_MASKED_ACCESS (*(volatile ui32*)0x50030000)
#define GPIO3_DATA          (*(volatile ui32*)0x50033FFC)
#define GPIO3_DIR           (*(volatile ui32*)0x50038000)
#define GPIO3_IS            (*(volatile ui32*)0x50038004)
#define GPIO3_IBE           (*(volatile ui32*)0x50038008)
#define GPIO3_IEV           (*(volatile ui32*)0x5003800C)
#define GPIO3_IE            (*(volatile ui32*)0x50038010)
#define GPIO3_RIS           (*(volatile ui32*)0x50038014)
#define GPIO3_MIS           (*(volatile ui32*)0x50038018)
#define GPIO3_IC            (*(volatile ui32*)0x5003801C)


/*------------- Timer (TMR) --------------------------------------------------*/

#define CT16B0_IR            (*(volatile ui32*)0x4000c000)
#define CT16B0_TCR           (*(volatile ui32*)0x4000c004)
#define CT16B0_TC            (*(volatile ui32*)0x4000c008)
#define CT16B0_PR            (*(volatile ui32*)0x4000c00C)
#define CT16B0_PC            (*(volatile ui32*)0x4000c010)
#define CT16B0_MCR           (*(volatile ui32*)0x4000c014)
#define CT16B0_MR0           (*(volatile ui32*)0x4000c018)
#define CT16B0_MR1           (*(volatile ui32*)0x4000c01C)
#define CT16B0_MR2           (*(volatile ui32*)0x4000c020)
#define CT16B0_MR3           (*(volatile ui32*)0x4000c024)
#define CT16B0_CCR           (*(volatile ui32*)0x4000c028)
#define CT16B0_CR0           (*(volatile ui32*)0x4000c02C)
#define CT16B0_EMR           (*(volatile ui32*)0x4000c03C)
#define CT16B0_CTCR          (*(volatile ui32*)0x4000c070)
#define CT16B0_PWMC          (*(volatile ui32*)0x4000c074)

#define CT16B1_IR            (*(volatile ui32*)0x40010000)
#define CT16B1_TCR           (*(volatile ui32*)0x40010004)
#define CT16B1_TC            (*(volatile ui32*)0x40010008)
#define CT16B1_PR            (*(volatile ui32*)0x4001000C)
#define CT16B1_PC            (*(volatile ui32*)0x40010010)
#define CT16B1_MCR           (*(volatile ui32*)0x40010014)
#define CT16B1_MR0           (*(volatile ui32*)0x40010018)
#define CT16B1_MR1           (*(volatile ui32*)0x4001001C)
#define CT16B1_MR2           (*(volatile ui32*)0x40010020)
#define CT16B1_MR3           (*(volatile ui32*)0x40010024)
#define CT16B1_CCR           (*(volatile ui32*)0x40010028)
#define CT16B1_CR0           (*(volatile ui32*)0x4001002C)
#define CT16B1_EMR           (*(volatile ui32*)0x4001003C)
#define CT16B1_CTCR          (*(volatile ui32*)0x40010070)
#define CT16B1_PWMC          (*(volatile ui32*)0x40010074)

#define CT32B0_IR            (*(volatile ui32*)0x40014000)
#define CT32B0_TCR           (*(volatile ui32*)0x40014004)
#define CT32B0_TC            (*(volatile ui32*)0x40014008)
#define CT32B0_PR            (*(volatile ui32*)0x4001400C)
#define CT32B0_PC            (*(volatile ui32*)0x40014010)
#define CT32B0_MCR           (*(volatile ui32*)0x40014014)
#define CT32B0_MR0           (*(volatile ui32*)0x40014018)
#define CT32B0_MR1           (*(volatile ui32*)0x4001401C)
#define CT32B0_MR2           (*(volatile ui32*)0x40014020)
#define CT32B0_MR3           (*(volatile ui32*)0x40014024)
#define CT32B0_CCR           (*(volatile ui32*)0x40014028)
#define CT32B0_CR0           (*(volatile ui32*)0x4001402C)
#define CT32B0_EMR           (*(volatile ui32*)0x4001403C)
#define CT32B0_CTCR          (*(volatile ui32*)0x40014070)
#define CT32B0_PWMC          (*(volatile ui32*)0x40014074)

#define CT32B1_IR            (*(volatile ui32*)0x40018000)
#define CT32B1_TCR           (*(volatile ui32*)0x40018004)
#define CT32B1_TC            (*(volatile ui32*)0x40018008)
#define CT32B1_PR            (*(volatile ui32*)0x4001800C)
#define CT32B1_PC            (*(volatile ui32*)0x40018010)
#define CT32B1_MCR           (*(volatile ui32*)0x40018014)
#define CT32B1_MR0           (*(volatile ui32*)0x40018018)
#define CT32B1_MR1           (*(volatile ui32*)0x4001801C)
#define CT32B1_MR2           (*(volatile ui32*)0x40018020)
#define CT32B1_MR3           (*(volatile ui32*)0x40018024)
#define CT32B1_CCR           (*(volatile ui32*)0x40018028)
#define CT32B1_CR0           (*(volatile ui32*)0x4001802C)
#define CT32B1_EMR           (*(volatile ui32*)0x4001803C)
#define CT32B1_CTCR          (*(volatile ui32*)0x40018070)
#define CT32B1_PWMC          (*(volatile ui32*)0x40018074)

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/

#define  UART_RBR          (*(volatile ui32*)0x40008000)
#define  UART_THR          (*(volatile ui32*)0x40008000)
#define  UART_DLL          (*(volatile ui32*)0x40008000)
#define  UART_DLM          (*(volatile ui32*)0x40008004)
#define  UART_IER          (*(volatile ui32*)0x40008004)
#define  UART_IIR          (*(volatile ui32*)0x40008008)
#define  UART_FCR          (*(volatile ui32*)0x40008008)
#define  UART_LCR          (*(volatile ui32*)0x4000800C)
#define  UART_MCR          (*(volatile ui32*)0x40008010)
#define  UART_LSR          (*(volatile ui32*)0x40008014)
#define  UART_MSR          (*(volatile ui32*)0x40008018)
#define  UART_SCR          (*(volatile ui32*)0x4000801C)
#define  UART_ACR          (*(volatile ui32*)0x40008020)
#define  UART_FDR          (*(volatile ui32*)0x40008028)
#define  UART_TER          (*(volatile ui32*)0x40008030)
#define  UART_RS485CTRL    (*(volatile ui32*)0x4000804C)
#define  UART_ADRMATCH     (*(volatile ui32*)0x40008050)
#define  UART_RS485DLY     (*(volatile ui32*)0x40008054)

/*------------- Synchronous Serial Communication (SSP) -----------------------*/

#define SSP0_CR0           (*(volatile ui32*)0x40040000)
#define SSP0_CR1           (*(volatile ui32*)0x40040004)
#define SSP0_DR            (*(volatile ui32*)0x40040008)
#define SSP0_SR            (*(volatile ui32*)0x4004000C)
#define SSP0_CPSR          (*(volatile ui32*)0x40040010)
#define SSP0_IMSC          (*(volatile ui32*)0x40040014)
#define SSP0_RIS           (*(volatile ui32*)0x40040018)
#define SSP0_MIS           (*(volatile ui32*)0x4004001C)
#define SSP0_ICR           (*(volatile ui32*)0x40040020)

#define SSP1_CR0           (*(volatile ui32*)0x40058000)
#define SSP1_CR1           (*(volatile ui32*)0x40058004)
#define SSP1_DR            (*(volatile ui32*)0x40058008)
#define SSP1_SR            (*(volatile ui32*)0x4005800C)
#define SSP1_CPSR          (*(volatile ui32*)0x40058010)
#define SSP1_IMSC          (*(volatile ui32*)0x40058014)
#define SSP1_RIS           (*(volatile ui32*)0x40058018)
#define SSP1_MIS           (*(volatile ui32*)0x4005801C)
#define SSP1_ICR           (*(volatile ui32*)0x40058020)

/*------------- Inter-Integrated Circuit (I2C) -------------------------------*/

#define IIC_CONSET        (*(volatile ui32*)0x40000000)
#define IIC_STAT          (*(volatile ui32*)0x40000004)
#define IIC_DAT           (*(volatile ui32*)0x40000008)
#define IIC_ADR0          (*(volatile ui32*)0x4000000C)
#define IIC_SCLH          (*(volatile ui32*)0x40000010)
#define IIC_SCLL          (*(volatile ui32*)0x40000014)
#define IIC_CONCLR        (*(volatile ui32*)0x40000018)
#define IIC_MMCTRL        (*(volatile ui32*)0x4000001C)
#define IIC_ADR1          (*(volatile ui32*)0x40000020)
#define IIC_ADR2          (*(volatile ui32*)0x40000024)
#define IIC_ADR3          (*(volatile ui32*)0x40000028)
#define IIC_DATA_BUFFER   (*(volatile ui32*)0x4000002C)
#define IIC_MASK0         (*(volatile ui32*)0x40000030)
#define IIC_MASK1         (*(volatile ui32*)0x40000034)
#define IIC_MASK2         (*(volatile ui32*)0x40000038)
#define IIC_MASK3         (*(volatile ui32*)0x4000003C)

/*------------- Windowed Watchdog Timer (WWDT) -----------------------------------------*/

#define WWDT_MOD           (*(volatile ui32*)0x40004000)
#define WWDT_TC            (*(volatile ui32*)0x40004004)
#define WWDT_FEED          (*(volatile ui32*)0x40004008)
#define WWDT_TV            (*(volatile ui32*)0x4000400C)
#define WWDT_WARNINT       (*(volatile ui32*)0x40004014)
#define WWDT_WINDOW        (*(volatile ui32*)0x40004018)

/*------------- Analog-to-Digital Converter (ADC) ----------------------------*/

#define ADC_CR            (*(volatile ui32*)0x4001c000)
#define ADC_GDR           (*(volatile ui32*)0x4001c004)
#define ADC_INTEN         (*(volatile ui32*)0x4001c00C)
#define ADC_DR0           (*(volatile ui32*)0x4001c010)
#define ADC_DR1           (*(volatile ui32*)0x4001c014)
#define ADC_DR2           (*(volatile ui32*)0x4001c018)
#define ADC_DR3           (*(volatile ui32*)0x4001c01c)
#define ADC_DR4           (*(volatile ui32*)0x4001c020)
#define ADC_DR5           (*(volatile ui32*)0x4001c024)
#define ADC_DR6           (*(volatile ui32*)0x4001c028)
#define ADC_DR7           (*(volatile ui32*)0x4001c02c)
#define ADC_STAT          (*(volatile ui32*)0x4001c030)

/*------------- Universal Serial Bus (USB) -----------------------------------*/

#define USB_DevIntSt      (*(volatile ui32*)0x40020000)
#define USB_DevIntEn      (*(volatile ui32*)0x40020004)
#define USB_DevIntClr     (*(volatile ui32*)0x40020008)
#define USB_DevIntSet     (*(volatile ui32*)0x4002000C)
#define USB_CmdCode       (*(volatile ui32*)0x40020010)
#define USB_CmdData       (*(volatile ui32*)0x40020014)
#define USB_RxData        (*(volatile ui32*)0x40020018)
#define USB_TxData        (*(volatile ui32*)0x4002001C)
#define USB_RxPLen        (*(volatile ui32*)0x40020020)
#define USB_TxPLen        (*(volatile ui32*)0x40020024)
#define USB_Ctrl          (*(volatile ui32*)0x40020028)
#define USB_DevFIQSel     (*(volatile ui32*)0x4002002C)

// NVIC

#define NVIC_ISER0  (*(volatile ui32*)0xe000e100)
#define NVIC_ISER1  (*(volatile ui32*)0xe000e104)
#define NVIC_ICER0  (*(volatile ui32*)0xe000e180)
#define NVIC_ICER1  (*(volatile ui32*)0xe000e184)
#define NVIC_ISPR0  (*(volatile ui32*)0xe000e200)
#define NVIC_ISPR1  (*(volatile ui32*)0xe000e204)
#define NVIC_ICPR0  (*(volatile ui32*)0xe000e280)
#define NVIC_ICPR1  (*(volatile ui32*)0xe000e284)
#define NVIC_IABR0  (*(volatile ui32*)0xe000e300)
#define NVIC_IABR1  (*(volatile ui32*)0xe000e304)
#define NVIC_IPR0   (*(volatile ui32*)0xe000e400)
#define NVIC_IPR1   (*(volatile ui32*)0xe000e404)
#define NVIC_IPR2   (*(volatile ui32*)0xe000e408)
#define NVIC_IPR3   (*(volatile ui32*)0xe000e40C)
#define NVIC_IPR4   (*(volatile ui32*)0xe000e410)
#define NVIC_IPR5   (*(volatile ui32*)0xe000e414)
#define NVIC_IPR6   (*(volatile ui32*)0xe000e418)
#define NVIC_IPR7   (*(volatile ui32*)0xe000e41C)
#define NVIC_IPR8   (*(volatile ui32*)0xe000e420)
#define NVIC_IPR9   (*(volatile ui32*)0xe000e424)
#define NVIC_IPR10  (*(volatile ui32*)0xe000e428)
#define NVIC_IPR11  (*(volatile ui32*)0xe000e42c)
#define NVIC_IPR12  (*(volatile ui32*)0xe000e430)
#define NVIC_IPR13  (*(volatile ui32*)0xe000e434)
#define NVIC_IPR14  (*(volatile ui32*)0xe000e438)
#define NVIC_STIR   (*(volatile ui32*)0xe000ef00)

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
#define FLASH_BASE        (0x00000000UL)
#define RAM_BASE          (0x10000000UL)
#define APB0_BASE         (0x40000000UL)
#define AHB_BASE          (0x50000000UL)

/* APB0 peripherals                                                           */
#define I2C_BASE          (APB0_BASE + 0x00000)
#define WWDT_BASE         (APB0_BASE + 0x04000)
#define UART_BASE         (APB0_BASE + 0x08000)
#define CT16B0_BASE       (APB0_BASE + 0x0C000)
#define CT16B1_BASE       (APB0_BASE + 0x10000)
#define CT32B0_BASE       (APB0_BASE + 0x14000)
#define CT32B1_BASE       (APB0_BASE + 0x18000)
#define ADC_BASE          (APB0_BASE + 0x1C000)
#define USB_BASE          (APB0_BASE + 0x20000)
#define PMU_BASE          (APB0_BASE + 0x38000)
#define SSP0_BASE         (APB0_BASE + 0x40000)
#define IOCON_BASE        (APB0_BASE + 0x44000)
#define SYSCON_BASE       (APB0_BASE + 0x48000)
#define SSP1_BASE         (APB0_BASE + 0x58000)

/* AHB peripherals                                                        */	
#define GPIO_BASE         (AHB_BASE  + 0x00000)
#define GPIO0_BASE        (AHB_BASE  + 0x00000)
#define GPIO1_BASE        (AHB_BASE  + 0x10000)
#define GPIO2_BASE        (AHB_BASE  + 0x20000)
#define GPIO3_BASE        (AHB_BASE  + 0x30000)

#endif  /* __LPC13xx_H__ */

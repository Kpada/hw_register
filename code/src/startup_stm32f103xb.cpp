/* 2021 Nikolai Chizhov */

#include <cstdint>
#include <algorithm>

/* С++ startup file for STM32F103C8  (Mainstream line)
 * Based on the Cube-Generated startup.s
 */

/* the list of constants from linker */
extern uintptr_t _sidata;    /* start of init data */
extern uintptr_t _sdata;     /* start of data */
extern uintptr_t _edata;     /* end of data */
extern uintptr_t _sbss;      /* start of bss */
extern uintptr_t _ebss;      /* end of bss */
extern uintptr_t _estack;    /* top of stack */

/* Static constructor initializator from libc */
extern "C" void __libc_init_array();
/* Main program endtry point */
extern int main();

/* Reset Handler */
extern "C" void Reset_Handler(void) {
    /* init the data section */
    size_t size = static_cast<size_t>(&_edata - &_sdata);
    std::copy(&_edata, &_edata + size, &_sidata);

    /* init the bss section */
    std::fill(&_sbss, &_ebss, 0x00);

    /* init static constructors */
    __libc_init_array();

    /* enter the main */
    main();

    /* something went wrong, we should never leave main */
}

/* Empty */
void DummyIrqHandler() {
    while(1);
}

/* Another empty callback */
void HardfaultHandler() {
    while(1);
}


using irqFunc = void(*)();
using irqVectorItem = union {
    irqFunc     func;
    uintptr_t*  ptr;
};


static_assert(sizeof(irqVectorItem) == sizeof(uint32_t));

extern "C"
__attribute__((section(".isr_vector")))
const irqVectorItem vectorTable[] = {
    {.ptr = &_estack},
    Reset_Handler,
    DummyIrqHandler,    /* NMI_Handler */
    HardfaultHandler,   /* HardFault_Handler */
    DummyIrqHandler,    /* MemManage_Handler */
    DummyIrqHandler,    /* BusFault_Handler */
    DummyIrqHandler,    /* UsageFault_Handler */
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    DummyIrqHandler,    /* SVC_Handler */
    DummyIrqHandler,    /* DebugMon_Handler */
    nullptr,
    DummyIrqHandler,    /* PendSV_Handler */
    DummyIrqHandler,    /* SysTick_Handler */
    DummyIrqHandler,    /* WWDG_IRQHandler */
    DummyIrqHandler,    /* PVD_IRQHandler */
    DummyIrqHandler,    /* TAMPER_IRQHandler */
    DummyIrqHandler,    /* RTC_IRQHandler */
    DummyIrqHandler,    /* FLASH_IRQHandler */
    DummyIrqHandler,    /* RCC_IRQHandler */
    DummyIrqHandler,    /* EXTI0_IRQHandler */
    DummyIrqHandler,    /* EXTI1_IRQHandler */
    DummyIrqHandler,    /* EXTI2_IRQHandler */
    DummyIrqHandler,    /* EXTI3_IRQHandler */
    DummyIrqHandler,    /* EXTI4_IRQHandler */
    DummyIrqHandler,    /* DMA1_Channel1_IRQHandler */
    DummyIrqHandler,    /* DMA1_Channel2_IRQHandler */
    DummyIrqHandler,    /* DMA1_Channel3_IRQHandler */
    DummyIrqHandler,    /* DMA1_Channel4_IRQHandler */
    DummyIrqHandler,    /* DMA1_Channel5_IRQHandler */
    DummyIrqHandler,    /* DMA1_Channel6_IRQHandler */
    DummyIrqHandler,    /* DMA1_Channel7_IRQHandler */
    DummyIrqHandler,    /* ADC1_2_IRQHandler */
    DummyIrqHandler,    /* USB_HP_CAN1_TX_IRQHandler */
    DummyIrqHandler,    /* USB_LP_CAN1_RX0_IRQHandler */
    DummyIrqHandler,    /* CAN1_RX1_IRQHandler */
    DummyIrqHandler,    /* CAN1_SCE_IRQHandler */
    DummyIrqHandler,    /* EXTI9_5_IRQHandler */
    DummyIrqHandler,    /* TIM1_BRK_IRQHandler */
    DummyIrqHandler,    /* TIM1_UP_IRQHandler */
    DummyIrqHandler,    /* TIM1_TRG_COM_IRQHandler */
    DummyIrqHandler,    /* TIM1_CC_IRQHandler */
    DummyIrqHandler,    /* TIM2_IRQHandler */
    DummyIrqHandler,    /* TIM3_IRQHandler */
    DummyIrqHandler,    /* TIM4_IRQHandler */
    DummyIrqHandler,    /* I2C1_EV_IRQHandler */
    DummyIrqHandler,    /* I2C1_ER_IRQHandler */
    DummyIrqHandler,    /* I2C2_EV_IRQHandler */
    DummyIrqHandler,    /* I2C2_ER_IRQHandler */
    DummyIrqHandler,    /* SPI1_IRQHandler */
    DummyIrqHandler,    /* SPI2_IRQHandler */
    DummyIrqHandler,    /* USART1_IRQHandler */
    DummyIrqHandler,    /* USART2_IRQHandler */
    DummyIrqHandler,    /* USART3_IRQHandler */
    DummyIrqHandler,    /* EXTI15_10_IRQHandler */
    DummyIrqHandler,    /* RTC_Alarm_IRQHandler */
    DummyIrqHandler,    /* USBWakeUp_IRQHandler */
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    /* @0x108. This is for boot in RAM mode for
     * STM32F10x Medium Density devices.
     */
    DummyIrqHandler, /*BootRAM*/
};
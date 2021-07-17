/* 2021 Nikolai Chizhov */

#include <cstdint>

#include "register.hpp"
#include "regs_f103.hpp"
#include "port.hpp"
#include "pin.hpp"

/* Button.
 * I use 2 buttons for demo purporses. For example:
 *  - ButtonCfg is declared in 'init.cpp' for init only,
 *  - Button is declared in 'businessLogic.cpp' and must not be reconfigured
 */
using ButtonPort = Port<GPIOA>;
static constexpr uint8_t btnPinNum = 1;
using Button = Pin<ButtonPort, btnPinNum, PinMode::Read>; /* PA1, Read-only */

/* Led */
using Led = Pin<Port<GPIOA>, 0, PinMode::Allmighty>; /* PA0, Cfg + Read + Write */


static inline void example_pin() {
    /* 1) You can read the button's state */
    Button::Get();              /* is the button pressed? */

    /* 2) or you can do whatever you want with the led */
    Led::ConfigOutput();        /* config */
    Led::Set();                 /* turn the led on */
    Led::Reset();               /* turn the led off */
    Led::Get();                 /* is the led on? */
    Led::ConfigInput();         /* doesn't make sense, but you can do this */

    /* 3) but you cannot change the button's state */
    // Button::Reset();         /* comp. error, you cannot modify Read-only pins */
    // Button::ConfigOutput();  /* comp. error, you cannot config Read-only pins */
}

static inline void example_register() {
    /* Registers, CMSIS-like but protected */
    GPIOB::CRL::CRL0::OutPP50MHz::Set();    /* Init as Out-PP-50MHz */
    GPIOB::ODR::ODR0::High::Set();          /* Set logic 1 using ODR */
    GPIOB::BSRR::BR0::Low::Set();           /* Set logic 0 using BSR */
    GPIOB::ODR::Set(0x01);                  /* Wrire to the register */
    // GPIOB::BSRR::BS1::Get();             /* comp. error, BSRR is write-only */
    // GPIOB::IDR::Set(0x01);               /* comp. error, IDR is read-only */

    /* you can also use the pack to set several values to the same register */
    GPIOA::CRLSet<
        GPIOA::CRL::CRL0::OutPP50MHz,
        GPIOA::CRL::CRL1::OutPP2MHz,
        GPIOA::CRL::CRL7::InFloat
    >::Set();

    /* compilation error.
     * you cannot set a value from another port (GPIOB)
     */
    // GPIOA::CRLSet<
    //     GPIOA::CRL::CRL0::OutPP50MHz,
    //     GPIOB::CRL::CRL1::OutPP2MHz
    // >::Set();

    /* compilation error.
     * you cannot set a value from another register (CRH)
     */
    // GPIOA::CRLSet<
    //     GPIOA::CRL::CRL0::OutPP50MHz,
    //     GPIOA::CRH::CRH0::OutPP2MHz
    // >::Set();
}


static inline void mcu_low_level_init() {
    /* Turn GPIOA clock ON */
    RCC::APB2ENR::GPIOAEN::Enable::Set();

    /* Led: config and switch on */
    Led::ConfigOutput(); 	    /* GPIOA::CRL::CRL0::OutPP50MHz::Set(); */
    Led::Set();			        /* GPIOA::BSRR::BS0::High::Set(); */

    /* Button: just config it */
    using ButtonCfg = Pin<ButtonPort, btnPinNum, PinMode::Config>;
    ButtonCfg::ConfigInput<ButtonCfg::InputMode::PullDown>();
}

int main() {
    mcu_low_level_init();

    while (1) {
        constexpr uint32_t ticksMax = 200'000;
        const uint32_t ticks = Button::Get() ? ticksMax / 4: ticksMax;

        for (uint32_t i = 0; i < ticks; ++i);

        Led::Toggle();
    };
}

/* 2021 Nikolai Chizhov */

#pragma once

#include "register.hpp"


/* Description of peripheral registers for STM32F103
 *
 * The existing fields were created manually using the attached SVD file
 *   for PoC purporses.
 *
 * If it is required to add all the registers you can make a simple script.
 *
 */


/* * * * * * * *
 *  RCC
 * * * * * * * */

template <typename Reg, size_t offset, typename AccessMode, typename BaseType>
struct RCC_APB2ENR_Values : public RegisterField<Reg, offset, 1U, AccessMode> {
    using Disable = FieldValue<RCC_APB2ENR_Values, BaseType, 0U>;
    using Enable = FieldValue<RCC_APB2ENR_Values, BaseType, 1U>;
};

struct RCC {
private:
    static constexpr uintptr_t base = 0x40021000U;
    struct RCCAPB2ENRBase {};

public:
    struct APB2ENR : public Register<base + 0x18, 32U,  RegisterMode::RW> {
        /* ... */
        using GPIOCEN =
            RCC_APB2ENR_Values<RCC::APB2ENR, 4,  RegisterMode::RW, RCCAPB2ENRBase>;
        using GPIOBEN =
            RCC_APB2ENR_Values<RCC::APB2ENR, 3,  RegisterMode::RW, RCCAPB2ENRBase>;
        using GPIOAEN =
            RCC_APB2ENR_Values<RCC::APB2ENR, 2,  RegisterMode::RW, RCCAPB2ENRBase>;
        using AFIOEN =
            RCC_APB2ENR_Values<RCC::APB2ENR, 0,  RegisterMode::RW, RCCAPB2ENRBase>;

        using FieldValues =
            RCC_APB2ENR_Values<RCC::APB2ENR, 0, RegisterMode::None, RCCAPB2ENRBase>;
    };
    template <typename... T>
    using APB2ENRSet =
        RegisterFieldSet<base + 0x18, 32U,  RegisterMode::RW, RCCAPB2ENRBase, T...>;
};

/* * * * * * * *
 *  GPIO
 * * * * * * * */

template <typename Reg, size_t offset, typename AccessMode, typename BaseType>
struct GPIO_IDR_Values : public RegisterField<Reg, offset, 1U, AccessMode> {
    using IsHigh = FieldValue<GPIO_IDR_Values, BaseType, 0U>;
    using IsLow  = FieldValue<GPIO_IDR_Values, BaseType, 1U>;
};

template <typename Reg, size_t offset, typename AccessMode, typename BaseType>
struct GPIO_MODER_Values : public RegisterField<Reg, offset, 1U, AccessMode> {
    using Input     = FieldValue<GPIO_MODER_Values, BaseType, 0U>;
    using Output    = FieldValue<GPIO_MODER_Values, BaseType, 1U>;
    using Alternate = FieldValue<GPIO_MODER_Values, BaseType, 2U>;
    using Analog    = FieldValue<GPIO_MODER_Values, BaseType, 3U>;
};

template <typename Reg, size_t offset, typename AccessMode, typename BaseType>
struct GPIO_BSRR_BR_Values : public RegisterField<Reg, offset, 1U, AccessMode> {
    using Low = FieldValue<GPIO_BSRR_BR_Values, BaseType, 1U>;
};

template <typename Reg, size_t offset, typename AccessMode, typename BaseType>
struct GPIO_BSRR_BS_Values : public RegisterField<Reg, offset, 1U, AccessMode> {
    using High = FieldValue<GPIO_BSRR_BS_Values, BaseType, 1U>;
};

template <typename Reg, size_t offset, typename AccessMode, typename BaseType>
struct GPIO_ODR_Values : public RegisterField<Reg, offset, 1U, AccessMode> {
    using Low  = FieldValue<GPIO_ODR_Values, BaseType, 0U>;
    using High = FieldValue<GPIO_ODR_Values, BaseType, 1U>;
};

template <typename Reg, size_t offset, typename AccessMode, typename BaseType>
struct GPIO_CR_Value : public RegisterField<Reg, offset, 4U, AccessMode> {
    /* as Input */
    using InAnalog      = FieldValue<GPIO_CR_Value, BaseType, 0b0000>;
    using InFloat       = FieldValue<GPIO_CR_Value, BaseType, 0b0100>;
    using InPushPull    = FieldValue<GPIO_CR_Value, BaseType, 0b1000>;
    /* as Output */
    using OutPP50MHz    = FieldValue<GPIO_CR_Value, BaseType, 0b0011>;
    using OutPP2MHz     = FieldValue<GPIO_CR_Value, BaseType, 0b0010>;
    using OutPP10MHz    = FieldValue<GPIO_CR_Value, BaseType, 0b0001>;
};


template <uintptr_t addr>
struct GPIO {
private:
    struct GPIOBSRRBase  {};
    struct GPIOMODERBase {};
    struct GPIOIDRBase   {};
    struct GPIOODRBase   {};
    struct GPIOCRLBase   {};
    struct GPIOCRHBase   {};

public:
    /* Control register (Low) */
    struct CRL : public Register<addr + 0x00, 32,  RegisterMode::RW> {
        using CRL7 =
            GPIO_CR_Value<GPIO::CRL, 28,  RegisterMode::RW, GPIOCRLBase>;
        /* ... */
        using CRL1 =
            GPIO_CR_Value<GPIO::CRL, 4,   RegisterMode::RW, GPIOCRLBase>;
        using CRL0 =
            GPIO_CR_Value<GPIO::CRL, 0,   RegisterMode::RW, GPIOCRLBase>;

        using FieldValues =
            GPIO_CR_Value<GPIO::CRL, 0, RegisterMode::None, GPIOCRLBase>;
    };
    template <typename... T>
    using CRLSet =
        RegisterFieldSet<addr + 0x00, 32,  RegisterMode::RW, GPIOCRLBase, T...>;

    /* Control register (High) */
    struct CRH : public Register<addr + 0x04, 32,  RegisterMode::RW> {
        using CRH7 =
            GPIO_CR_Value<GPIO::CRH, 28,  RegisterMode::RW, GPIOCRHBase>;
        /* ... */
        using CRH1 =
            GPIO_CR_Value<GPIO::CRH, 4,   RegisterMode::RW, GPIOCRHBase>;
        using CRH0 =
            GPIO_CR_Value<GPIO::CRH, 0,   RegisterMode::RW, GPIOCRHBase>;

        using FieldValues =
            GPIO_CR_Value<GPIO::CRH, 0, RegisterMode::None, GPIOCRHBase>;
    };
    template <typename... T>
    using CRHSet =
        RegisterFieldSet<addr + 0x00, 32,  RegisterMode::RW, GPIOCRHBase, T...>;

    /* IDR */
    struct IDR : public Register<addr + 0x08, 32, RegisterMode::Read> {
        using IDR15 =
            GPIO_IDR_Values<GPIO::IDR, 15, RegisterMode::Read, GPIOIDRBase>;
        /* ... */
        using IDR1 =
            GPIO_IDR_Values<GPIO::IDR, 0, RegisterMode::Read, GPIOIDRBase>;
        using IDR0 =
            GPIO_IDR_Values<GPIO::IDR, 0, RegisterMode::Read, GPIOIDRBase>;

        using FieldValues =
            GPIO_IDR_Values<GPIO::IDR, 0, RegisterMode::None, GPIOIDRBase>;
    };
    template <typename... T>
    using IDRSet =
        RegisterFieldSet<addr + 0x08, 32, RegisterMode::Read, GPIOIDRBase, T...>;

    /* ODR */
    struct ODR : public Register<addr + 0x0C, 32,  RegisterMode::RW> {
        using ODR15 =
            GPIO_ODR_Values<GPIO::ODR, 15,  RegisterMode::RW, GPIOODRBase>;
        /* ... */
        using ODR1 =
            GPIO_ODR_Values<GPIO::ODR, 1,  RegisterMode::RW, GPIOODRBase>;
        using ODR0 =
            GPIO_ODR_Values<GPIO::ODR, 0,  RegisterMode::RW, GPIOODRBase>;

        using FieldValues =
            GPIO_ODR_Values<GPIO::ODR, 0, RegisterMode::None, GPIOODRBase>;
    };
    template <typename... T>
    using ODRSet =
        RegisterFieldSet<addr + 0x0C, 32, RegisterMode::Read, GPIOODRBase, T...>;

    /* BSRR */
    struct BSRR : public Register<addr + 0x10, 32, RegisterMode::Write> {
        using BR15 =
            GPIO_BSRR_BR_Values<GPIO::BSRR, 31, RegisterMode::Write, GPIOBSRRBase>;
        /* ... */
        using BR1 =
            GPIO_BSRR_BR_Values<GPIO::BSRR, 17, RegisterMode::Write, GPIOBSRRBase>;
        using BR0 =
            GPIO_BSRR_BR_Values<GPIO::BSRR, 16, RegisterMode::Write, GPIOBSRRBase>;

        using BS15 =
            GPIO_BSRR_BS_Values<GPIO::BSRR, 15, RegisterMode::Write, GPIOBSRRBase>;
        /* ... */
        using BS1 =
            GPIO_BSRR_BS_Values<GPIO::BSRR, 1, RegisterMode::Write, GPIOBSRRBase>;
        using BS0 =
            GPIO_BSRR_BS_Values<GPIO::BSRR, 0, RegisterMode::Write, GPIOBSRRBase>;
    };
    template <typename... T>
    using BSRRSet =
        RegisterFieldSet<addr + 0x10, 32, RegisterMode::Write, GPIOBSRRBase, T...>;
};


using GPIOA = GPIO<0x40010800>;
using GPIOB = GPIO<0x40010C00>;
using GPIOC = GPIO<0x40011000>;
using GPIOD = GPIO<0x40011400>;

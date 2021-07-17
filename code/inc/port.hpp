/* 2021 Nikolai Chizhov */

#pragma once

#include <cassert>
#include <cstdint>

#include "utils.hpp"
#include "register.hpp"


/* template for Port class
 *
 */
template <typename T>
class Port {
public:
    static inline constexpr void Toggle(uint32_t value) {
        T::ODR::Toggle(static_cast<typename T::ODR::Type>(value));
    }

    static inline constexpr void Set(uint32_t value) {
        T::BSRR::Set(static_cast<typename T::BSRR::Type>(value));
    }

    static inline constexpr auto Get() {
        return T::IDR::Get();
    }

    static constexpr void SetOutput(uint32_t pinNum) {
        assert(pinNum <= pinNumMax);

        using Type = typename T::CRL::Type;
        using Field = typename T::CRL::FieldValues::OutPP50MHz;

        Utils::Sync::Atomic<Type>::Set(
            T::CRL::Address,
            Field::Mask,
            Field::Value,
            static_cast<Type>(pinNum * 4U)
        );
    }

    static constexpr void SetInput(uint8_t pinNum) {
        assert(pinNum <= pinNumMax);

        using Type = typename T::CRL::Type;
        using Field = typename T::CRL::FieldValues::InPushPull;

        Utils::Sync::Atomic<Type>::Set(
            T::CRL::Address,
            Field::Mask,
            Field::Value,
            static_cast<Type>(pinNum * 4U)
        );
    }

private:
    static constexpr uint32_t pinNumMax = 15;
};

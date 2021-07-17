/* 2021 Nikolai Chizhov */

#pragma once

#include <cassert>
#include <cstdint>

#include "utils.hpp"
#include "port.hpp"

struct PinMode {
    /* Read-only:
     * If you try to modify it (set, reset, etc) you will get the comp. error
     */
    struct Read
    {};
    /* Write-only:
     * If you try to read it (get) you will get the comp. error
     */
    struct Write
    {};
    /* Configurable
     * If you try to config a pin without this mode you will get a comp. error
     */
    struct Config
    {};
    /* Configurable + Readable */
    struct ReadConfig : public Config, public Read
    {};
    /* Configurable + Writeble */
    struct WriteConfig : public Config, public Write
    {};
    /* Allmighty:
     * No restrinctions
     */
    struct Allmighty : public Config, public Read, public Write
    {};
};


/* template for Pin class
 *
 * Port         - port template from port.hpp
 * pinNum       - a corresponding pin number, as is
 * AccessMode   - allowed PinModes described above
 */
template <typename Port, uint8_t pinNum, typename AccessMode>
class Pin {
public:
    enum class InputMode {
        PullUp,
        PullDown
    };

    static inline auto Get() {
        CheckMode<PinMode::Read>();

        return (Port::Get() & (1U << pinNum)) >> pinNum;
    }

    static inline void Toggle() {
        CheckMode<PinMode::Write>();

        Port::Toggle(1U << pinNum);
    }

    static inline void Set() {
        CheckMode<PinMode::Write>();

        SetInternal();
    }

    static inline void Reset() {
        CheckMode<PinMode::Write>();

        ResetInternal();
    }

    static void ConfigOutput() {
        CheckMode<PinMode::Config>();

        Port::SetOutput(pinNum);
    }

    template <InputMode InMode = InputMode::PullUp>
    static void ConfigInput() {
        CheckMode<PinMode::Config>();

        /* init as input */
        Port::SetInput(pinNum);

        /* push-pull */
        if constexpr (InMode == InputMode::PullDown) {
            ResetInternal();
        } else if constexpr (InMode == InputMode::PullUp) {
            SetInternal();
        } else {
            static_assert(Utils::dependentBool<false>, "Unknown Pin mode");
        }
    }
private:
    static constexpr uint8_t pinNumMax = 15;
    static_assert(pinNum <= pinNumMax, "There are only 16 pins on port");

    /* Check the mode, instead of SFINAE */
    template <typename T>
    static constexpr void CheckMode() {
        static_assert(std::is_base_of_v<T, AccessMode>);
    }

    /* these 2 methods declared here as we could use Set and Reset
     * for both writeble (directly) and readable (pull-up/down) targets
     */

    static inline void SetInternal() {
        Port::Set(1U << pinNum);
    }

    static inline void ResetInternal() {
        Port::Set((1U << (pinNum)) << (pinNumMax + 1));
    }
};

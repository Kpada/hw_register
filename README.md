# C++ Hardware Register Access

## What is this about?

How do we usually work with hardware registers?
The structures of registers and their fields are defined in vendor's header files (or something similar). And we just do something like this:

```cpp
/* Example for STM32 */
GPIOA->ODR = 0x01;  /* Port A, Pin 0. Set High */
GPIOA->IDR = 0x01;  /* Read-only register. Obviously, we wanted to do something different */
```

Is there a way to prevent these issues without sacrificing efficiency?

Actually, everything about a register is listed in the datasheet. This means, that everything we need is known at compile time. We have its address, its offset, how to use it, and we know whether the register is writable, readable, or both.

Thus, using C++ templates, we can ask the compiler to perform these checks for us. In case of violations, we will receive a compilation error. 

Here you can see my experiments on:
1) protection from such nonsense
```cpp
GPIOA::ODR::Set(0x01);  /* Ok, Set High */
GPIOA::IDR::Set(0x01);  /* compilation error, IDR is read-only */
```

2) HW abstractions
```cpp
using Led = Pin<Port<GPIOA>, 0, PinMode::WriteConfig>;
Led::ConfigOutput();    /* config */
Led::Set();             /* turn the led on */
```

## Hardware

The project was created for my blue STM32F103C8 board, but it won't take long to change it for any other MCU.

You will need:
 - STM32F103C8 board
 - Debugger / Flasher (ST-Link, J-Link, etc.)


## How to build (Linux)

Tools:
- CMake
- arm-none-eabi-g++

```
mkdir build
cd build
cmake ..
make
```

## VS code plugins
 - C/C++
 - Cortex-Debug
 - Disassembly Explorer
 - CMake

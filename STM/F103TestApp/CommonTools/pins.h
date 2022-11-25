/*
 * pins.h
 *
 *  Created on: Nov 7, 2022
 *      Author: VertexNi
 */

#ifndef SRC_WRAPPERS_PINS_H_
#define SRC_WRAPPERS_PINS_H_
#include "stm32f1xx_hal.h"

enum Ports
{
    GPIOa, GPIOc, GPIOd
};

enum Configuration
{
    In = 0,
    Out = 1,
    AltFunc = 2,
    Analog = 3,
    ItRising = 0x10110000u,
	ItFalling = 0x10210000u,
	ItRaisingFalling = 0x10310000u
};

// Pull mode
enum PullMode
{
    NoPull = 0,
    PullUp   = 1,
    PullDown = 2
};

// Port driver type
enum DriverType
{
    PushPull = 0,   ///< Push-pull
    OpenDrain = 1   ///< Open-drain
};

// Port speed
enum Speed
{
    Slow = 0, ///< Slow (< 2 MHz)
    Medium = 1, ///< Medium (< 10 MHz)
    Fast = 2, ///< Fast (< 50MHz)
    Fastest = 3 ///< Fastest (< 50MHz)
};

template<Ports PORT>
    class PortBase
    {
    public:
        static void configurePin(GPIO_InitTypeDef& gPIO_InitStruct, Configuration mode, PullMode pullMode, Speed speed, uint16_t pin)
        {
            uint16_t pinAddress = 0x0001 << pin;
            gPIO_InitStruct.Pin = pinAddress;
            gPIO_InitStruct.Mode = mode;
            gPIO_InitStruct.Pull = pullMode;
            gPIO_InitStruct.Speed = speed;
        }
    };

template<Ports PORT>
    class Port:public PortBase<PORT>
{

};

template<>
    class Port<Ports::GPIOa>:public PortBase<Ports::GPIOa>
    {
    static void configurePin(Configuration mode, PullMode pullMode, Speed speed, uint16_t pin)
    {
        GPIO_InitTypeDef gPIO_InitStruct;
        PortBase<Ports::GPIOa>::configurePin(gPIO_InitStruct, mode, pullMode, speed, pin);
        HAL_GPIO_Init(GPIOA, &gPIO_InitStruct);
    }
    public:
        static void
        enable ()
        {
            __HAL_RCC_GPIOA_CLK_ENABLE();

        }

        template <typename ...T> static void reset(T... pins)
        {
            (HAL_GPIO_WritePin(GPIOA, 0x0001 << pins, GPIO_PIN_RESET), ...);
        }

        template <typename ...T> static void set(T... pins)
        {
            (HAL_GPIO_WritePin(GPIOA, 0x0001 << pins, GPIO_PIN_SET), ...);
        }

        template <typename ...T> static void configure(Configuration mode, PullMode pullMode, Speed speed, T... pins)
        {
            (configurePin(mode, pullMode, speed, pins), ...);
        }
    };


template<>
    class Port<Ports::GPIOc>:public PortBase<Ports::GPIOc>
    {
        static void configurePin(Configuration mode, PullMode pullMode, Speed speed, uint16_t pin)
        {
            GPIO_InitTypeDef gPIO_InitStruct;
            PortBase<Ports::GPIOc>::configurePin(gPIO_InitStruct, mode, pullMode, speed, pin);
            HAL_GPIO_Init(GPIOC, &gPIO_InitStruct);
        }
    public:
        static void
        enable ()
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();
        }

        template <typename ...T> static void reset(T... pins)
        {
            (HAL_GPIO_WritePin(GPIOC, 0x0001 << pins, GPIO_PIN_RESET), ...);
        }

        template <typename ...T> static void set(T... pins)
        {
            (HAL_GPIO_WritePin(GPIOC, 0x0001 << pins, GPIO_PIN_SET), ...);
        }

        template <typename ...T> static void configure(Configuration mode, PullMode pullMode, Speed speed, T... pins)
        {
            (configurePin(mode, pullMode, speed, pins), ...);
        }
    };

template<>
    class Port<Ports::GPIOd>:public PortBase<Ports::GPIOd>
    {
    static void configurePin(Configuration mode, PullMode pullMode, Speed speed, uint16_t pin)
    {
        GPIO_InitTypeDef gPIO_InitStruct;
        PortBase<Ports::GPIOd>::configurePin(gPIO_InitStruct, mode, pullMode, speed, pin);
        HAL_GPIO_Init(GPIOD, &gPIO_InitStruct);
    }
    public:
        static void
        enable ()
        {
            __HAL_RCC_GPIOD_CLK_ENABLE();
        }

        template <typename ...T> static void reset(T... pins)
        {
            (HAL_GPIO_WritePin(GPIOD, 0x0001 << pins, GPIO_PIN_RESET), ...);
        }

        template <typename ...T> static void set(T... pins)
        {
            (HAL_GPIO_WritePin(GPIOD, 0x0001 << pins, GPIO_PIN_SET), ...);
        }

        template <typename ...T> static void configure(Configuration mode, PullMode pullMode, Speed speed, T... pins)
        {
            (configurePin(mode, pullMode, speed, pins), ...);
        }
    };

#endif /* SRC_WRAPPERS_PINS_H_ */

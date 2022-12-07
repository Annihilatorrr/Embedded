/*
 * pins.h
 *
 *  Created on: Nov 7, 2022
 *      Author: VertexNi
 */

#ifndef PINS_STM32F1_H_
#define PINS_STM32F1_H_
#include "stm32f1xx_hal.h"
#include "definitions.h"

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
    class Port<Ports::GPIOb>:public PortBase<Ports::GPIOb>
    {
    static void configurePin(Configuration mode, PullMode pullMode, Speed speed, uint16_t pin)
    {
        GPIO_InitTypeDef gPIO_InitStruct;
        PortBase<Ports::GPIOb>::configurePin(gPIO_InitStruct, mode, pullMode, speed, pin);
        HAL_GPIO_Init(GPIOB, &gPIO_InitStruct);
    }
    public:
        static void
        enable ()
        {
            __HAL_RCC_GPIOB_CLK_ENABLE();
        }

        template <typename ...T> static void reset(T... pins)
        {
            (HAL_GPIO_WritePin(GPIOB, 0x0001 << pins, GPIO_PIN_RESET), ...);
        }

        template <typename ...T> static void set(T... pins)
        {
            (HAL_GPIO_WritePin(GPIOB, 0x0001 << pins, GPIO_PIN_SET), ...);
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

#endif

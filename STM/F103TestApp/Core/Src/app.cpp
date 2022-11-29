/*
 * app.cpp
 *
 *  Created on: Nov 8, 2022
 *      Author: VertexNi
 */

#include "app.h"
#include "CommonTools/pins.h"
void
App::initializePorts ()
{
    Port<Ports::GPIOa>::enable ();
    Port<Ports::GPIOc>::enable ();
    Port<Ports::GPIOd>::enable ();

    // Pins C13, C14 are used as output ports for signaling purpose (connected to LEDs)
    Port<Ports::GPIOc>::reset (13, 14);

    Port<Ports::GPIOc>::configure (Configuration::Out, PullMode::NoPull,
                                   Speed::Fastest, 13, 14);

    Port<Ports::GPIOa>::configure (Configuration::ItRaisingFalling, PullMode::NoPull,
                                   Speed::Fastest, 0);
    Port<Ports::GPIOa>::configure (Configuration::ItRaisingFalling, PullMode::NoPull,
                                   Speed::Fastest, 1);

    // Pint C5, C6, C7 are used for SPI (clock, cs, data)
    Port<Ports::GPIOa>::reset (4);
    Port<Ports::GPIOa>::configure (Configuration::Out, PullMode::NoPull,
                                   Speed::Medium, 4);
}

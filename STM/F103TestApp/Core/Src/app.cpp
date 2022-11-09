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

    Port<Ports::GPIOc>::reset (13, 14, 15);

    Port<Ports::GPIOc>::configure (Configuration::Out, PullMode::NoPull,
                                   Speed::Fastest, 13, 14, 15);

    Port<Ports::GPIOa>::configure (Configuration::ItRising, PullMode::NoPull,
                                   Speed::Fastest, 0);
}

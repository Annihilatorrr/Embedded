#ifndef DEFINITIONS_H
#define DEFINITIONS_H

enum Ports
{
    GPIOa, GPIOb, GPIOc, GPIOd
};

enum Configuration
{
    In = 0x00000000u,
    OutPushPull = 0x00000001u,
    AltFunc = 0x00000002u,
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
#endif

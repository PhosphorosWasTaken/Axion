#pragma once

#include <stdint.h>

#ifndef BUTTONS_H
#define BUTTONS_H
// MCP23017 button interrupts
#define mcp23s17_MOSI 19
#define mcp23s17_MISO 21
#define mcp23s17_CS   24

// button memory locations

// TODO: change with actual location later
#define BUTTONSTATES 0xFF // stores the current button states(pressed, rising, falling) in memory

// defined memory offsets for buttons. used by & it with the button states to get a binary value
#define W         0x0000000000000001
#define W_RISING  0x0000000000000002
#define W_FALLING 0x0000000000000003

#define A         0x0000000000000004
#define A_RISING  0x0000000000000005
#define A_FALLING 0x0000000000000006

#define S         0x0000000000000007
#define S_RISING  0x0000000000000008
#define S_FALLING 0x0000000000000009

#define D         0x000000000000000A
#define S_RISING  0x000000000000000B
#define S_FALLING 0x000000000000000C

#define DPAD_UP         0x000000000000000D
#define DPAD_UP_RISING  0x000000000000000E
#define DPAD_UP_FALLING 0x000000000000000F

#define DPAD_DOWN         0x0000000000000010
#define DPAD_DOWN_RISING  0x0000000000000011
#define DPAD_DOWN_FALLING 0x0000000000000012

#define DPAD_LEFT         0x0000000000000013
#define DPAD_LEFT_RISING  0x0000000000000014
#define DPAD_LEFT_FALLING 0x0000000000000015

#define DPAD_RIGHT         0x0000000000000016
#define DPAD_RIGHT_RISING  0x0000000000000017
#define DPAD_RIGHT_FALLING 0x0000000000000018

#define LEFT_TRIGGER         0x0000000000000019
#define LEFT_TRIGGER_RISING  0x000000000000001A
#define LEFT_TRIGGER_FALLING 0x000000000000001B

#define RIGHT_TRIGGER         0x000000000000001C
#define RIGHT_TRIGGER_RISING  0x000000000000001D
#define RIGHT_TRIGGER_FALLING 0x000000000000001E

void initialize_mcp23s17();

void get_button_state(int button);

#endif
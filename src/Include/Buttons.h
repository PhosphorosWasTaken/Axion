// Copyright (C) 2026  PhosphorosWasTaken
// Copyright (C) 2026  p123o215
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://gnu.org>.
//
// See the LICENSE file in the project root for additional terms
// appended under GPL v3 Section 7 regarding attribution screens.

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
#define W             0x000000000001
#define A             W << 1
#define S             A << 1
#define D             S << 1
#define DPAD_UP       D << 1
#define DPAD_DOWN     DPAD_UP << 1
#define DPAD_LEFT     DPAD_DOWN << 1
#define DPAD_RIGHT    DPAD_LEFT << 1
#define LEFT_TRIGGER  DPAD_RIGHT << 1
#define RIGHT_TRIGGER LEFT_TRIGGER << 1

#define W_RISING             RIGHT_TRIGGER << 1
#define A_RISING             W_RISING << 1d
#define S_RISING             A_RISING << 1
#define D_RISING             S_RISING << 1
#define DPAD_UP_RISING       D_RISING << 1
#define DPAD_DOWN_RISING     DPAD_UP_RISING << 1
#define DPAD_LEFT_RISING     DPAD_DOWN_RISING << 1
#define DPAD_RIGHT_RISING    DPAD_LEFT_RISING << 1
#define LEFT_TRIGGER_RISING  DPAD_RIGHT_RISING << 1
#define RIGHT_TRIGGER_RISING LEFT_TRIGGER_RISING <<1

#define W_FALLING             RIGHT_TRIGGER_RISING << 1
#define A_FALLING             W_FALLING << 1
#define S_FALLING             A_FALLING << 1
#define D_FALLING             S_FALLING << 1
#define DPAD_UP_FALLING       D_FALLING << 1
#define DPAD_DOWN_FALLING     DPAD_UP_FALLING << 1
#define DPAD_LEFT_FALLING     DPAD_DOWN_FALLING << 1
#define DPAD_RIGHT_FALLING    DPAD_LEFT_FALLING << 1
#define LEFT_TRIGGER_FALLING  DPAD_RIGHT_FALLING << 1
#define RIGHT_TRIGGER_FALLING LEFT_TRIGGER_FALLING << 1

/*
we do not currently have other buttons that need to be pressed, so i will not add headers for them.
*/

void initialize_mcp23s17();

void get_button_state(int button);

#endif
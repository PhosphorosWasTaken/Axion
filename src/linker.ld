/* Copyright (C) 2026  PhosphorosWasTaken */
/* Copyright (C) 2026  p123o215 */
/*
/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */
/*
/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */
/*
/* You should have received a copy of the GNU General Public License */
/* along with this program.  If not, see <https://gnu.org>. */
/*
/* See the LICENSE file in the project root for additional terms  */
/* appended under GPL v3 Section 7 regarding attribution screens. */

ENTRY(_start)

SECTIONS
{
    . = 0x80000;

    .text : {
        KEEP(*(.text.boot))
        *(.text .text.*)
    }

    .rodata : {
        *(.data .data.*)
    }

    .data : {
        *(.data .data.*)
    }

    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }
}

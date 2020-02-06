/* Copyright 2017 Columbia University, SLD Group */

//
// defines.h - Robert Margelli
// This file contains several defines. Some of which must be
// commented/uncommented correctly before running.
//

#ifndef DEFINES_H
#define DEFINES_H

// Enable/disable multiplier, divider, CSR.

#define MUL32       1 // Enable 32x32 multiplier for MUL
#define MUL64       1 // Enable 64x64 multiplier for MULH, MULHSU, MULHU
#define DIV         1 // Enable division operations DIV, DIVU
#define REM         1 // Enable remainder operations REM, REMU
#define CSR_LOGIC   1 // Enable CSR logic in exe stage.


// Cache size
#define ICACHE_SIZE 51200
#define DCACHE_SIZE 51200

#define TAG_WIDTH 4
#define FWD_ENABLE

// Dbg directives.

#define INTERNAL_PROG // When on specifies the program to execute as an array in the fetch stage (not for production).

#define VERBOSE

#endif // DEFINES_H

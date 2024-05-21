#include <stdio.h>
#include <stdlib.h>
#include <arch/timer.h>

/*
 * Compression Algorithm Summary:
 * 
 * This algorithm is designed to output compressed binary entries to a 'trace.bin' file,
 * leveraging the -finstrument-functions functionality of GCC for function
 * instrumentation.
 *
 * Each entry in the output file has the following format:
 *   1. A '<' or '>' character indicating whether the function was entered or exited.
 *   2. A 3-byte address representing the function's address in memory.
 *   3. A 1-byte length field specifying the number of following bytes.
 *   4. The following bytes, representing the delta-encoded cycle timestamp of
 *      entering or leaving the function.
 *
 * The main purpose of this algorithm is to reduce the overhead of function
 * instrumentation and to provide a compact representation of function call traces,
 * allowing for efficient storage and processing of the collected data using 'dctrace'.
 *
 * Delta compression is used for the cycle timestamps to further reduce the size
 * of the output file. This compression method encodes timestamps as the difference
 * between the current timestamp and the previous one, resulting in smaller values
 * that often require fewer bytes to represent.
 */

#define LIKELY(exp)    __builtin_expect(!!(exp), 1)
#define UNLIKELY(exp)  __builtin_expect(!!(exp), 0)

#define BASE_ADDRESS 0x8c010000

void __attribute__ ((no_instrument_function, hot)) __cyg_profile_func_enter(void *this, void *callsite) {
    //printf("__cyg_profile_func_enter\n");
}

void __attribute__ ((no_instrument_function, hot)) __cyg_profile_func_exit(void *this, void *callsite) {
    //printf("__cyg_profile_func_exit\n");
}

void __attribute__ ((no_instrument_function, constructor)) main_constructor(void) {
    // extern char _etext;
    // extern void _monstartup (uintptr_t lowpc, uintptr_t highpc);

    //_monstartup (BASE_ADDRESS, _etext);

    //printf("_monstartup\n");
}

void __attribute__ ((no_instrument_function, destructor)) main_destructor(void) {

}
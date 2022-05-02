#ifndef CONFIG_H
#define CONFIG_H

#include "constants.h"

#define TRIPHONIC // polyphony mode: MONOPHONIC || DUOPHONIC || TRIPHONIC 

#if defined(MONOPHONIC)
    #define CV_1_SOURCE            NOTE // NOTE || VELOCITY || CC1 || CC2 || CC3 || PITCHBEND
    #define CV_2_SOURCE            NOTE // NOTE || VELOCITY || CC1 || CC2 || CC3 || PITCHBEND
    #define CV_3_SOURCE            NOTE // NOTE || VELOCITY || CC1 || CC2 || CC3 || PITCHBEND 
#elif defined(DUOPHONIC)
    #define CV_1_AND_2_MODE        LAST // HIGHEST || LOWEST || LAST 
    #define CV_3_SOURCE            NOTE // NOTE || VELOCITY || CC1 || CC2 || CC3 || PITCHBEND 
#elif defined(TRIPHONIC)
    #define CV_1_AND_2_AND_3_MODE  LAST // HIGHEST || LOWEST || LAST
#endif

#define CV_1_SCALE V_OCT  // V_OCT || HZ_V  (CV_1_SCALE determines scale for all polyphonic cvs)
#define CV_2_SCALE V_OCT  // V_OCT || HZ_V
#define CV_3_SCALE V_OCT  // V_OCT || HZ_V

// CCs will output CV scaled from the sent CC value, listening on n number CCx
#define CC1_COMMAND      7    // 0...119
#define CC2_COMMAND      10   // 0...119
#define CC3_COMMAND      11   // 0...119

// clocks will send trigger every n number of midi clocks (there are 24 midi clocks per beat)
#define CLOCK_1_DIVIDER  12   // 1...infinity
#define CLOCK_2_DIVIDER  24   // 1...infinity
#define CLOCK_3_DIVIDER  48   // 1...infinity

// resets will send trigger on MIDI_START, MIDI_STOP, MIDI CONTINUE, and every n number of beats (where 24 midi clocks is 1 beat)
#define RESET_1_BEATS    4    // 0(just at the start)...infinity
#define RESET_2_BEATS    16   // 0(just at the start)...infinity
#define RESET_3_BEATS    32   // 0(just at the start)...infinity

// #define USE_SUSTAIN

// TRANSPORT sets gate high after MIDI_START or MIDI_CONTINUE, and low after MIDI_STOP
// NOTE_ON_OFF sets gate high after NOTE_ON and low after NOTE_OFF, listening on n note number
// CC1, CC2, CC3 will set gate high if value received on CCx (number defined above for each) is greater then 63, and low if less then 64
#define GATE_1_SOURCE    CLOCK_1 // NOTE_ON_OFF || CLOCK_1 || CLOCK_2 || CLOCK_3 || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1 || CC2 || CC3
#define GATE_1_NOTE      C6          // C_2...G8 || 0...127
// #define GATE_1_INVERT
#define GATE_2_SOURCE    CLOCK_2 // NOTE_ON_OFF || CLOCK_1 || CLOCK_2 || CLOCK_3 || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1 || CC2 || CC3
#define GATE_2_NOTE      Cs6         // C_2...G8 || 0...127
// #define GATE_2_INVERT
#define GATE_3_SOURCE    CLOCK_3 // NOTE_ON_OFF || CLOCK_1 || CLOCK_2 || CLOCK_3 || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1 || CC2 || CC3
#define GATE_3_NOTE      D6          // C_2...G8 || 0...127
// #define GATE_3_INVERT
#define GATE_4_SOURCE    NOTE_ON_OFF // NOTE_ON_OFF || CLOCK_1 || CLOCK_2 || CLOCK_3 || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1 || CC2 || CC3
#define GATE_4_NOTE      Ds6         // C_2...G8 || 0...127
// #define GATE_4_INVERT
#define GATE_5_SOURCE    NOTE_ON_OFF // NOTE_ON_OFF || CLOCK_1 || CLOCK_2 || CLOCK_3 || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1 || CC2 || CC3
#define GATE_5_NOTE      E6          // C_2...G8 || 0...127
// #define GATE_5_INVERT
#define GATE_6_SOURCE    NOTE_ON_OFF // NOTE_ON_OFF || CLOCK_1 || CLOCK_2 || CLOCK_3 || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1 || CC2 || CC3
#define GATE_6_NOTE      F6          // C_2...G8 || 0...127
// #define GATE_6_INVERT
#define GATE_7_SOURCE    NOTE_ON_OFF // NOTE_ON_OFF || CLOCK_1 || CLOCK_2 || CLOCK_3 || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1 || CC2 || CC3
#define GATE_7_NOTE      Fs6         // C_2...G8 || 0...127
// #define GATE_7_INVERT
#define GATE_8_SOURCE    NOTE_ON_OFF // NOTE_ON_OFF || CLOCK_1 || CLOCK_2 || CLOCK_3 || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1 || CC2 || CC3
#define GATE_8_NOTE      G6          // C_2...G8 || 0...127
// #define GATE_8_INVERT

#endif
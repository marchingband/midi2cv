#include <Arduino.h>
#include "handlers.h"
#include "config.h"
#include "constants.h"
#include "dac.h"
#include "gates.h"
#include "polyphony.h"

struct cv_t cvs[3];
struct gate_t gates[8];

unsigned long clock_cnt = 0;
unsigned long last_start = 0;
unsigned long beat_cnt = 0;
bool playing = false;

void cv_1_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
#ifdef MONOPHONIC
    if((CV_1_SOURCE == NOTE) && is_note_on)
    {
        note_to_dac(0, note, CV_1_SCALE == V_OCT);
    }
    else if((CV_1_SOURCE == VELOCITY) && is_note_on)
    {
        velocity_to_dac(0, velocity);
    }
#endif
}

void cv_2_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
#ifdef MONOPHONIC
    if((CV_2_SOURCE == NOTE) && is_note_on)
    {
        note_to_dac(1, note, CV_2_SCALE == V_OCT);
    }
    else if((CV_2_SOURCE == VELOCITY) && is_note_on)
    {
        // char log[30];
        // sprintf(log, "cv_2_handle_note_on_off vel-%d", velocity);
        // Serial1.println(log);
        velocity_to_dac(1, velocity);
    }
#endif
}

void cv_3_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
#ifndef TRIPHONIC
    if((CV_3_SOURCE == NOTE) && is_note_on)
    {
        note_to_dac(2, note, CV_3_SCALE == V_OCT);
    }
    else if((CV_3_SOURCE == VELOCITY) && is_note_on)
    {
        velocity_to_dac(2, velocity);
    }
#endif
}

void duophonic_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
#ifdef DUOPHONIC
    if(
        (CV_1_SCALE == V_OCT) &&
        ((note < V_OCT_MIN) || (note > V_OCT_MAX))
    )
    {
        return;
    }
    if(
        (CV_1_SCALE == HZ_V) &&
        ((note < HZ_V_MIN) || (note > HZ_V_MAX))
    )
    {
        return;
    }
    duophonic_process_event(channel, note, velocity, is_note_on);
#endif
}

void triphonic_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
#ifdef TRIPHONIC
    if(
        (CV_1_SCALE == V_OCT) &&
        ((note < V_OCT_MIN) || (note > V_OCT_MAX))
    )
    {
        return;
    }
    if(
        (CV_1_SCALE == HZ_V) &&
        ((note < HZ_V_MIN) || (note > HZ_V_MAX))
    )
    {
        return;
    }
    triphonic_process_event(channel, note, velocity, is_note_on);
#endif
}

void gates_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
    // Serial1.println("gates_handle_note_on_off");
    for(int i=0; i<8; i++)
    {
        if((gates[i].source == NOTE_ON_OFF) && (gates[i].note == note))
        {
            if(is_note_on)
            {
                gate_set(i, gates[i].invert ? 0 : 1);
            }
            else // note off
            {
                gate_set(i, gates[i].invert ? 1 : 0);
            }
        }
    }
}

void gates_handle_monophonic_note_on_off(uint8_t channel, uint8_t note, bool is_note_on)
{
    // Serial1.println("gates_handle_monophonic_note_on_off");
    for(int i=0; i<8; i++)
    {
        if(gates[i].source == MONOPHONIC_ON_OFF)
        {
            if(is_note_on)
            {
                gate_set(i, gates[i].invert ? 0 : 1);
            }
            else if(cvs[0].note == note) // only obey note off if its the current note
            {
                gate_set(i, gates[i].invert ? 1 : 0);
            }
        }
    }
}


void gates_handle_duophonic_note_on_off(uint8_t channel, uint8_t voice, bool is_note_on)
{
    for(int i=0; i<8; i++)
    {
        if(
            ((gates[i].source == DUOPHONIC_ON_OFF_VOICE_1) && (voice == 1)) ||
            ((gates[i].source == DUOPHONIC_ON_OFF_VOICE_2) && (voice == 2))
        )
        {
            if(is_note_on)
            {
                gate_set(i, gates[i].invert ? 0 : 1);
            }
            else // note off
            {
                gate_set(i, gates[i].invert ? 1 : 0);
            }
        }
    }
}

void gates_handle_triphonic_note_on_off(uint8_t channel, uint8_t voice, bool is_note_on)
{
    for(int i=0; i<8; i++)
    {
        if(
            ((gates[i].source == TRIPHONIC_ON_OFF_VOICE_1) && (voice == 1)) ||
            ((gates[i].source == TRIPHONIC_ON_OFF_VOICE_2) && (voice == 2)) ||
            ((gates[i].source == TRIPHONIC_ON_OFF_VOICE_3) && (voice == 3))
        )
        {
            if(is_note_on)
            {
                gate_set(i, gates[i].invert ? 0 : 1);
            }
            else // note off
            {
                gate_set(i, gates[i].invert ? 1 : 0);
            }
        }
    }
}

void cv_1_handle_cc(uint8_t channel, uint8_t cc, uint8_t val)
{
#ifdef MONOPHONIC
    if(
        ((CV_1_SOURCE == CC1) && (CC1_COMMAND == cc)) ||
        ((CV_1_SOURCE == CC2) && (CC2_COMMAND == cc)) ||
        ((CV_1_SOURCE == CC3) && (CC3_COMMAND == cc))
    )
    {
        cc_to_dac(0, val);
    }
#endif
}

void cv_2_handle_cc(uint8_t channel, uint8_t cc, uint8_t val)
{
#ifdef MONOPHONIC
    if(
        ((CV_2_SOURCE == CC1) && (CC1_COMMAND == cc)) ||
        ((CV_2_SOURCE == CC2) && (CC2_COMMAND == cc)) ||
        ((CV_2_SOURCE == CC3) && (CC3_COMMAND == cc))
    )
    {
        cc_to_dac(1, val);
    }
#endif
}

void cv_3_handle_cc(uint8_t channel, uint8_t cc, uint8_t val)
{
#ifndef TRIPHONIC
    if(
        ((CV_3_SOURCE == CC1) && (CC1_COMMAND == cc)) ||
        ((CV_3_SOURCE == CC2) && (CC2_COMMAND == cc)) ||
        ((CV_3_SOURCE == CC3) && (CC3_COMMAND == cc))
    )
    {
        cc_to_dac(2, val);
    }
#endif
}

void gates_handle_cc(uint8_t channel, uint8_t cc, uint8_t val)
{
    for(int i=0; i<8; i++)
    {
        if(
            ((gates[i].source == CC1_HI_LOW) && (CC1_COMMAND == cc)) ||
            ((gates[i].source == CC2_HI_LOW) && (CC2_COMMAND == cc)) ||
            ((gates[i].source == CC3_HI_LOW) && (CC3_COMMAND == cc))
        )
        {
            if(val > 63) // on
            {
                gate_set(i, gates[i].invert ? 0 : 1 );
            }
            else // off
            {
                gate_set(i, gates[i].invert ? 1 : 0 );
            }
        }
    }
}

void gates_handle_clock()
{
    clock_cnt++;
    if(clock_cnt % CLOCK_1_DIVIDER == 0)
    {
        for(int i=0; i<8; i++)
        {
            if(gates[i].source == CLOCK_1)
            {
                trigger_start(i);
            }
        }
    }
    if(clock_cnt % CLOCK_2_DIVIDER == 0)
    {
        for(int i=0; i<8; i++)
        {
            if(gates[i].source == CLOCK_2)
            {
                trigger_start(i);
            }
        }
    }
    if(clock_cnt % CLOCK_3_DIVIDER == 0)
    {
        for(int i=0; i<8; i++)
        {
            if(gates[i].source == CLOCK_3)
            {
                trigger_start(i);
            }
        }
    }
    if(clock_cnt % CLOCK_4_DIVIDER == 0)
    {
        for(int i=0; i<8; i++)
        {
            if(gates[i].source == CLOCK_4)
            {
                trigger_start(i);
            }
        }
    }
    if(clock_cnt % CLOCK_5_DIVIDER == 0)
    {
        for(int i=0; i<8; i++)
        {
            if(gates[i].source == CLOCK_5)
            {
                trigger_start(i);
            }
        }
    }
    if(clock_cnt % CLOCK_6_DIVIDER == 0)
    {
        for(int i=0; i<8; i++)
        {
            if(gates[i].source == CLOCK_6)
            {
                trigger_start(i);
            }
        }
    }
    if(clock_cnt % CLOCK_7_DIVIDER == 0)
    {
        for(int i=0; i<8; i++)
        {
            if(gates[i].source == CLOCK_7)
            {
                trigger_start(i);
            }
        }
    }
    if(clock_cnt % CLOCK_8_DIVIDER == 0)
    {
        for(int i=0; i<8; i++)
        {
            if(gates[i].source == CLOCK_8)
            {
                trigger_start(i);
            }
        }
    }
    unsigned long clock_position = clock_cnt - last_start;
    if(
        playing &&
        (clock_position > 0) && 
        (clock_position % 24 == 0)
    ) // this is a beat and not the first
    {
        beat_cnt++;
        if((RESET_1_BEATS > 0) && (beat_cnt % RESET_1_BEATS == 0))
        {
            for(int i=0; i<8; i++)
            {
                if(gates[i].source == RESET_1)
                {
                    trigger_start(i);
                }
            }
        }
        if((RESET_2_BEATS > 0) && (beat_cnt % RESET_2_BEATS == 0))
        {
            for(int i=0; i<8; i++)
            {
                if(gates[i].source == RESET_2)
                {
                    trigger_start(i);
                }
            }
        }
        if((RESET_3_BEATS > 0) && (beat_cnt % RESET_3_BEATS == 0))
        {
            for(int i=0; i<8; i++)
            {
                if(gates[i].source == RESET_3)
                {
                    trigger_start(i);
                }
            }
        }
    }
}

void gates_handle_transport(uint8_t code)
{
    if(
        (code == MIDI_START) ||
        (code == MIDI_CONTINUE)
    )
    {
        last_start = clock_cnt;
        beat_cnt = 0;
        playing = true;
    }
    else if(code == MIDI_STOP)
    {
        playing = false;
    }
    for(int i=0; i<8; i++)
    {
        if(gates[i].source == TRANSPORT)
        {
            switch(code){
                case MIDI_START:
                case MIDI_CONTINUE:
                    gate_set(i, gates[i].invert ? 0 : 1);
                    break;
                case MIDI_STOP:
                    gate_set(i, gates[i].invert ? 1 : 0);
                    break;
                default: break;
            }
        }
        else if(
            (gates[i].source == RESET_1) ||
            (gates[i].source == RESET_2) ||
            (gates[i].source == RESET_3)
        )
        {
            trigger_start(i);
        }
    }
}

void dacs_handle_pitch_bend(void)
{
    // note_to_dac() will update the pitch
#ifdef MONOPHONIC
    if(cvs[0].is_pitch)
    {
        note_to_dac(0, cvs[0].note, CV_1_SCALE == V_OCT);
    }
    if(cvs[1].is_pitch)
    {
        note_to_dac(1, cvs[1].note, CV_2_SCALE == V_OCT);
    }
    if(cvs[2].is_pitch)
    {
        note_to_dac(2, cvs[2].note, CV_3_SCALE == V_OCT);
    }
#endif
#ifdef DUOPHONIC
    note_to_dac(0, cvs[0].note, CV_1_SCALE == V_OCT);
    note_to_dac(1, cvs[1].note, CV_1_SCALE == V_OCT);
    if(cvs[2].is_pitch)
    {
        note_to_dac(2, cvs[2].note, CV_3_SCALE == V_OCT);
    }
#endif
#ifdef TRIPHONIC
    note_to_dac(0, cvs[0].note, CV_1_SCALE == V_OCT);
    note_to_dac(1, cvs[1].note, CV_1_SCALE == V_OCT);
    note_to_dac(2, cvs[2].note, CV_1_SCALE == V_OCT);
#endif
}

void init_handlers(void)
{
    // gates - - - - - - - - - - - - - - - - - - - - - - - - - - -
    gates[0].source = GATE_1_SOURCE;
    gates[0].note = GATE_1_NOTE;
#ifdef GATE_1_INVERT
    gates[0].invert = 1;
#else
    gates[0].invert = 0;
#endif
    gates[1].source = GATE_2_SOURCE;
    gates[1].note = GATE_2_NOTE;
#ifdef GATE_2_INVERT
    gates[1].invert = 1;
#else
    gates[1].invert = 0;
#endif
    gates[2].source = GATE_3_SOURCE;
    gates[2].note = GATE_3_NOTE;
#ifdef GATE_3_INVERT
    gates[2].invert = 1;
#else
    gates[2].invert = 0;
#endif
    gates[3].source = GATE_4_SOURCE;
    gates[3].note = GATE_4_NOTE;
#ifdef GATE_4_INVERT
    gates[3].invert = 1;
#else
    gates[3].invert = 0;
#endif
    gates[4].source = GATE_5_SOURCE;
    gates[4].note = GATE_5_NOTE;
#ifdef GATE_5_INVERT
    gates[4].invert = 1;
#else
    gates[4].invert = 0;
#endif
    gates[5].source = GATE_6_SOURCE;
    gates[5].note = GATE_6_NOTE;
#ifdef GATE_6_INVERT
    gates[5].invert = 1;
#else
    gates[5].invert = 0;
#endif
    gates[6].source = GATE_7_SOURCE;
    gates[6].note = GATE_7_NOTE;
#ifdef GATE_7_INVERT
    gates[6].invert = 1;
#else
    gates[6].invert = 0;
#endif
    gates[7].source = GATE_8_SOURCE;
    gates[7].note = GATE_8_NOTE;
#ifdef GATE_8_INVERT
    gates[7].invert = 1;
#else
    gates[7].invert = 0;
#endif
    // cvs - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef MONOPHONIC
    cvs[0].is_pitch = (CV_1_SOURCE == NOTE);
    cvs[1].is_pitch = (CV_2_SOURCE == NOTE);
    cvs[2].is_pitch = (CV_3_SOURCE == NOTE);
#endif
#ifdef DUOPHONIC
    cvs[0].is_pitch = true;
    cvs[1].is_pitch = true;
    cvs[2].is_pitch = (CV_3_SOURCE == NOTE);
#endif
#ifdef TRIPHONIC
    cvs[0].is_pitch = true;
    cvs[1].is_pitch = true;
    cvs[2].is_pitch = true;
#endif
}
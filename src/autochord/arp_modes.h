/*
 * Mode representation
 */

#ifndef _ARP_MODES_H
#define _ARP_MODES_H

#include <stdint.h>

#include "chord.h"
#include "scale.h"
/////////////////////////////////////////////////////////////////////////////
// Global definitions
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Global Types
/////////////////////////////////////////////////////////////////////////////
typedef enum key_e {
   KEY_C = 0,
   KEY_CSHARP = 1,
   KEY_D = 2,
   KEY_EFLAT = 3,
   KEY_E = 4,
   KEY_F = 5,
   KEY_FSHARP = 6,
   KEY_G = 7,
   KEY_AFLAT = 8,
   KEY_A = 9,
   KEY_BFLAT = 10,
   KEY_B = 11
} key_t;

typedef enum mode_groups_e {
   MODE_GROUPS_BASE = 0,
   MODE_GROUPS_SEVENTH = 1,
   MODE_GROUPS_NINTH = 2,
   MODE_GROUPS_FIFTH = 3
}  mode_groups_t;
#define NUM_MODE_GROUPS 4


/**
 * Class definition
 */

class ArpModes {
const char * getNoteName(uint8_t note);
const chord_type_t getModeChord(scale_t scale,mode_groups_t modeGroup,uint8_t keySig,uint8_t note);
const char * getChordExtensionText(mode_groups_t extension);

const char * getModeGroupName(mode_groups_t group);

};

#endif 

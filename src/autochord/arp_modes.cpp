/*
 * Arpeggiator modes
 *
 *  Copyright Scott Rush 2024.
 */

 /////////////////////////////////////////////////////////////////////////////
 // Include files
 /////////////////////////////////////////////////////////////////////////////

#include "arp_modes.h"

#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// Local definitions
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Local prototypes
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Local variables
/////////////////////////////////////////////////////////////////////////////

static const char* key_names[] = { "C","C#","D","Eb","E","F","F#","G","Ab","A","Bb","B" };

// Defines the natural chords in the mode by key.
typedef struct mode_chords_s {
   chord_type_t chords[12];
   uint8_t numChords;
   scale_t scale;
} mode_chords_t;
#define NUM_MODES 7

// Table of chords indexed by mode group
static const mode_chords_t mode_group_chord_table[NUM_MODE_GROUPS][NUM_MODES] = {
   // MODE_GROUP_BASE
   {{{CHORD_MAJOR_I, CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJOR_I,  CHORD_MAJOR_I,  CHORD_MINOR_I,  CHORD_MIN7B5}, 7, SCALE_IONIAN},
   {{CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJOR_I,  CHORD_MAJOR_I,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJOR_I}, 7, SCALE_DORIAN},
   {{CHORD_MINOR_I, CHORD_MAJOR_I,  CHORD_MAJOR_I,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJOR_I,  CHORD_MINOR_I, }, 7, SCALE_PHRYGIAN},
   {{CHORD_MAJOR_I,  CHORD_MAJOR_I,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJOR_I,  CHORD_MINOR_I,  CHORD_MINOR_I}, 7, SCALE_LYDIAN},
   {{CHORD_MAJOR_I,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJOR_I,  CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJOR_I}, 7, SCALE_MIXOLYDIAN},
   {{CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJOR_I,  CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJOR_I,  CHORD_MAJOR_I},    7, SCALE_AEOLIAN},
   {{CHORD_MIN7B5,  CHORD_MAJOR_I,  CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJOR_I,  CHORD_MAJOR_I,  CHORD_MINOR_I}, 7, SCALE_LOCRIAN}},
   // MODE_GROUP_SEVENTH
   {{{CHORD_MAJ7, CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJ7,  CHORD_DOM7,  CHORD_MINOR_I,  CHORD_MIN7B5},  7, SCALE_IONIAN},
   {{CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJ7,  CHORD_DOM7,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJ7}, 7, SCALE_DORIAN},
   {{CHORD_MINOR_I, CHORD_MAJ7,  CHORD_MAJ7,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJ7,  CHORD_MINOR_I, }, 7, SCALE_PHRYGIAN},
   {{CHORD_MAJ7,  CHORD_DOM7,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJ7,  CHORD_MINOR_I,  CHORD_MINOR_I}, 7, SCALE_LYDIAN},
   {{CHORD_DOM7,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJ7,  CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJ7}, 7, SCALE_MIXOLYDIAN},
   {{CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJ7,  CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJ7,  CHORD_DOM7}, 7, SCALE_AEOLIAN},
   {{CHORD_MIN7B5,  CHORD_MAJ7,  CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJ7,  CHORD_DOM7,  CHORD_MINOR_I}, 7, SCALE_LOCRIAN}},
   // MODE_GROUP_NINTH
   {{{CHORD_MAJ9, CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJ9,  CHORD_DOM9,  CHORD_MINOR_I,  CHORD_MIN7B5},  7, SCALE_IONIAN},
   {{CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJ9,  CHORD_DOM9,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJ9}, 7, SCALE_DORIAN},
   {{CHORD_MINOR_I, CHORD_MAJ9,  CHORD_MAJ9,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJ9,  CHORD_MINOR_I, }, 7, SCALE_PHRYGIAN},
   {{CHORD_MAJ9,  CHORD_DOM9,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJ9,  CHORD_MINOR_I,  CHORD_MINOR_I}, 7, SCALE_LYDIAN},
   {{CHORD_DOM9,  CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJ9,  CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJ9}, 7, SCALE_MIXOLYDIAN},
   {{CHORD_MINOR_I,  CHORD_MIN7B5,  CHORD_MAJ9,  CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJ9,  CHORD_DOM9}, 7, SCALE_AEOLIAN},
   {{CHORD_MIN7B5,  CHORD_MAJ9,  CHORD_MINOR_I,  CHORD_MINOR_I, CHORD_MAJ9,  CHORD_DOM9,  CHORD_MINOR_I}, 7, SCALE_LOCRIAN}},
   // MODE_GROUP_FIFTH
   {{{CHORD_FIFTH, CHORD_FIFTH,  CHORD_FIFTH, CHORD_FIFTH,  CHORD_FIFTH,  CHORD_FIFTH,  CHORD_MIN7B5},  7, SCALE_IONIAN},
   {{CHORD_FIFTH,  CHORD_FIFTH, CHORD_FIFTH,  CHORD_FIFTH,  CHORD_FIFTH,  CHORD_MIN7B5,  CHORD_FIFTH}, 7, SCALE_DORIAN},
   {{CHORD_FIFTH, CHORD_FIFTH,  CHORD_FIFTH,  CHORD_FIFTH,  CHORD_MIN7B5,  CHORD_FIFTH,  CHORD_FIFTH, }, 7, SCALE_PHRYGIAN},
   {{CHORD_FIFTH,  CHORD_FIFTH,  CHORD_FIFTH,  CHORD_MIN7B5,  CHORD_FIFTH,  CHORD_FIFTH,  CHORD_FIFTH}, 7, SCALE_LYDIAN},
   {{CHORD_FIFTH,  CHORD_FIFTH,  CHORD_MIN7B5,  CHORD_FIFTH,  CHORD_FIFTH,  CHORD_FIFTH, CHORD_FIFTH}, 7, SCALE_MIXOLYDIAN},
   {{CHORD_FIFTH,  CHORD_MIN7B5,  CHORD_FIFTH,  CHORD_FIFTH,  CHORD_FIFTH, CHORD_FIFTH,  CHORD_FIFTH}, 7, SCALE_AEOLIAN},
   {{CHORD_MIN7B5,  CHORD_FIFTH,  CHORD_FIFTH,  CHORD_FIFTH, CHORD_FIFTH,  CHORD_FIFTH,  CHORD_FIFTH}, 7, SCALE_LOCRIAN}}
};


typedef struct mode_group_preset_s {
   mode_extensions_t group;
   const char* presetName;
} mode_group_preset_t;

const mode_group_preset_t mode_group_presets[NUM_MODE_GROUPS] =
{ {MODE_EXTENSION_TRIAD,"Base"},{MODE_EXTENSION_SEVENTH,"Seventh"},{MODE_EXTENSION_NINTH,"Ninth"},{MODE_EXTENSION_FIFTH,"Fifths"} };

/////////////////////////////////////////////////////////////////////////////
// Returns the next name of the mode group
/////////////////////////////////////////////////////////////////////////////
const char* ArpModes::getModeExtensionName(enum mode_extensions_e group) {

   if ((group < 0) > (group >= NUM_MODE_GROUPS)) {
      return "Invalid Mode Extension";
   }
   return (const char*)mode_group_presets[group].presetName;
}

/////////////////////////////////////////////////////////////////////////////
// Returns chord type for mode and note
// scale:  selected scale.
// extFlags:  extension flags
// keySig:  key signature root
// note:  root note of the chord 
/////////////////////////////////////////////////////////////////////////////
const chord_type_t ArpModes::getModeChord(scale_t scale, mode_extensions_t extension, uint8_t keySig, uint8_t note) {
   if (scale > SCALE_MAX_ENUM_VALUE) {
      fprintf(stderr,"ArpModes::getChordType:  Invalid scale=%d", scale);
      return CHORD_ERROR;  // invalid
   }
   if ((extension < 0) || (extension >= NUM_MODE_GROUPS)) {
      return CHORD_ERROR;
   }

   // Get the mode scale from the table or error if not found
   const mode_chords_t* pModeChords = NULL;
   int found = 0;
   for (int modeIndex = 0;modeIndex < NUM_MODES;modeIndex++) {
      pModeChords = &mode_group_chord_table[extension][modeIndex];
      if (pModeChords->scale == scale) {
         found = 1;
         break;
      }
   }

   if (!found) {
      fprintf(stderr,"ArpModes::getChordType:  Scale: %s and mode group %d match not found", SEQ_SCALE_NameGet(scale), extension);
      return CHORD_ERROR;  // invalid      
   }

   // Check if key is part of mode scale.  
   if (!SEQ_SCALE_IsNoteInScale(pModeChords->scale, keySig, note)) {
#ifdef DEBUG
      fprintf(stderr,"ArpModes::getModeChord:  note: %d in key:%d not in scale %s", note, keySig, SEQ_SCALE_NameGet(scale));
#endif
      // Note is not in the scale
      return CHORD_INVALID;
   }
   // return the chord by index within the mode group by pulling the index for the scale from the SEQ_SCALE database.
   uint8_t index = SEQ_SCALE_GetScaleIndex(pModeChords->scale, keySig, note);
   if (index >= pModeChords->numChords){
      return CHORD_ERROR;    // Shouldn't ever happen, but need to check
   }
   else{
      return pModeChords->chords[index];
   }
}

/////////////////////////////////////////////////////////////////////////////
// Utility returns string name for each note.
// input:  notenumber from 0 to 127
/////////////////////////////////////////////////////////////////////////////
const char* ArpModes::getNoteName(uint8_t note) {
   uint8_t key = note % 12;
   return key_names[key];
}

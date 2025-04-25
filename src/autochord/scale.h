/*
 * Header file for scale routines with extensions for M3 SuperPedal by Scott Rush
 *
 * ==========================================================================
 *
 *  Copyright (C) 2008 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

#ifndef _SCALE_H
#define _SCALE_H

#include <stdint.h>

/////////////////////////////////////////////////////////////////////////////
// Global definitions
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Global Types
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Enum of scales added by Scott Rush.
// Enum constants are indexes into seq_scale_table so that they 
// can be used in SEQ_SCALE_NameGet, NoteValueGet, etc.
/////////////////////////////////////////////////////////////////////////////
typedef enum scale_e {
   SCALE_MAJOR = 0,
   SCALE_HARMONIC_MINOR =1,
   SCALE_MELODIC_MINOR = 2,
   SCALE_NATURAL_MINOR = 3,
   SCALE_CHROMATIC = 4,
   SCALE_WHOLE_TONE = 5,
   SCALE_PENT_MAJOR = 6,
   SCALE_PENT_MINOR = 7,
   SCALE_PENT_BLUES =8,
   SCALE_PENT_NEUTRAL = 9,
   SCALE_OCTATONIC_HW = 10,
   SCALE_OCTATONIC_WH = 11,
   SCALE_IONIAN = 12,
   SCALE_DORIAN = 13,
   SCALE_PHRYGIAN = 14,
   SCALE_LYDIAN = 15,
   SCALE_MIXOLYDIAN = 16,
   SCALE_AEOLIAN = 17,
   SCALE_LOCRIAN = 18,
   SCALE_ALGERIAN = 19,
   SCALE_ARABIAN_A = 20,
   SCALE_ARABIAN_B = 21,
   SCALE_AUGMENTED = 22,
   SCALE_AUX_DIM = 23,
   SCALE_AUX_AUG = 24,
   // TODO:  Scale 25 is different with same name as 23.  Figure this out and add
   SCALE_BALINESE = 26,
   SCALE_BLUES = 27,
   SCALE_BYZANTINE = 28,
   SCALE_CHINESE = 29,
   SCALE_CHINESE_MONGOLIAN = 30,
   SCALE_DIATONIC = 31,
   SCALE_DIMINISHED = 32,
   SCALE_DIMINISHED_HALF = 33,
   SCALE_DIMINISHED_WHOLE = 34,
   SCALE_DIMINISHED_WHOLE_TONE = 35,
   SCALE_DOMINANT_7TH = 37,
   SCALE_DOUBLE_HARMONIC = 38,
   SCALE_EGYPTIAN = 39,
   SCALE_EIGHT_TONE_SPANISH = 40,
   // TODO:  Finish all of the others later
   SCALE_INVALID = 0xFF
} scale_t;
#define SCALE_MAX_ENUM_VALUE 40

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

extern uint32_t SEQ_SCALE_Init(uint32_t mode);

extern int32_t SEQ_SCALE_NumGet(void);
extern char * SEQ_SCALE_NameGet(uint8_t scale);

extern int32_t SEQ_SCALE_NoteValueGet(uint8_t note, uint8_t scale, uint8_t root);
extern int32_t SEQ_SCALE_NextNoteInScale(uint8_t current_note, uint8_t scale, uint8_t root);

extern int32_t SEQ_SCALE_GetScaleLength(uint8_t scale);
extern int32_t SEQ_SCALE_GetScaleIndex(uint8_t scale,uint8_t root,uint8_t note);
extern uint8_t SEQ_SCALE_IsNoteInScale(uint8_t scale,uint8_t rootNote,uint8_t note);

/////////////////////////////////////////////////////////////////////////////
// Export global variables
/////////////////////////////////////////////////////////////////////////////

#endif /* _SEQ_SCALE_H */

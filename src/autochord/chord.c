// $Id$
/*
 * Chord Functions copied from MIDIBOX_SEQ_V4 with extensions from Scott Rush
 *
 * ==========================================================================
 *
 *  Copyright (C) 2008 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 *
 * ==========================================================================
 */

 /////////////////////////////////////////////////////////////////////////////
 // Include files
 /////////////////////////////////////////////////////////////////////////////


#include "chord.h"

/////////////////////////////////////////////////////////////////////////////
// Local types
/////////////////////////////////////////////////////////////////////////////

typedef struct {
   s8   keys[6];
   char name[7];
} seq_chord_entry_t;



/////////////////////////////////////////////////////////////////////////////
// Local variables
/////////////////////////////////////////////////////////////////////////////

  // note: chords are used together with the forced-to-scale feature in seq_scale.c
  // if no key should be played, add -1
static const seq_chord_entry_t seq_chord_table[2][32] = {
  {
      // 1   2   3   4   5   6     <----> (6 chars!)
      {{ 0,  4,  7, -1, -1, -1 }, "Maj.I " },
      {{ 4,  7, 12, -1, -1, -1 }, "Maj.II" },
      {{ 7, 12, 16, -1, -1, -1 }, "MajIII" },
      {{ 0, -1, -1, -1, -1, -1 }, "Root  " },
      {{ 4, -1, -1, -1, -1, -1 }, "3rd   " },
      {{ 7, -1, -1, -1, -1, -1 }, "5th   " },
      {{ 0,  4, -1, -1, -1, -1 }, "R.+3rd" },
      {{ 0,  7, -1, -1, -1, -1 }, "R.+5th" },
      {{ 0,  4,  7,  9, -1, -1 }, "Maj.6 " },
      {{ 0,  4,  7, 11, -1, -1 }, "Maj.7 " },
      {{ 0,  4,  7, 12, -1, -1 }, "Maj.8 " },
      {{ 0,  4,  7, 14, -1, -1 }, "Maj.9 " },
      {{ 0,  7, 12, 16, -1, -1 }, "Maj.10" },
      {{ 0,  7, 12, 19, -1, -1 }, "Maj.12" },
      {{ 0,  5,  7, -1, -1, -1 }, "Sus.4 " },
      {{ 0,  4,  8, -1, -1, -1 }, "Maj.+ " },

      {{ 0,  3,  7, -1, -1, -1 }, "Min.I " },
      {{ 3,  7, 12, -1, -1, -1 }, "Min.II" },
      {{ 7, 12, 15, -1, -1, -1 }, "MinIII" },
      {{ 0, -1, -1, -1, -1, -1 }, "Root  " },
      {{ 3, -1, -1, -1, -1, -1 }, "3rdMin" },
      {{ 7, -1, -1, -1, -1, -1 }, "5th   " },
      {{ 0,  3, -1, -1, -1, -1 }, "R.+3rd" },
      {{ 0,  7, -1, -1, -1, -1 }, "R.+5th" },
      {{ 0,  3,  7,  9, -1, -1 }, "Min.6 " },
      {{ 0,  3,  7, 11, -1, -1 }, "Min.7 " },
      {{ 0,  3,  7, 12, -1, -1 }, "Min.8 " },
      {{ 0,  3,  7, 14, -1, -1 }, "Min.9 " },
      {{ 0,  7, 12, 16, -1, -1 }, "Min.10" },
      {{ 0,  7, 12, 18, -1, -1 }, "Min.12" },
      {{ 0,  3,  6,  9, -1, -1 }, "Co7   " },
      {{ 0,  3,  8, -1, -1, -1 }, "Min.+ " }
    }, {
       // see http://midibox.org/forums/topic/19886-extended-chords-dominants-tensions
       // corrected version at http://midibox.org/forums/topic/13137-midibox-seq-v4-release-feedback/?do=findComment&comment=174313
       {{ 0,  7, -1, -1, -1, -1 }, "Pwr5  " },
       {{ 0,  7, 12, -1, -1, -1 }, "Pwr8  " },
       {{ 0,  4, -1, -1, -1, -1 }, "R+mj3 " },
       {{ 0,  3, -1, -1, -1, -1 }, "R+min3" },
       // major chords
       {{ 0,  4,  7, -1, -1, -1 }, "Maj   " },
       {{ 0,  5,  7, -1, -1, -1 }, "Sus4  " },
       {{ 0,  4,  8, -1, -1, -1 }, "Maj+  " },
       {{ 0,  4,  7,  9, -1, -1 }, "Maj6  " },
       {{ 0,  4,  7, 11, -1, -1 }, "Maj7  " },
       {{ 0,  4,  7, 14, -1, -1 }, "add9  " },
       {{ 0,  4,  7, 11, 14, -1 }, "Maj9  " },
       {{ 0,  4,  7, 11, 14, 17 }, "Maj11 " },
       {{ 0,  4,  7, 11, 14, 21 }, "Maj13 " },
       // minor chords
       {{ 0,  3,  7, -1, -1, -1 }, "Min   " },
       {{ 0,  3,  7,  9, -1, -1 }, "Min6  " },
       {{ 0,  3,  7, 10, -1, -1 }, "Min7  " },
       {{ 0,  3,  7, 14, -1, -1 }, "Minad9" },
       {{ 0,  3,  7, 10, 14, -1 }, "Min9  " },
       {{ 0,  3,  7, 10, 14, 17 }, "Min11 " },
       {{ 0,  3,  7, 10, 14, 21 }, "Min13 " },
       // dominant chords
       {{ 0,  4,  7, 10, -1, -1 }, "Dom7  " },
       {{ 0,  5,  7, 10, -1, -1 }, "7Sus4 " },
       {{ 0,  4,  7, 10, 14, -1 }, "Dom9  " },
       {{ 0,  4,  7, 10, 14, 17 }, "Dom11 " },
       {{ 0,  4,  7, 10, 14, 21 }, "Dom13 " },
       // dominant chords with tensions
       {{ 0,  4,  6, 10, -1, -1 }, "7b5   " },
       {{ 0,  4,  8, 10, -1, -1 }, "7#5   " },
       {{ 0,  4,  7, 10, 13, -1 }, "7b9   " },
       {{ 0,  4,  7, 10, 15, -1 }, "7#9   " },
       // diminished
       {{ 0,  3,  6, -1, -1, -1 }, "DimTri" },
       {{ 0,  3,  6,  9, -1, -1 }, "Dim   " },
       // half diminished aka m7b5
       {{ 0,  3,  6, 10, -1, -1 }, "m7b5  " },
     }
};


static const seq_chord_entry_t seq_chord3_table[] = {
   // 1   2   3   4   5   6     <----> (6 chars!)
   {{-1, -1, -1, -1, -1, -1 }, " ---- " },
   {{ 0,  7, -1, -1, -1, -1 }, "pwr5  "},
   {{ 0,  7, 12, -1, -1, -1 }, "pwr8  "},
   {{ 0,  4,  7, -1, -1, -1 }, "Maj   "},
   {{ 4,  7, 12, -1, -1, -1 }, "M1inv "},
   {{ 7, 12, 16, -1, -1, -1 }, "M2inv "},
   {{ 4, 12, 19, -1, -1, -1 }, "drp3  "},
   {{ 7, 12, 16, -1, -1, -1 }, "drp5  "},
   {{ 0,  7, 16, -1, -1, -1 }, "top3  "},
   {{ 0,  4,  7, 12, 16, -1 }, "MgC   "},
   {{ 0,  7, 12, 16, -1, -1 }, "MgD   "},
   {{ 0,  7, 12, 16, 19, 24 }, "MgE   "},
   {{ 0,  4,  7, 12, 19, 24 }, "MgG   "},
   {{ 0,  3,  7, -1, -1, -1 }, "Min   "},
   {{ 3,  7, 12, -1, -1, -1 }, "m1inv "},
   {{ 7, 12, 15, -1, -1, -1 }, "m2inv "},
   {{ 3, 12, 19, -1, -1, -1 }, "mdr3  "},
   {{ 7, 12, 15, -1, -1, -1 }, "mdr5  "},
   {{ 0,  7, 15, -1, -1, -1 }, "mtp3  "},
   {{ 0,  7, 12, 15, 19, 24 }, "mgEm  "},
   {{ 0,  7, 17, 15, 19, -1 }, "mgAm  "},
   {{ 0,  7, 12, 14, -1, -1 }, "mgDm  "},
   {{ 0,  4,  7,  9, -1, -1 }, "M6r   "},
   {{ 4,  7,  9, 12, -1, -1 }, "M61   "},
   {{ 7,  9, 12, 16, -1, -1 }, "M62   "},
   {{ 9, 12, 16, 19, -1, -1 }, "M63   "},
   {{ 7, 16, 21, 24, -1, -1 }, "M6g2  "},
   {{ 7, 12, 16, 21, -1, -1 }, "M6g3  "},
   {{ 0,  9, 28, 31, -1, -1 }, "M6g6  "},
   {{ 0,  3,  7, 10, -1, -1 }, "m7r   "},
   {{ 3,  7, 10, 12, -1, -1 }, "m71   "},
   {{ 7, 10, 12, 15, -1, -1 }, "m72   "},
   {{10, 12, 15, 19, -1, -1 }, "m73   "},
   {{ 0,  7, 10, 15, 22, 24 }, "m7gE  "},
   {{ 0,  7, 10, 15, 19, 24 }, "m2gA  "},
   {{ 0, 10, 15, -1, -1, -1 }, "m7g5  "},
   {{ 0,  3,  7,  9, -1, -1 }, "m6r   "},
   {{ 3,  7,  9, 12, -1, -1 }, "m61   "},
   {{ 7,  9, 12, 15, -1, -1 }, "m62   "},
   {{ 9, 12, 15, 19, -1, -1 }, "m63   "},
   {{ 9, 15, 19, 24, -1, -1 }, "m6g2  "},
   {{ 0,  9, 15, 19, -1, -1 }, "m5g5  "},
   {{ 0,  9, 15, 19, 24, -1 }, "m6g6  "},
   {{ 9, 19, 24, 27, -1, -1 }, "m6g2  "},
   {{ 0,  3,  6, 10, -1, -1 }, "m7b5r "},
   {{ 3,  6, 10, 12, -1, -1 }, "m7b51 "},
   {{ 6, 10, 12, 15, -1, -1 }, "m7b52 "},
   {{10, 12, 15, 18, -1, -1 }, "m7b53 "},
   {{ 0,  7, 10, 12, -1, -1 }, "m7b5g5"},
   {{10, 15, 18, 24, -1, -1 }, "m7b6g3"},
   {{ 0, 10, 15, 18, -1, -1 }, "m7b5g6"},
   {{ 3, 12, 18, 22, -1, -1 }, "m7b5g4"},
   {{ 0,  4,  7, 11, -1, -1 }, "Mj7r  "},
   {{ 4,  7, 11, 12, -1, -1 }, "Mj71  "},
   {{ 7, 11, 12, 16, -1, -1 }, "Mj72  "},
   {{11, 12, 16, 19, -1, -1 }, "Mj73  "},
   {{ 4, 12, 19, 23, -1, -1 }, "Mj7d3 "},
   {{ 7, 12, 16, 23, -1, -1 }, "Mj7d5 "},
   {{ 0,  7, 11, 16, 19, -1 }, "Mj7g5 "},
   {{ 7, 12, 19, 23, 28, 31 }, "Mj7g6 "},
   {{ 0, 11, 16, 19, 24, -1 }, "Mj6g6b"},
   {{ 7, 12, 16, 23, -1, -1 }, "Mj7g3 "},
   {{ 0,  4,  7, 10, -1, -1 }, "7r    "},
   {{ 4,  7, 10, 12, -1, -1 }, "71    "},
   {{ 7, 10, 12, 16, -1, -1 }, "72    "},
   {{10, 12, 16, 19, -1, -1 }, "73    "},
   {{ 0,  4, 10, 12, 16, -1 }, "7gC   "},
   {{ 0,  7, 10, 16, -1, -1 }, "7gD   "},
   {{ 0,  7, 10, 16, 19, 24 }, "7gE   "},
   {{ 0,  4,  7, 12, 16, 22 }, "7gG   "},
   {{ 0,  7, 10, 17, 19, -1 }, "7gA   "},
   {{ 0, 10, 14, -1, -1, -1 }, "7g6   "},
   {{ 0,  4, 10, -1, -1, -1 }, "7g5   "},
   {{ 0,  3,  7, 14, -1, -1 }, "m9n4  "},
   {{ 0,  3,  7, 10, 14, -1 }, "m9n5  "},
   {{ 0,  4,  7, 14, -1, -1 }, "Mj9   "},
   {{ 0,  4,  7, 11, 14, -1 }, "Mj9   "},
   {{ 0,  4,  7, 10, 14, -1 }, "9     "},
   {{ 0,  3,  7, 10, 14, 17 }, "m11   "},
   {{ 0,  4,  7, 10, 14, 17 }, "11    "},
   {{ 0,  4,  7, 11, 14, 21 }, "Mj13  "},
   {{ 0,  3,  7, 10, 14, 21 }, "m13   "},
   {{ 0,  4,  7, 10, 14, 21 }, "13    "},
   {{ 0,  5, 10, -1, -1, -1 }, "ply4  "},
   {{ 0,  7, 14, -1, -1, -1 }, "ply5  "},
   {{ 0,  3,  6,  9, -1, -1 }, "dim   "},
   {{ 0,  3,  6, -1, -1, -1 }, "dm3   "},
   {{ 0,  5,  7, -1, -1, -1 }, "sus4  "},
   {{ 0,  5,  7, 10, -1, -1 }, "7sus  "},
   {{ 0,  4,  8, -1, -1, -1 }, "+     "},
   {{ 0,  4,  8, 10, -1, -1 }, "7+    "},
   {{ 0,  3,  7, 11, -1, -1 }, "m(Mj7)"},
   {{ 0,  4,  6, 10, -1, -1 }, "7b5   "},
   {{ 0,  4,  8, 10, -1, -1 }, "7#5   "},
   {{ 0,  4,  7, 10, 13, -1 }, "7b9   "},
   {{ 0,  4, 10, 13, -1, -1 }, "7b9   "},
   {{ 0,  4,  7, 10, 15, -1 }, "7#9   "},
   {{ 0,  4, 10, 15, -1, -1 }, "7#9   "},
   {{ 0,  4,  6, 13, -1, -1 }, "7b5b9 "},
   {{ 0,  4,  8, 13, -1, -1 }, "7#5b9 "},
   {{ 0,  4,  6, 15, -1, -1 }, "7b5#9 "},
   {{ 0,  4,  8, 15, -1, -1 }, "7b5b9 "}, // C101
   // add additional chords here... (26 free slots)
};


/////////////////////////////////////////////////////////////////////////////
// Initialisation
/////////////////////////////////////////////////////////////////////////////
s32 SEQ_CHORD_Init(u32 mode)
{
   // here we could also generate the chord table in RAM...

   return 0; // no error
}


/////////////////////////////////////////////////////////////////////////////
// returns number of available chords
/////////////////////////////////////////////////////////////////////////////
s32 SEQ_CHORD_NumGet(u8 chord_set)
{
   if (chord_set == 2) {
      return sizeof(seq_chord3_table) / sizeof(seq_chord_entry_t);
   }
   else {
      return sizeof(seq_chord_table[0]) / sizeof(seq_chord_entry_t);
   }
}


/////////////////////////////////////////////////////////////////////////////
// returns pointer to the name of a chord
// Length: 6 characters + zero terminator
/////////////////////////////////////////////////////////////////////////////
char* SEQ_CHORD_NameGet(u8 chord_set, u8 chord_ix)
{
   if (chord_ix == 0 || chord_ix >= SEQ_CHORD_NumGet(chord_set))
      return "------";

   if (chord_set == 2) {
      return (char*)seq_chord3_table[chord_ix].name;
   }
   else {
      return (char*)seq_chord_table[chord_set][chord_ix].name;
   }
}


/////////////////////////////////////////////////////////////////////////////
// This function returns the transpose value of a chord with the given
// key number (0-3)
// IN: key number (0-5)
//     chord number (bit 4:0) and octave transpose value (7:5)
// returns note number if >= 0
// returns < 0 if no note defined for the given key
/////////////////////////////////////////////////////////////////////////////
s32 SEQ_CHORD_NoteGet(u8 key_num, u8 chord_set, u8 chord)
{
   if (key_num >= 6)
      return -2; // key number too high

   if (chord_set >= 3)
      return -3; // invalid chord set

   s32 note = 0;
   s32 oct_transpose = 0;

   if (chord_set == 2) {
      note = (s32)seq_chord3_table[chord].keys[key_num];
   }
   else {
      u8 chord_ix = chord & 0x1f;
      oct_transpose = (chord >> 5) - 2;

      note = (s32)seq_chord_table[chord_set][chord_ix].keys[key_num];
   }

   if (note < 0)
      return note;

   // add C-2
   note += 0x30;

   // transpose octave
   note += oct_transpose * 12;

   // ensure that note is in the 0..127 range
   note = SEQ_CORE_TrimNote(note, 0, 127);

   return note;
}

/////////////////////////////////////////////////////////////////////////////
// Added by Scott Rush to get chord by chord_type enum instead of chord number
// key_num:  key number (0..5) of the chord
// chord:  enum for the cord
// oct_transpose:  octave transpose from (0 corresponds to MIDI Note# 24=C0)
/////////////////////////////////////////////////////////////////////////////
s32 SEQ_CHORD_NoteGetByEnum(u8 key_num, chord_type_t chord, s8 oct_transpose) {
   if (key_num >= 6)
      return -2; // key number too high
   s32 note = 0;
   u8 index = chord;
   u8 chordset = 0;
   if (chord >= CHORD_SET2_ENUM_OFFSET) {
      // This is in chord set 2
      index -= CHORD_SET2_ENUM_OFFSET;
      chordset = 1;
   }
   note = seq_chord_table[chordset][index].keys[key_num];
   if (note < 0)
      return note;  // Returns a -1 if the key isn't part of this chord

   // add C-2
   note += 0x30;

   // transpose octave
   note += oct_transpose * 12;

   // ensure that note is in the 0..127 range
   note = SEQ_CORE_TrimNote(note, 0, 127);

   return note;
}

/////////////////////////////////////////////////////////////////////////////
// Added by Scott Rush to get chord name by chord_type enum instead of chord number
// chord:  enum for the cord

/////////////////////////////////////////////////////////////////////////////
const char* SEQ_CHORD_NameGetByEnum(chord_type_t chord) {
   u8 index = chord;
   u8 chordset = 0;
   if (chord >= CHORD_SET2_ENUM_OFFSET) {
      // This is in chord set 2
      index -= CHORD_SET2_ENUM_OFFSET;
      chordset = 1;
   }
   return seq_chord_table[chordset][index].name;
}

/////////////////////////////////////////////////////////////////////////////
// Added by Scott Rush to get the number of notes in the chord
/////////////////////////////////////////////////////////////////////////////
u8 SEQ_CHORD_GetNumNotesByEnum(chord_type_t chord) {
   for(u8 num= 0;num < 6;num++){
      s32 note = SEQ_CHORD_NoteGetByEnum(num,chord,0);
      if (note < 0){
         return num;
      }
   }
   return 0;  // Can't happen for valid chord
}
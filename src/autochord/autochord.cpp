#include "autochord.h"

static scale_t SCALE_INDEX_MAP[] = {SCALE_IONIAN,SCALE_DORIAN,SCALE_PHRYGIAN,SCALE_LYDIAN,SCALE_MIXOLYDIAN,SCALE_AEOLIAN,SCALE_LOCRIAN};

AutoChord::AutoChord(){

}

/////////////////////////////////////////////////////////////////////////////
// This function ensures, that a (transposed) note is within
// the <lower>..<upper> range.
//
// If the note is outside the range, it will be "trimmed" in the semitone
// range, and the octave will be kept.
/////////////////////////////////////////////////////////////////////////////
uint8_t AutoChord::trimNote(uint8_t note, uint8_t lower, uint8_t upper)
{
  // negative note (e.g. after transpose?)
  // shift it to the positive range
  if( note < 0 )
    note = 11 - ((-note - 1) % 12);

  // check for lower boundary
  if( note < (int32_t)lower ) {
    note = 12*(lower/12) + (note % 12);
  }

  // check for upper boundary
  if( note > (int32_t)upper ) {
    note = 12*(upper/12) + (note % 12);

    // if note still > upper value (e.g. upper is set to >= 120)
    // an if (instead of while) should work in all cases, because note will be (12*int(127/12)) + 11 = 131 in worst case!
    if( note > upper )
      note -= 12;
  }

  return note;
}
 /////////////////////////////////////////////////////////////////////////////
// Fills the provided array with the notes of the code based on the supplied root
// and mode.
// returns 1 if valid chord play, 0 if not a valid root for current mode/scale
// TODO:  Redo this function with a new note model
/////////////////////////////////////////////////////////////////////////////
unsigned int AutoChord::fillChordNotestack(void* pNotestack, uint8_t rootNote, uint8_t velocity,__mode_t) {
    // Fill the note stack with the the keys of the chord as k1...k#notes to the stack.  
      //  This is the same as pressing all the keys.  The stack will be processed at the next
      // synch point.
    persisted_arp_data_t* pArpSettings = ARP_GetARPSettings();
    const chord_type_t chord = ARP_MODES_GetModeChord(pArpSettings->modeScale,
       pArpSettings->modeGroup, pArpSettings->rootKey, rootNote);
    // Compute octave by subtracting C-2 (note 24)
    s8 octave = ((rootNote - 24) / 12) - 2;
 
    // Now put the notes of the chord into the pattern buffer at the chordNoteVelocity
    uint8_t numChordNotes = SEQ_CHORD_GetNumNotesByEnum(chord);
 #ifdef DEBUG
    DEBUG_MSG("ARP_FillChordPadNoteStack: Pushing chord: %s, chordPlayedNote=%d octave=%d numChordNotes=%d",
       SEQ_CHORD_NameGetByEnum(chord), rootNote, octave, numChordNotes);
 #endif   
    for (uint8_t keyNum = 0;keyNum < numChordNotes;keyNum++) {
       int32_t chordNote = SEQ_CHORD_NoteGetByEnum(keyNum, chord, octave);
       // Add offset from the root note
       chordNote += (rootNote % 12);
 
       NOTESTACK_Push(pNotestack, chordNote, velocity);
    }
 
    // if the push was a different note that exceeded the max number of keys, then pop it and return 
    // it was pulled from notestack in error
    if (NOTESTACK_CountActiveNotes(&notestack) > MAX_NUM_KEYS) {
       NOTESTACK_Pop(pNotestack, rootNote);
    }
 #ifdef DEBUG
    DEBUG_MSG("ARP_PAT_FillChordNotestack");
    NOTESTACK_SendDebugMessage(pNotestack);
 #endif
    return (pNotestack->len > 0);
 }


scale_t AutoChord::getScale(uint16_t scaleIndex){
    if ((scaleIndex < 0)||(scaleIndex >= SCALE_INDEX_MAP_LENGTH){
        return SCALE_INVALID;
    }
    return SCALE_INDEX_MAP[scaleIndex];
}

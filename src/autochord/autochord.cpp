/**
 * AutoChord implementatoin file
 */

#include "autochord.h"

static scale_t SCALE_INDEX_MAP[] = { SCALE_IONIAN, SCALE_DORIAN, SCALE_PHRYGIAN, SCALE_LYDIAN, SCALE_MIXOLYDIAN, SCALE_AEOLIAN, SCALE_LOCRIAN };

static const char* SCALE_NAMES[] = { "Ionian", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Aeolian", "Locrian" };

// Declaration of singleton instance variables
AutoChord* AutoChord::instance = nullptr;
std::mutex AutoChord::mutex;

AutoChord::AutoChord() {
   m_state = AUTOCHORD_OFF;
   m_keySignature = KEY_C;
   m_extension = MODE_EXTENSION_NONE;
   m_lv2scale = AUTOCHORD_LV2_SCALE_IONIAN;
}
void AutoChord::setState(autochord_state_t state) {
   m_state = state;
}
autochord_state_t AutoChord::getState() {
   return m_state;
}

void AutoChord::setExtension(mode_extensions_t ext) {
   m_extension = ext;
}
mode_extensions_t AutoChord::getExtension() {
   return m_extension;
}

void AutoChord::setScale(autochord_lv2_scale_t scale) {
   m_lv2scale = scale;
}
autochord_lv2_scale_t AutoChord::getScale() {
   return m_lv2scale;
}

void AutoChord::setKeySignature(key_signature_t key) {
   m_keySignature = key;
}
key_signature_t AutoChord::getKeySignature() {
   return m_keySignature;
}

uint8_t AutoChord::trimNote(uint8_t note, uint8_t lower, uint8_t upper) {
   // negative note (e.g. after transpose?)
   // shift it to the positive range
   if (note < 0)
      note = 11 - ((-note - 1) % 12);

   // check for lower boundary
   if (note < (int32_t)lower) {
      note = 12 * (lower / 12) + (note % 12);
   }

   // check for upper boundary
   if (note > (int32_t)upper) {
      note = 12 * (upper / 12) + (note % 12);

      // if note still > upper value (e.g. upper is set to >= 120)
      // an if (instead of while) should work in all cases, because note will be (12*int(127/12)) + 11 = 131 in worst case!
      if (note > upper)
         note -= 12;
   }

   return note;
}

const autochord_notes_t* AutoChord::getChordNotes(uint8_t rootNote, uint8_t velocity) {
   // Lookup the chord
   const chord_type_t chord = ArpModes::getInstance()->getModeChord(
      SCALE_INDEX_MAP[(uint8_t)m_lv2scale], m_extension, m_keySignature, rootNote);

   if (chord == CHORD_ERROR) {
      // invalid configuration.  this is an error. Set the number of notes to zero and return
      chordNotes.numNotes = 0;
      return &chordNotes;
   }

   // Compute octave by subtracting C-2 (note 24)
   int8_t octave = ((rootNote - 24) / 12) - 2;

   // Now put the notes of the chord into the pattern buffer at the chordNoteVelocity
   uint8_t numChordNotes = Chord::getInstance()->getNumNotesByEnum(chord);
#ifdef DEBUG
   DEBUG_MSG("ARP_FillChordPadNoteStack: Pushing chord: %s, chordPlayedNote=%d octave=%d numChordNotes=%d",
      Chord::getInstance()->NameGetByEnum(chord), rootNote, octave, numChordNotes);
#endif
   // transfer the notes to the buffer
   chordNotes.numNotes = numChordNotes;
   for (uint8_t keyNum = 0; keyNum < numChordNotes; keyNum++) {
      int32_t chordNote = Chord::getInstance()->noteGetByEnum(keyNum, chord, octave);
      // Add offset from the root note
      chordNote += (rootNote % 12);
      // And put in the buffer at the index
      chordNotes.notes[keyNum] = chordNote;
      chordNotes.velocities[keyNum] = velocity;
   }
   return &chordNotes;
}


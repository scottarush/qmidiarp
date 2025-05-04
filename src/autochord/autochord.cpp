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
   m_state = AUTOCHORD_PAD;
   m_keySignature = KEY_A;
   m_extension = MODE_EXTENSION_NONE;
   m_lv2scale = AUTOCHORD_LV2_SCALE_AEOLIAN;

   m_activeNote = 0;
   m_activeNoteVelocity = 0;

   m_chordNotes.numNotes = 0;   
   m_padOffNotes.numNotes = 0;
   m_padOnPending = false;

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
    
void AutoChord::notePressed(uint8_t note,uint8_t velocity){
   if (note == m_activeNote)
      return;  // shouldn't happen
   if (m_activeNote > 0){
      // This is a double-press.  force a release of the currently active notes
      // by copying into the m_padOffNotes buffer.
      uint16_t offset = m_padOffNotes.numNotes;
      for(uint8_t i=0;i < m_chordNotes.numNotes;i++){
         m_padOffNotes.notes[offset+i] = m_chordNotes.notes[i];
         m_padOffNotes.velocities[offset+i] = m_padOffNotes.velocities[i];
      }         
   }
   // Now transfer the active note and mark transmit as pending
   m_activeNote = note;
   m_activeNoteVelocity = velocity;
   updateChordNotes();
   m_padOnPending = true;
}

void AutoChord::noteReleased(uint8_t note,uint8_t velocity){
   m_activeNote = 0;
   // Add notes to be released to the pending buffer
   uint16_t offset = m_padOffNotes.numNotes;
   m_padOffNotes.numNotes = m_chordNotes.numNotes;
   for(uint8_t i=0;i < m_chordNotes.numNotes;i++){
      m_padOffNotes.notes[offset+i] = m_chordNotes.notes[i];
      m_padOffNotes.velocities[offset+i] = m_padOffNotes.velocities[i];
   }
}

void AutoChord::clearPadNoteOnPending(){
   m_padOnPending = false;
}
bool AutoChord::padNoteOnPending(){
   return m_padOnPending;
}

void AutoChord::clearPadNoteOffPending(){
   m_padOffNotes.numNotes = 0;
}
bool AutoChord::padNoteOffPending(){
   return m_padOffNotes.numNotes > 0;
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

const autochord_notes_t* AutoChord::getChordNotes() {
   return &m_chordNotes;
}
const autochord_notes_t* AutoChord::getPadNoteOffs() {
   return &m_padOffNotes;
}

void AutoChord::updateChordNotes(){
   if (m_activeNote == 0){
      m_chordNotes.numNotes = 0;
      return;
   }
   // Lookup the chord
   const chord_type_t chord = ArpModes::getInstance()->getModeChord(
      SCALE_INDEX_MAP[(uint8_t)m_lv2scale], m_extension, m_keySignature, m_activeNote);

   switch (chord) {
   case CHORD_ERROR:
      // invalid configuration.  this is an error. Set the number of notes to zero and return
      m_chordNotes.numNotes = 0;
      return;
   case CHORD_INVALID:
      // note is not in chord.  Just return a single note
      m_chordNotes.numNotes = 1;
      m_chordNotes.notes[0] = m_activeNote;
      m_chordNotes.velocities[0] = m_activeNoteVelocity;
      return;
      // drop through for valid chord
   }


   // Compute octave by subtracting C-2 (note 24)
   int8_t octave = ((m_activeNote - 24) / 12) - 2;

   // Now put the notes of the chord into the pattern buffer at the chordNoteVelocity
   uint8_t numChordNotes = Chord::getInstance()->getNumNotesByEnum(chord);
#ifdef DEBUG
   DEBUG_MSG("ARP_FillChordPadNoteStack: Pushing chord: %s, chordPlayedNote=%d octave=%d numChordNotes=%d",
      Chord::getInstance()->NameGetByEnum(chord), rootNote, octave, numChordNotes);
#endif
   // transfer the notes to the buffer
   m_chordNotes.numNotes = numChordNotes;
   for (uint8_t keyNum = 0; keyNum < numChordNotes; keyNum++) {
      int32_t chordNote = Chord::getInstance()->noteGetByEnum(keyNum, chord, octave);
      // Add offset from the root note
      chordNote += (m_activeNote % 12);
      // And put in the buffer at the index
      m_chordNotes.notes[keyNum] = chordNote;
      m_chordNotes.velocities[keyNum] = m_activeNoteVelocity;
   }
   return;
}

void AutoChord::copyNotes(const autochord_notes_t* srcPtr,autochord_notes_t* destPtr){
   destPtr->numNotes = srcPtr->numNotes;
     for(int i=0;i < srcPtr->numNotes;i++){
        destPtr->notes[i] = srcPtr->notes[i];
        destPtr->velocities[i] = destPtr->velocities[i];
     }

}
/*
* AutoChord class is a singleton class that is the interface to/from
* the autochord classes and the QMidiArp application itself.
*/
#ifndef _AUTOCHORD_H
#define _AUTOCHORD_H

// #include "lv2/lv2plug.in/ns/ext/log/log.h"
#include <stdio.h>
#include <mutex>
#include <cstdint>
using namespace std;

#include "scale.h"
#include "arp_modes.h"

#define MAX_CHORD_NOTES 6

// Debug logging macros
#define DEBUG_MSG printf
// #define DEBUG_MSG

typedef enum autochord_state_e {
    AUTOCHORD_OFF = 0,
    AUTOCHORD_PAD = 1,
    AUTOCHORD_ARP = 2
} autochord_state_t;

typedef struct autochord_notes_s {
    uint8_t numNotes;
    uint8_t notes[MAX_CHORD_NOTES];
    uint8_t velocities[MAX_CHORD_NOTES];
} autochord_notes_t;

/**
 * Singleton class definition
 */
class AutoChord {
public:
    static AutoChord* getInstance() {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr) {
            instance = new AutoChord();
        }
        return instance;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Utility function ensures, that a (transposed) note is within
    // the <lower>..<upper> range.
    //
    // If the note is outside the range, it will be "trimmed" in the semitone
    // range, and the octave will be kept.
    /////////////////////////////////////////////////////////////////////////////
    uint8_t trimNote(uint8_t note, uint8_t lower, uint8_t upper);

    void setState(autochord_state_t state);
    autochord_state_t getState();

    void setExtension(mode_extensions_t ext);
    mode_extensions_t getExtension();

    void setScaleIndex(uint8_t index);
    uint8_t getScaleIndex();

    void setKeySignature(key_signature_t key);
    key_signature_t getKeySignature();

    /**
     * Called to get the notes of a chord for a pressed root
     */
    const autochord_notes_t* getChordNotes(uint8_t root, uint8_t velocity);

private:
    mode_extensions_t m_extension;
    autochord_state_t m_state;
    int m_scaleIndex;
    key_signature_t m_keySignature;

    // Singleton members and private constructor
    static AutoChord* instance;
    static std::mutex mutex;
    AutoChord();

    // internal buffer of notes
    autochord_notes_t chordNotes;
};

#endif
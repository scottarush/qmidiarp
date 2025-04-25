/*
includes specific to lv2 plugin use
*/
#ifndef _AUTOCHORD_H
#define _AUTOCHORD_H


//#include "lv2/lv2plug.in/ns/ext/log/log.h"
#include <stdio.h>
#include <mutex>
using namespace std;

#include "scale.h"

// Debug logging macros
#define DEBUG_MSG printf
//#define DEBUG_MSG

typedef enum autochord_state_e {
    AUTOCHORD_OFF = 0,
    AUTOCHORD_PAD = 1,
    AUTOCHORD_ARP = 2
} autochord_state_t;

typedef enum autochord_extensions_e {
    AUTOCHORD_EXT_NONE = 0,
    AUTOCHORD_EXT_SEVENTHS = 1,
    AUTOCHORD_EXT_NINTHS = 2,
    AUTOCHORD_EXT_FIFTHS = 3
} autochord_extensions_t;


/**
 * Singleton class definition
 */
class AutoChord {

    public:

    uint8_t trimNote(uint8_t note, uint8_t lower, uint8_t upper);
    uint32_t fillChordNotestack(void* pNotestack, uint8_t rootNote, uint8_t velocity,__mode_t);

    void setAutoChordState(autochord_state_t state);
    autochord_state_t getAutoChordState();

    void setAutoChordExtension(autochord_extensions_t ext);
    autochord_extensions_t getAutoChordExtension();

    void setAutoChordScale(scale_t scale);
    scale_t getAutoChordScale();

    void setAutoChordKeySig(key_t key);
    
    private:
    // Singleton members and private constructor

    static AutoChord * instance;
    static std::mutex mutex;
    AutoChord();

    public:
    static AutoChord* getInstance() {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr) {
            instance = new AutoChord();
        }
        return instance;
    }

};

#endif
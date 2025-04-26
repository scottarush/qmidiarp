/*
 * Mode representation
 */

#ifndef _ARP_MODES_H
#define _ARP_MODES_H

#include <mutex>
#include <cstdint>

#include "chord.h"
#include "scale.h"
/////////////////////////////////////////////////////////////////////////////
// Global definitions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Global Types
/////////////////////////////////////////////////////////////////////////////
typedef enum key_signature_e
{
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
} key_signature_t;

typedef enum mode_extensions_e
{
   MODE_EXTENSION_TRIAD = 0,
   MODE_EXTENSION_SEVENTH = 1,
   MODE_EXTENSION_NINTH = 2,
   MODE_EXTENSION_FIFTH = 3
} mode_extensions_t;
#define NUM_MODE_GROUPS 4

/**
 * Class definition
 */

class ArpModes
{
public:
   static ArpModes *getInstance()
   {
      std::lock_guard<std::mutex> lock(mutex);
      if (instance == nullptr)
      {
         instance = new ArpModes();
      }
      return instance;
   }

   const char *getNoteName(uint8_t note);
   const chord_type_t getModeChord(scale_t scale, mode_extensions_t modeGroup, uint8_t keySig, uint8_t note);
   const char *getChordExtensionText(mode_extensions_t extension);

   const char *getModeExtensionName(mode_extensions_t group);

private:
   // Singleton members and private constructor
   static ArpModes *instance;
   static std::mutex mutex;
   ArpModes();
};

#endif

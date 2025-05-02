// $Id$
/*
 * Header file for chord routines
 *
 * ==========================================================================
 *
 *  Copyright (C) 2008 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 *
 * ==========================================================================
 */

#ifndef _CHORD_H
#define _CHORD_H
#include <cstdint>
#include <mutex>
 /*
*  The enum values are indexes into therows of the chord_table within chorp.cpp,
 */
typedef enum chord_type_e
{
   CHORD_MAJOR_I = 0,
   CHORD_MAJOR_II = 1,
   CHORD_MAJOR_III = 2,
   CHORD_ROOT = 3,
   CHORD_THIRD = 4,
   CHORD_FIFTH = 5,
   CHORD_ROOT_THIRD = 6,
   CHORD_ROOT_FIFTH = 7,
   CHORD_MAJ6 = 8,
   CHORD_MAJ7 = 9,
   CHORD_MAJ8 = 10,
   CHORD_ADD9 = 11,
   CHORD_MAJ9 = 12,
   CHORD_MAJADD10 = 13,
   CHORD_MAJ11 = 14,
   CHORD_MAJADD12 = 15,
   CHORD_MAJ13 = 16,
   CHORD_SUS4 = 17,
   CHORD_MAJ_PLUS = 18,
   CHORD_MINOR_I = 19,
   CHORD_MINOR_II = 20,
   CHORD_MINOR_III = 21,
   CHORD_MIN6 = 22,
   CHORD_MIN7 = 23,
   CHORD_MIN8 = 24,
   CHORD_MINADD9 = 25,
   CHORD_MIN9 = 26,
   CHORD_MIN10 = 27,
   CHORD_MIN11 = 28,
   CHORD_MINADD12 = 29,
   CHORD_MIN13 = 30,
   CHORD_CO7 = 31,
   CHORD_MINOR_PLUS = 32,
   CHORD_POWER5 = 33,
   CHORD_POWER8 =  34,
   CHORD_DOM7 =  37,
   CHORD_DOM7SUS4 =  38,
   CHORD_DOM9 = 39,
   CHORD_DOM11 = 40,
   CHORD_DOM13 =  41,
   CHORD_DOM7B5 =  42,
   CHORD_DOM7_SHARP5 =  43,
   CHORD_DOM7B9 = 44,
   CHORD_DOM7_SHARP9 =  45,
   CHORD_DIM3 =  46,
   CHORD_DIM =  47,
   CHORD_MIN7B5 =  48,
   CHORD_INVALID = 0xFE,
   CHORD_ERROR = 0xFF
} chord_type_t;

#define MAX_CHORD_TYPE_ENUM 48

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

/**
 * Singleton class definition
 */
class Chord {
public:
   static Chord* getInstance()
   {
      std::lock_guard<std::mutex> lock(mutex);
      if (instance == nullptr)
      {
         instance = new Chord();
      }
      return instance;
   }

   int32_t numGet();
   char* nameGet( chord_type_t chord);
   int32_t noteGet(uint8_t key_num, chord_type_t chord,uint8_t oct_transpose);

   int32_t noteGetByEnum(uint8_t key_num, chord_type_t chord, int8_t oct_transpose);
   const char* nameGetByEnum(chord_type_t chord);
   uint8_t getNumNotesByEnum(chord_type_t chord);

private:
   // Singleton members and private constructor
   static Chord* instance;
   static std::mutex mutex;
   Chord();
};
#endif /* _H */

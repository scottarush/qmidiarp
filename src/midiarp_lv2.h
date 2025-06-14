/*!
 * @file midiarp_lv2.h
 * @brief Headers for the MidiArp LV2 plugin class
 *
 *
 *      Copyright 2009 - 2024 <qmidiarp-devel@lists.sourceforge.net>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 *
 */

#ifndef QMIDIARP_ARP_LV2_H
#define QMIDIARP_ARP_LV2_H

#include "midiarp.h"
#include "lv2_common.h"

#include "autochord/autochord.h"

#define QMIDIARP_ARP_LV2_URI QMIDIARP_LV2_URI "/arp"
#define QMIDIARP_ARP_LV2_PREFIX QMIDIARP_ARP_LV2_URI "#"


class MidiArpLV2 : public MidiArp {
public:

        MidiArpLV2(double sample_rate, const LV2_Feature* const* host_features);

        ~MidiArpLV2();

        enum FloatField {
                TRANSPORT_MODE = 0,
                AUTOCHORD_STATE,
                AUTOCHORD_KEY_SIG,
                AUTOCHORD_SCALE,
                AUTOCHORD_EXTENSIONS,
                ATTACK,
                RELEASE,
                RANDOM_TICK,
                RANDOM_LEN,
                RANDOM_VEL,
                CH_OUT,
                CH_IN,
                CURSOR_POS, //output1
                ENABLE_RESTARTBYKBD,
                ENABLE_TRIGBYKBD,
                MUTE,
                LATCH_MODE,
                OCTAVE_MODE,
                OCTAVE_LOW,
                OCTAVE_HIGH,
                INDEX_IN1,
                INDEX_IN2,
                RANGE_IN1,
                RANGE_IN2,
                ENABLE_TRIGLEGATO,
                REPEAT_MODE,
                RPATTERNFLAG,
                DEFER,
                PATTERN_PRESET,
                TEMPO,
                HOST_TEMPO,
                HOST_POSITION,
                HOST_SPEED
        };

        void connect_port(uint32_t port, void* data);
        void run(uint32_t nframes);
        void activate();
        void deactivate();
        void updatePosAtom(const LV2_Atom_Object* obj);
        void updatePos(uint64_t position, float bpm, int speed, bool ignore_pos = false);
        void initTransport();
        LV2_URID_Map* uridMap;
        QMidiArpURIs m_uris;
        LV2_Atom_Forge forge;
        LV2_Atom_Forge_Frame m_frame;

        bool sendPatternFlag;

private:

        float* val[33];
        uint64_t curFrame;
        uint64_t tempoChangeTick;
        uint64_t trStartingTick;
        int curTick;
        double internalTempo;
        double sampleRate;
        double tempo;

        bool ui_up;
        bool transportAtomReceived;
        void updateParams();
        void sendPattern(const std::string& p);
        void forgeMidiEvent(uint32_t f, const uint8_t* const buffer, uint32_t size);

        uint64_t transportFramesDelta;  /**< Frames since last click start */
        float transportBpm;
        float transportSpeed;
        bool hostTransport;
        uint32_t evQueue[JQ_BUFSZ];
        uint32_t evTickQueue[JQ_BUFSZ];
        int bufPtr;

        LV2_Atom_Sequence* inEventBuffer;
        const LV2_Atom_Sequence* outEventBuffer;
};

#endif

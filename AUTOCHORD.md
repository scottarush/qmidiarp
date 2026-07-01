# AutoChord for QMidiArp

AutoChord is a powerful harmonizer and chord generation engine built directly into QMidiArp. It enables users to trigger full diatonic chords and complex arpeggiations using single-note MIDI inputs, strictly adhering to a user-defined musical scale and key signature.

## Key Features

- **Diatonic Harmonization**: Automatically generates the correct Major, Minor, or Diminished chord based on the input note's position within the selected scale.
- **Scales and Modes**: Supports all fundamental modes (Ionian, Dorian, Phrygian, Lydian, Mixolydian, Aeolian, Locrian) across all 12 key signatures.
- **Chord Extensions**: Supports generating extended chords (e.g., 7ths, 9ths) to add color to the harmony.
- **Dual Output Modes**:
  - **PAD Mode**: Outputs the generated chord as a polyphonic pad. Perfect for laying down lush backing chords with a single finger.
  - **ARP Mode**: Injects the generated chord into QMidiArp's arpeggiator engine, transforming the chord into rhythmic arpeggiated sequences.
- **Note Extend Time (Gated Tail)**: A specialized parameter (range 0 to 32, representing 1/32 note units) designed to work exclusively with the **Drum Gate**. All standard arpeggiator notes play strictly at their native preset duration, ensuring tight, clean sequences. However, if the Drum Gate closes and silences the arpeggiator, this feature kicks in to automatically extend the *final note before the gate closes*. This allows the last note in your phrase (e.g., a final bass pluck) to hang on and ring out beautifully across the silenced gap without turning the rest of your arpeggiator runs into mud.

## Operating Modes

AutoChord can be toggled through three states via the LV2 plugin UI:

1. **OFF (Normal)**
   - QMidiArp behaves normally. Single notes are arpeggiated as single notes, and physical chords are arpeggiated normally.
2. **PAD (Polyphonic)**
   - Single-note inputs are translated into full chords.
   - QMidiArp's internal arpeggiator engine is bypassed. 
   - Note-On and Note-Off events for the entire chord are sent directly to the output.
   - **Double-Press Logic**: If a new note is pressed while a previous note is held, AutoChord automatically sends Note-Offs for the previous chord and seamlessly transitions to the new chord, preventing muddy overlaps and ghost notes.
3. **ARP (Arpeggiated)**
   - Single-note inputs are translated into full chords.
   - The generated chord notes are buffered into QMidiArp's core engine, treating the single input as if the user physically played the full chord on a keyboard.
   - **Forced Arpeggiation**: AutoChord overrides QMidiArp's `Repeat Pattern Through Chord` parameter (forcing it to `1`) to guarantee that the engine sequentially steps through the generated chord notes (`0, 1, 2, 0, 1, 2`), even if a simple single-note pattern like `Simple 8` (`>0`) is selected. This preserves the rhythm of the preset while ensuring upward arpeggiation.

## Architecture

The AutoChord architecture is divided into the core chord-generation logic and its integration hooks within QMidiArp's event loop.

### 1. Core Engine (`src/autochord/`)
- **`AutoChord` (Singleton)**: The central state machine (`autochord.cpp`). It tracks the active note, scale, key signature, and extensions. When a note is pressed, it calls upon the mode engine to generate an `autochord_notes_t` buffer containing the absolute MIDI pitches and velocities for the chord.
- **`ArpModes` & `Scale`**: These modules (`arp_modes.cpp`, `scale.cpp`) contain the mathematical definitions of intervals, scales, and how specific degrees of a scale map to chord qualities (Major, Minor, Dim).

### 2. LV2 Plugin Integration (`src/midiarp_lv2.cpp`)
- **Parameters**: AutoChord exposes 5 new LV2 control ports: `State`, `Scale`, `Key Signature`, `Extensions`, and `Gated Tail Time`. These are updated in the `updateParams()` loop.
- **PAD Mode Forging**: In the `run()` function, `MidiArpLV2` intercepts `AUTOCHORD_PAD` mode. It checks if there are pending Note-On or Note-Off events from the AutoChord engine and uses `forgeMidiEvent()` to send them directly to the LV2 output buffer, bypassing the `MidiArp` sequence engine completely.
- **Gated Tail Engine**: During the standard ARP run loop, after querying `MidiArp::getNextFrame()`, the engine evaluates if the Drum Gate is active and about to close (`nextTick >= m_gateCloseTick`). If so, it overrides the `Note-Off` timestamp of the current frame to `curTick + Gated Tail Time`. All standard notes bypass this block and turn off at their native lengths.

### 3. Core QMidiArp Integration (`src/midiarp.cpp`)
- **Event Handling (`handleEvent`)**: 
  - Intercepts incoming `NOTE_ON` and `NOTE_OFF` events.
  - In `AUTOCHORD_ARP` mode, a single `NOTE_ON` queries the AutoChord singleton for the chord notes. The resulting notes are sequentially pushed into QMidiArp's internal `notes` buffer via `addNote()`.
- **Pattern Overrides (`advancePatternIndex`)**:
  - Modifies the pattern indexer to intercept `AUTOCHORD_ARP` mode and force `noteOfs` to increment at the end of every pattern cycle. This ensures the arpeggiator engine sweeps through the buffered chord notes instead of repeating the root note. 

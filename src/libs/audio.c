#include "audio.h"
#include <gb/gb.h>

void setupAudio() {
    //  Inicializacion del audio
    rAUDENA = AUDENA_ON; // 1000 0000 -> ENCIENDE EL SONIDO
    // Pone el volumen al máximo en ambos canales, izquierdo y derecho
    rAUDVOL = AUDVOL_VOL_LEFT(0x7) | AUDVOL_VOL_RIGHT(0x7);
    //  Canal 1: Quadrangular wave
    //  Canal 2: Quadrangular wave
    //  Canal 3: Programmable wave table
    //  Canal 4: Noise generator
    // 1111 1111 -> que canales queremos usar, un bit para cada canal. los primeros 4 bits son para el izquierdo y los segundos 4 para el canal derecho
    rAUDTERM = AUDTERM_4_LEFT | AUDTERM_3_LEFT | AUDTERM_2_LEFT | AUDTERM_1_LEFT |
               AUDTERM_4_RIGHT | AUDTERM_3_RIGHT | AUDTERM_2_RIGHT | AUDTERM_1_RIGHT;
}
// ============================================
// ================= playCrash =================
// ============================================

void playCrash() {
    // bit 5-0 Sound length
    // 0001 1111 is 0x1F the maximum length
    NR41_REG = 0x1F;

    // volume envelope
    // bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
    // bit 3 - Envelope Direction (0=Decrease, 1=Increase)
    // bit 2-0 - Number of envelope sweep (n: 0-7) (If zero, stop envelope operation.)
    // 1111 0001 is 0xF1
    // 1111     -   start at full volume,
    // 0        -   fade down,
    // 001      -   1 envelope sweep  (decimal)

    NR42_REG = 0xF1;

    // bit 7-4 - Shift Clock Frequency (s)
    // bit 3   - Counter Step/Width (0=15 bits, 1=7 bits)
    // bit 2-0 - Dividing Ratio of Frequencies (r)
    // The amplitude is randomly switched between high and low at the given frequency.
    // A higher frequency will make the noise to appear 'softer'.
    // When Bit 3 is set, the output will become more regular, and some frequencies will sound more like Tone than Noise.
    // 0011 0000 is 0x30
    // 0011     -   shift clock frequency 3
    // 0        -   step 0
    // 000      -   dividing ratio 0

    NR43_REG = 0x30;

    // bit 7   - Initial (1=Restart Sound)
    // bit 6   - Controls if last forever or stops when NR41 ends
    // (1=Stop output when length in NR41 expires)
    // bit 5-0	Unused
    // 1100 0000 is 0xC0
    // 1        -       start sound
    // 1        -       not continuous
    // 00 0000  -       unused
    NR44_REG = 0xC0;
}

// ============================================
// ================= playFlap =================
// ============================================

void playFlap() {
    // see https://github.com/bwhitman/pushpin/blob/master/src/gbsound.txt
    // chanel 1 register 0, Frequency sweep settings
    // 7	Unused
    // 6-4	Sweep time(update rate) (if 0, sweeping is off)
    // 3	Sweep Direction (1: decrease, 0: increase)
    // 2-0	Sweep RtShift amount (if 0, sweeping is off)
    // 0001 0110 is 0x16
    // 0    -   Unused
    // 001  -   sweep time 1
    // 0    -   sweep direction increase
    // 110  -   shift ammount per step 110 (6 decimal)
    NR10_REG = 0x16;

    // chanel 1 register 1: Wave pattern duty and sound length
    // Channels 1 2 and 4
    // 7-6	Wave pattern duty cycle 0-3 (12.5%, 25%, 50%, 75%), duty cycle is how long a quadrangular  wave is "on" vs "off" so 50% (2) is both equal.
    // 5-0 sound length (higher the number shorter the sound)
    // 01000000 is 0x40,
    // 01       -   duty cycle 1 (25%),
    // 000000   -   wave length 0 (long)

    NR11_REG = 0x40;

    // chanel 1 register 2: Volume Envelope (Makes the volume get louder or quieter each "tick")
    // On Channels 1 2 and 4
    // 7-4	(Initial) Channel Volume
    // 3	Volume sweep direction (0: down; 1: up)
    // 2-0	Length of each step in sweep (if 0, sweeping is off)
    // NOTE: each step is n/64 seconds long, where n is 1-7
    // 0111 0011 is 0x73
    // 0111 -   volume 7
    // 0    -   sweep down
    // 011  -   step length 3

    NR12_REG = 0x73;

    // chanel 1 register 3: Frequency LSbs (Least Significant bits) and noise options
    // for Channels 1 2 and 3
    // 7-0	8 Least Significant bits of frequency (3 Most Significant Bits are set in register 4)
    NR13_REG = 0x00;

    // chanel 1 register 4: Playback and frequency MSbs
    // Channels 1 2 3 and 4
    // 7	Initialize (trigger channel start, AKA channel INIT) (Write only)
    // 6	Consecutive select/length counter enable (Read/Write). When "0", regardless of the length of data on the NR11 register, sound can be produced consecutively.  When "1", sound is generated during the time period set by the length data contained in register NR11.  After the sound is ouput, the Sound 1 ON flag, at bit 0 of register NR52 is reset.
    // 5-3	Unused
    // 2-0	3 Most Significant bits of frequency
    // 1100 0011 is 0xC3
    // 1    -    initialize
    // 1    -    no consecutive
    // 00 0 -    UNUSED
    // 011  -   frequency = MSB + LSB = 011 0000 0000 = 0x300

    NR14_REG = 0xC3;
}

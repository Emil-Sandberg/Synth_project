/**
 * Lab 6, dt047g HT2024
 * @author Emil Sandberg
 * @date 2025-03-25
 * @brief Course project, a in real-time digital synthesizer using the external SFML 3.0 library
 * @file sound_modifier_data.h
 */
#ifndef SFML_PROJECT_SOUND_MODIFIER_DATA_H
#define SFML_PROJECT_SOUND_MODIFIER_DATA_H
#include "Soundwave.h"

/**
 * @brief Contains all variables for sound modification
 */
//class sound_modifier_data {
//public:


inline Square square_wave(1);
inline Sine sine_wave(0);
inline Sawtooth saw_wave(0);
inline Triangle triangle_wave(0);

using WavePtr = std::variant<Square*, Sine*, Sawtooth*, Triangle*>;

inline std::vector<WavePtr> waveForms{
    &square_wave,&sine_wave,&saw_wave,&triangle_wave
};


    inline float sqr_amplitude{1.};
    inline float tri_amplitude{};
    inline float sin_amplitude{};
    inline float saw_amplitude{};

    inline double decay{1}; //!< Modifies the time taken for the sound to go silent after a key is released
    inline double attack{1}; //!< Modifies the time taken for the sound to reach maximum volume after a key is pressed

    inline float pulse_width_modulation_range = 0.; //!< Modifies the width of the square wave

    inline bool sustainON{false}; //!< Toggle to keep increasing volume until max volume reached after a key is released
//};


#endif //SFML_PROJECT_SOUND_MODIFIER_DATA_H

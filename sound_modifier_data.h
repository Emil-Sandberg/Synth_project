/**
 * Lab 6, dt047g HT2024
 * @author Emil Sandberg
 * @date 2025-03-25
 * @brief Course project, a in real-time digital synthesizer using the external SFML 3.0 library
 * @file sound_modifier_data.h
 */
#ifndef SFML_PROJECT_SOUND_MODIFIER_DATA_H
#define SFML_PROJECT_SOUND_MODIFIER_DATA_H

/**
 * @brief Contains all variables for sound modification
 */
class sound_modifier_data {
public:
    double sqr_amplitude{1.};
    double tri_amplitude{};
    double sin_amplitude{};
    double saw_amplitude{};

    double decay{1}; //!< Modifies the time taken for the sound to go silent after a key is released
    double attack{1}; //!< Modifies the time taken for the sound to reach maximum volume after a key is pressed

    double pulse_width_modulation_range = 0.; //!< Modifies the width of the square wave

    bool sustainON{false}; //!< Toggle to keep increasing volume until max volume reached after a key is released
};


#endif //SFML_PROJECT_SOUND_MODIFIER_DATA_H

/**
 * Lab 6, dt047g HT2024
 * @author Emil Sandberg
 * @date 2025-03-25
 * @brief Course project, a in real-time digital synthesizer using the external SFML 3.0 library
 * @file note_data.h
 */
#ifndef SFML_PROJECT_NOTE_DATA_H
#define SFML_PROJECT_NOTE_DATA_H

#define KEY_NUMBER (88) //!< Total number of notes on the keyboard
#define KEYBOARD_NOTE_INPUTS (16)

/**
 * @class note_data
 * @brief Holds the states of each note
 */
//class note_data {

inline float phase[KEY_NUMBER] = {}; //!< Keeps track of the samples current phase
inline double volume[KEY_NUMBER] = {};
inline bool holding[KEY_NUMBER] = {false};
inline bool reachedMaxVol[KEY_NUMBER] = {false};
inline int current_octave = 4; //!< The current octave state of the Keyboard
inline int input_current_octave[KEYBOARD_NOTE_INPUTS] = {};
//!< Stores the current octave position when a key is pressed
//};


#endif //SFML_PROJECT_NOTE_DATA_H

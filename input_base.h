/**
 * Lab 6, dt047g HT2024
 * @author Emil Sandberg
 * @date 2025-03-25
 * @brief Course project, a in real-time digital synthesizer using the external SFML 3.0 library
 * @file input_base.h
 */
#ifndef SFML_PROJECT_INPUT_BASE_H
#define SFML_PROJECT_INPUT_BASE_H

#include <algorithm>
#include <iostream>
#include "note_data.h"

#define SEMITONES_PER_OCTAVE (12) //!<The number of notes in an octave. \n
//!<e.g. How many notes there are in-between C3 and C4


/**
 * @brief Base class holding the necessary functions for every type of inputs
 */
class input_base {
public:

    input_base() = default;

    explicit input_base(sf::Keyboard::Key key) : key(key) {}

    virtual ~input_base() = default;

    virtual void action() = 0; //!< Decides what happens if a key is pressed

    virtual void release() = 0;  //!< Decides what happens if a key is released

    bool input_match(const sf::Event &event) {
        auto keyReleased = event.getIf<sf::Event::KeyReleased>();
        auto keyPressed = event.getIf<sf::Event::KeyPressed>();
        return (keyPressed && keyPressed->code == key) || (keyReleased && keyReleased->code == key);
    }

private:
    sf::Keyboard::Key key{};
};

/**
 * @brief inherited class from base_input. Adds a templated variable "output".
 * Which creates a versatile base for defining multiple outputs
 * @tparam T type of the chosen output
 */
template<typename T>
class input_t : public input_base {
public:

    input_t(sf::Keyboard::Key key, T output) : input_base(key), output(output) {}

    void release() override {};

    T get_output() { return output; }

private:
    T output;
};

/**
 * @brief Used for creating toggleable buttons by making output a pointer to a boolean value
 */
class input_button : public input_t<bool *> {
public:
    /**
     * @param key The keyboard key
     * @param output The targeted bool variable
     */
    input_button(sf::Keyboard::Key key, bool *output) : input_t<bool *>(key, output) {}

    void action() override {
        *get_output() = !*get_output();
    }
};

/**
 * @brief Used for handling all the notes on a keyboard by making output act as an index
 */
class input_note : public input_t<int> {

public:
    /**
     * @param data Pointer to note data
     * @param key The keyboard key
     * @param output The index of the notes pitch
     */
    input_note(note_data *data, sf::Keyboard::Key key, int output) : input_t<int>(key, output), note(data) {}

    void action() override {
        int pitch_class = get_output();

        //!calculates which note to play by shifting the inputs pitch class index based on the current octave position
        auto note_index = pitch_class + (note->current_octave * SEMITONES_PER_OCTAVE);
        if (!note->holding[note_index] && note_index < KEY_NUMBER) {
            note->input_current_octave[get_output()] = note->current_octave;
            note->volume[note_index] = 1;
            note->holding[note_index] = true;
            note->reachedMaxVol[note_index] = false;
        }
    }

    //!Finds the correct note to release by checking the stored octave position from input_current_octave
    void release() override {
        auto pitch_class = get_output();
        auto note_index = pitch_class + (note->input_current_octave[pitch_class] * SEMITONES_PER_OCTAVE);
        note->holding[note_index] = false;
    }

    note_data *note; //!< Pointer to Note data which holds the current states of each note
};

/**
 * @brief Used to modify a certain target by increasing or decreasing its value
 * @tparam T The type of the target variable
 * @tparam TNumeric The type of the range which in which the target variable can be modified
 */
template<typename TNumeric>
class modifier : public input_t<TNumeric> {

public:
    /**
     * @param key The keyboard key
     * @param target The variable that will be modified
     * @param output The amount the target-value will be increased/decreased
     * @param lo Lowest allowed target value
     * @param hi Highest allowed target value
     */
    modifier(sf::Keyboard::Key key, TNumeric *target, TNumeric output, TNumeric lo, TNumeric hi) :
            input_t<TNumeric>(key, output), target(target), hi(hi), lo(lo) {}

    void action() override {
        *target = std::clamp(*target + this->get_output(), lo, hi);
    }

    TNumeric *target; //!< Pointer to the modifiable variable
    TNumeric lo;
    TNumeric hi;
};

/**
 * @brief Container class for holding each input type
 */
class input_container {
public:

    template<typename T>
    void push(T object) {
        inputs.push_back(std::make_unique<T>(object));
    }

    input_container() = default;

    void is_released(sf::Event::KeyReleased event) {
        for (auto &element: inputs) {
            if (element->input_match(event)) {
                element->release();
                break;
            }
        }
    }

    void is_pressed(sf::Event::KeyPressed keyPressed) {
        for (auto &element: inputs) {
            if (element->input_match(keyPressed)) {
                element->action();
                break;
            }
        }
    }

    std::vector<std::unique_ptr<input_base>> inputs;
};

#endif //SFML_PROJECT_INPUT_BASE_H

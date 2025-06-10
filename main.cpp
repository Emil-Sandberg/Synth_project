/**
 * Lab 6, dt047g HT2024
 * @author Emil Sandberg
 * @date 2025-03-25
 * @brief Course project, a in real-time digital synthesizer using the external SFML 3.0 library
 * @file main.cpp
 */
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "input_base.h"
#include "MyStream.h"
#include "sound_modifier_data.h"

#define SAMPLERATE      44100
#define AUDIOBUFSIZE    1024
#define CHANNELCOUNT    2

int main() {
    sound_modifier_data sound_modifier;
    note_data noteData;

    //!Contains all modifier inputs and button inputs
    input_container inputContainer;

    inputContainer.push(modifier(sf::Keyboard::Key::Z, &noteData.current_octave, -1, 0, 6));
    inputContainer.push(modifier(sf::Keyboard::Key::X, &noteData.current_octave, 1, 0, 6));

    inputContainer.push(modifier(sf::Keyboard::Key::Numpad4, &sound_modifier.decay, -1., 1., 100.));
    inputContainer.push(modifier(sf::Keyboard::Key::Numpad6, &sound_modifier.decay, 1., 1., 100.));

    inputContainer.push(modifier(sf::Keyboard::Key::Numpad1, &sound_modifier.attack, -1., 1., 100.));
    inputContainer.push(modifier(sf::Keyboard::Key::Numpad3, &sound_modifier.attack, 1., 1., 100.));

    inputContainer.push(modifier(sf::Keyboard::Key::Left, &sound_modifier.pulse_width_modulation_range, -0.01, 0., 0.9));
    inputContainer.push(modifier(sf::Keyboard::Key::Right, &sound_modifier.pulse_width_modulation_range, 0.01, 0., 0.9));

    inputContainer.push(modifier(sf::Keyboard::Key::Num1, &sound_modifier.sqr_amplitude, -0.1, 0., 1.));
    inputContainer.push(modifier(sf::Keyboard::Key::Num2, &sound_modifier.sqr_amplitude, 0.1, 0., 1.));

    inputContainer.push(modifier(sf::Keyboard::Key::Num3, &sound_modifier.tri_amplitude, -0.1, 0., 1.));
    inputContainer.push(modifier(sf::Keyboard::Key::Num4, &sound_modifier.tri_amplitude, 0.1, 0., 1.));

    inputContainer.push(modifier(sf::Keyboard::Key::Num5, &sound_modifier.sin_amplitude, -0.1, 0., 1.));
    inputContainer.push(modifier(sf::Keyboard::Key::Num6, &sound_modifier.sin_amplitude, 0.1, 0., 1.));

    inputContainer.push(modifier(sf::Keyboard::Key::Num7, &sound_modifier.saw_amplitude, -0.1, 0., 1.));
    inputContainer.push(modifier(sf::Keyboard::Key::Num8, &sound_modifier.saw_amplitude, 0.1, 0., 1.));

    inputContainer.push(input_button(sf::Keyboard::Key::Numpad7, &sound_modifier.sustainON));


    //!Note input container \n
    //!separated into its own container to avoid looping through unnecessary inputs while checking for key-release
    input_container notes;
    notes.push(input_note(&noteData, sf::Keyboard::Key::Q, 0));
    notes.push(input_note(&noteData, sf::Keyboard::Key::A, 1));
    notes.push(input_note(&noteData, sf::Keyboard::Key::W, 2));
    notes.push(input_note(&noteData, sf::Keyboard::Key::S, 3));
    notes.push(input_note(&noteData, sf::Keyboard::Key::D, 4));
    notes.push(input_note(&noteData, sf::Keyboard::Key::R, 5));
    notes.push(input_note(&noteData, sf::Keyboard::Key::F, 6));
    notes.push(input_note(&noteData, sf::Keyboard::Key::T, 7));
    notes.push(input_note(&noteData, sf::Keyboard::Key::G, 8));
    notes.push(input_note(&noteData, sf::Keyboard::Key::H, 9));
    notes.push(input_note(&noteData, sf::Keyboard::Key::U, 10));
    notes.push(input_note(&noteData, sf::Keyboard::Key::J, 11));
    notes.push(input_note(&noteData, sf::Keyboard::Key::I, 12));
    notes.push(input_note(&noteData, sf::Keyboard::Key::K, 13));
    notes.push(input_note(&noteData, sf::Keyboard::Key::O, 14));
    notes.push(input_note(&noteData, sf::Keyboard::Key::L, 15));
    notes.push(input_note(&noteData, sf::Keyboard::Key::Grave, 16));


    sf::RenderWindow window(sf::VideoMode({400, 100}), "This is the best I could do visually");

    MyStream stream(&sound_modifier, &noteData, AUDIOBUFSIZE, CHANNELCOUNT, SAMPLERATE);
    stream.setVolume(100);
    stream.play();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (auto KeyPressed = event->getIf<sf::Event::KeyPressed>()) {
                inputContainer.is_pressed(*KeyPressed);
                notes.is_pressed(*KeyPressed);
            }
            if (auto KeyReleased = event->getIf<sf::Event::KeyReleased>()) {
                notes.is_released(*KeyReleased);
            }
        }
    }

    return 0;
}

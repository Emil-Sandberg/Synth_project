/**
 * Lab 6, dt047g HT2024
 * @author Emil Sandberg
 * @date 2025-03-25
 * @brief Course project, in real-time digital synthesizer using the external SFML 3.0 library
 * @file main.cpp
 */
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "input_base.h"
#include "MyStream.h"
#include "Soundwave.h"
#include "sound_modifier_data.h"


#define SAMPLERATE      44100
#define AUDIOBUFSIZE    1024
#define CHANNELCOUNT    2


input_container inputs{

    input_note(sf::Keyboard::Key::Q, 0),
    input_note(sf::Keyboard::Key::A, 1),
    input_note(sf::Keyboard::Key::W, 2),
    input_note(sf::Keyboard::Key::S, 3),
    input_note(sf::Keyboard::Key::D, 4),
    input_note(sf::Keyboard::Key::R, 5),
    input_note(sf::Keyboard::Key::F, 6),
    input_note(sf::Keyboard::Key::T, 7),
    input_note(sf::Keyboard::Key::G, 8),
    input_note(sf::Keyboard::Key::H, 9),
    input_note(sf::Keyboard::Key::U, 10),
    input_note(sf::Keyboard::Key::J, 11),
    input_note(sf::Keyboard::Key::I, 12),
    input_note(sf::Keyboard::Key::K, 13),
    input_note(sf::Keyboard::Key::O, 14),
    input_note(sf::Keyboard::Key::L, 15),
    input_note(sf::Keyboard::Key::Grave, 16),

    modifier(sf::Keyboard::Key::Z, &current_octave, -1, 0, 6),
    modifier(sf::Keyboard::Key::X, &current_octave, 1, 0, 6),

    modifier(sf::Keyboard::Key::Numpad4, &decay, -1., 1., 100.),
    modifier(sf::Keyboard::Key::Numpad6, &decay, 1., 1., 100.),

    modifier(sf::Keyboard::Key::Numpad1, &attack, -1., 1., 100.),
    modifier(sf::Keyboard::Key::Numpad3, &attack, 1., 1., 100.),

    modifier<float>(sf::Keyboard::Key::Left, &square_wave.PWM_Range, -0.01, 0., 0.9),
    modifier<float>(sf::Keyboard::Key::Right, &square_wave.PWM_Range, 0.01, 0., 0.9),

    modifier<float>(sf::Keyboard::Key::Num1, &square_wave.amplitude, -0.1, 0., 1.),
    modifier<float>(sf::Keyboard::Key::Num2, &square_wave.amplitude, 0.1, 0., 1.),

    modifier<float>(sf::Keyboard::Key::Num3, &triangle_wave.amplitude, -0.1, 0., 1.),
    modifier<float>(sf::Keyboard::Key::Num4, &triangle_wave.amplitude, 0.1, 0., 1.),

    modifier<float>(sf::Keyboard::Key::Num5, &sine_wave.amplitude, -0.1, 0., 1.),
    modifier<float>(sf::Keyboard::Key::Num6, &sine_wave.amplitude, 0.1, 0., 1.),

    modifier<float>(sf::Keyboard::Key::Num7, &saw_wave.amplitude, -0.1, 0., 1.),
    modifier<float>(sf::Keyboard::Key::Num8, &saw_wave.amplitude, 0.1, 0., 1.),

    input_button(sf::Keyboard::Key::Numpad7, &sustainON)

};

void updateScaling(sf::RenderWindow &window, sf::Sprite &sprite,const sf::Texture& texture) {

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = texture.getSize();

    // Optional: scale sprite to fit window
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    float scale = std::min(scaleX, scaleY); // Letterbox scale

    sprite.setScale(sf::Vector2f(scale, scale));

    sf::Vector2f spriteSize(
        scale * static_cast<float>(textureSize.x),
        scale * static_cast<float>(textureSize.y)
    );
    sprite.setPosition(
        sf::Vector2f((windowSize.x - spriteSize.x) / 2.0f,
        (windowSize.y - spriteSize.y) / 2.0f)
    );
}


int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");
    sf::Texture texture;
    if (!texture.loadFromFile("../Input_explanation.png")) {
        return -1; // Failed to load image
    }

    sf::Sprite sprite(texture);

    updateScaling(window, sprite, texture);

    MyStream stream(AUDIOBUFSIZE, CHANNELCOUNT, SAMPLERATE);
    stream.setVolume(100);
    stream.play();


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (auto KeyPressed = event->getIf<sf::Event::KeyPressed>()) {
                inputs.is_pressed(*KeyPressed);
            }
            else if (auto KeyReleased = event->getIf<sf::Event::KeyReleased>()) {
                inputs.is_released(*KeyReleased);
            }
            if (auto resized = event->getIf<sf::Event::Resized>()) {
                sf::Vector2f position(2.f, 0.f);
                sf::Vector2f size(static_cast<float>(resized->size.x), static_cast<float>(resized->size.y));
                window.setView(sf::View(sf::FloatRect(position, size)));
                updateScaling(window, sprite,texture);
            }
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}

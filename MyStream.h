/**
 * Lab 6, dt047g HT2024
 * @author Emil Sandberg
 * @date 2025-03-25
 * @brief Course project, a in real-time digital synthesizer using the external SFML 3.0 library
 * @file MyStream.h
 */
#ifndef SFML_PROJECT_MYSTREAM_H
#define SFML_PROJECT_MYSTREAM_H

#include <SFML/Audio.hpp>
#include <cmath>
#include "note_data.h"
#include "sound_modifier_data.h"
#define MAXVOL (INT16_MAX / 6) //!< Maximum volume. Divided by 6 to ensure that at least six notes can play simultaneously
#define A4_FREQ 440 //!< Standard tuning frequency of A4
#define A4_POS 49 //!< Default key placement of A4 on a keyboard

/**
 * @brief Overloaded SFML MyStream class to allow sound generation
 */
class MyStream : public sf::SoundStream {

public:
    MyStream() = default;

    static float note_to_freq(const int &reference_freq,const int &reference_note_pos,const int &note_pos);
    MyStream(const int &audioBufSize, const int &channelCount,const int &sampleRate);
private:

    bool onGetData(Chunk &data) override;

    void onSeek(sf::Time timeOffset) override {
        m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
    }

    /**
    * Calculates the corresponding target note frequency based on chosen tuning standard
    * \param reference_freq the frequency of the reference note. Usually 440hz for A4
    * \param reference_note_pos the reference note position. Usually 49 for A4
    * \param note_pos key position of target note
     * \return the corresponding frequency of target note
    */


    std::vector<sf::SoundChannel> channelMap{
            sf::SoundChannel::FrontLeft,
            sf::SoundChannel::FrontRight,
    };
    int audioBufSize;
    int channelCount;
    int sampleRate;
    //sound_modifier_data *sound_modifier;
    //note_data *note;
    std::vector<int16_t> m_samples;
    std::size_t m_currentSample{};
    double sample_phase_step{};
};

#endif //SFML_PROJECT_MYSTREAM_H

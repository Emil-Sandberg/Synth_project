/**
 * Lab 6, dt047g HT2024
 * @author Emil Sandberg
 * @date 2025-03-25
 * @brief Course project, a in real-time digital synthesizer using the external SFML 3.0 library
 * @file MyStream.cpp
 */
#include "MyStream.h"

#include <cmath>
#include "Soundwave.h"

/**
 * @param sound_modifier Pointer to sound_modifier_data object
 * @param note Pointer to note_data object
 * @param audioBufSize The size for m_samples
 * @param channelCount Number of channels that produces sound i.e. 2 for stereo sound
 * @param sampleRate How frequent the program should sample the sound wave
 */
MyStream::MyStream(const int &audioBufSize, const int &channelCount,const int &sampleRate) :
        audioBufSize(audioBufSize),
        channelCount(channelCount), sampleRate(sampleRate){
    m_samples.resize(audioBufSize, 0);
    m_currentSample = 0;
    initialize(this->channelCount, this->sampleRate, channelMap);
}


bool MyStream::onGetData(sf::SoundStream::Chunk &data)  {
    std::ranges::fill(m_samples, 0);
    data.samples = &m_samples[m_currentSample];


    float cycle = 2 * M_PI;

    for (int current_note{}; current_note < KEY_NUMBER; current_note++) {

        if (volume[current_note] > 0) {
            float freq = note_to_freq(A4_FREQ, A4_POS, current_note); //!<Current note frequency
            sample_phase_step = cycle * freq / sampleRate; //!< By how many steps the phase should increase each loop

            if((!reachedMaxVol[current_note] && holding[current_note])
                    ||(sustainON && !reachedMaxVol[current_note])){

                volume[current_note] += INT16_MAX * std::pow(10, -log(attack));
                if (volume[current_note] >= MAXVOL) {
                    volume[current_note] = MAXVOL;
                    reachedMaxVol[current_note] = true;
                }
            }
            for (int i{}; i < audioBufSize; i += channelCount) {
                int left_channel = i;
                int right_channel = i + 1;

                float numberOfWaves = 0.0; //!< Keeps track of combined wave-amplitude to ensure a pretty even
                                            //!< volume regardless wave combination. Although some waves are
                                            //!< perceived as louder since they have harsher peaks and throughs\n e.g.
                                            //!< Sawtooth and Square(which is only peaks and throughs).
                //Calculations for each waveform

                float combined_wave_result{};
                for (auto& wave : waveForms) {
                    std::visit([&](auto* w) {
                        if (w->amplitude > 0) {
                            auto result = w->get_phase(phase[current_note]);
                            combined_wave_result += result * w->amplitude;
                            numberOfWaves += w->amplitude;
                        }
                    }, wave);
                }

                if (numberOfWaves < 1.0) {
                    numberOfWaves = 1.0;
                }
                int16_t currentSample = m_samples[i] + combined_wave_result * volume[current_note] / numberOfWaves;

                m_samples[left_channel] = currentSample;
                m_samples[right_channel] = currentSample;
                phase[current_note] += sample_phase_step;
            }
            phase[current_note] = std::fmod(phase[current_note], cycle); //!Resets the note phase pos to its first cycle position
        }
        if ((!holding[current_note] && sustainON && reachedMaxVol[current_note])
        ||(!holding[current_note] && !sustainON)){

            if (volume[current_note] > 0) {
                volume[current_note] -= INT16_MAX * std::pow(10, -log(decay));
            } else {
                volume[current_note] = 0;
                reachedMaxVol[current_note] = false;
            }
        }
    }
    data.sampleCount = audioBufSize;
    m_currentSample = 0;
    return true;
}

// C
float MyStream::note_to_freq(const int &reference_freq, const int &reference_note_pos, const int &note_pos) {
    double freq = reference_freq * std::pow(2, (note_pos - reference_note_pos) / 12.0);
    return freq;
}
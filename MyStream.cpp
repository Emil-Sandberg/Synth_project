/**
 * Lab 6, dt047g HT2024
 * @author Emil Sandberg
 * @date 2025-03-25
 * @brief Course project, a in real-time digital synthesizer using the external SFML 3.0 library
 * @file MyStream.cpp
 */
#include "MyStream.h"

/**
 * @param sound_modifier Pointer to sound_modifier_data object
 * @param note Pointer to note_data object
 * @param audioBufSize The size for m_samples
 * @param channelCount Number of channels that produces sound i.e. 2 for stereo sound
 * @param sampleRate How frequent the program should sample the sound wave
 */
MyStream::MyStream(sound_modifier_data *sound_modifier, note_data *note,const int &audioBufSize, const int &channelCount,const int &sampleRate) :
        sound_modifier(sound_modifier), note(note), audioBufSize(audioBufSize),
        channelCount(channelCount), sampleRate(sampleRate){
    m_samples.resize(audioBufSize, 0);
    m_currentSample = 0;
    initialize(this->channelCount, this->sampleRate, channelMap);
}

bool MyStream::onGetData(sf::SoundStream::Chunk &data)  {
    std::ranges::fill(m_samples, 0);
    data.samples = &m_samples[m_currentSample];


    double cycle = 2 * M_PI;
    double sin_result = 0.; //!< Value of the current sine wave sample
    double sqr_result = 0.; //!< Value of the current square wave sample
    double tri_result = 0.; //!< Value of the current triangle wave sample
    double saw_result = 0.; //!< Value of the current sawtooth wave sample

    for (int current_note{}; current_note < KEY_NUMBER; current_note++) {

        if (note->volume[current_note] > 0) {
            double freq = note_to_freq(A4_FREQ, A4_POS, current_note); //!<Current note frequency
            sample_phase_step = cycle * freq / sampleRate; //!< By how many steps the phase should increase each loop

            if((!note->reachedMaxVol[current_note] && note->holding[current_note])
                    ||(sound_modifier->sustainON && !note->reachedMaxVol[current_note])){

                note->volume[current_note] += INT16_MAX * std::pow(10, -log(sound_modifier->attack));
                if (note->volume[current_note] >= MAXVOL) {
                    note->volume[current_note] = MAXVOL;
                    note->reachedMaxVol[current_note] = true;
                }
            }
            for (int i{}; i < audioBufSize; i += channelCount) {
                int left_channel = i;
                int right_channel = i + 1;

                double numberOfWaves = 0.0; //!< Keeps track of combined wave-amplitude to ensure a pretty even
                                            //!< volume regardless wave combination. Although some waves are
                                            //!< perceived as louder since they have harsher peaks and throughs\n e.g.
                                            //!< Sawtooth and Square(which is only peaks and throughs).
                //Calculations for each waveform
                if (sound_modifier->sin_amplitude > 0) {
                    sin_result = sin(note->phase[current_note]);
                    numberOfWaves += sound_modifier->sin_amplitude;
                }
                if (sound_modifier->sqr_amplitude > 0) {
                    sqr_result = sin(note->phase[current_note]);
                    if (sqr_result < -sound_modifier->pulse_width_modulation_range) {
                        sqr_result = -1;
                    } else if (sqr_result > sound_modifier->pulse_width_modulation_range) {
                        sqr_result = 1;
                    } else {
                        sqr_result = 0;
                    }
                    numberOfWaves += sound_modifier->sqr_amplitude;
                }
                if (sound_modifier->tri_amplitude > 0) {
                    numberOfWaves += sound_modifier->tri_amplitude;
                    tri_result = (2 / M_PI) * asin(sin(note->phase[current_note]));
                }
                if (sound_modifier->saw_amplitude > 0) {
                    numberOfWaves += sound_modifier->saw_amplitude;
                    saw_result = (2 / M_PI) * atan(tan(note->phase[current_note] / 2));
                }
                if (numberOfWaves < 1.0) {
                    numberOfWaves = 1.0;
                }

                double combined_wave_result =   sin_result * sound_modifier->sin_amplitude
                                              + sqr_result * sound_modifier->sqr_amplitude
                                              + tri_result * sound_modifier->tri_amplitude
                                              + saw_result * sound_modifier->saw_amplitude;

                int16_t currentSample = m_samples[i] + combined_wave_result * note->volume[current_note] / numberOfWaves;

                m_samples[left_channel] = currentSample;
                m_samples[right_channel] = currentSample;
                note->phase[current_note] += sample_phase_step;
            }
            note->phase[current_note] = fmod(note->phase[current_note], cycle); //!Resets the note phase pos to its first cycle position
        }
        if ((!note->holding[current_note] && sound_modifier->sustainON && note->reachedMaxVol[current_note])
        ||(!note->holding[current_note] && !sound_modifier->sustainON)){

            if (note->volume[current_note] > 0) {
                note->volume[current_note] -= INT16_MAX * std::pow(10, -log(sound_modifier->decay));
            } else {
                note->volume[current_note] = 0;
                note->reachedMaxVol[current_note] = false;
            }
        }
    }
    data.sampleCount = audioBufSize;
    m_currentSample = 0;
    return true;
}

double MyStream::note_to_freq(const int &reference_freq, const int &reference_note_pos, const int &note_pos) const {
    double freq = reference_freq * std::pow(2, (note_pos - reference_note_pos) / 12.0);
    return freq;
}
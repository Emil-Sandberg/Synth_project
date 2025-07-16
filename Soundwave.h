//
// Created by emils on 10/07/2025.
//

#ifndef SOUNDWAVE_H
#define SOUNDWAVE_H
#include <cmath>


template<typename Derived>
struct Soundwave {
    explicit Soundwave(const float amplitude) : amplitude(amplitude) {
    }

    virtual ~Soundwave() = default;

    float amplitude;

};


struct Square final : Soundwave<Square> {
    explicit Square(const float amplitude) : Soundwave(amplitude) {

    };

    float get_phase(const float phase_step) {
        float result{};
        if (amplitude > 0) {
            result = std::sin(phase_step);
            if (result < -PWM_Range) {
                result = -1;
            } else if (result > PWM_Range) {
                result = 1;
            } else {
                result = 0;
            }

        }
        return result;
    }
    float PWM_Range{};
};

struct Triangle final : Soundwave<Triangle> {
    explicit Triangle(const float amplitude) : Soundwave(amplitude) {
    };

    float get_phase(float phase_step)  {
        float result = 0.;
        if (amplitude > 0.) {
            result =  (2 / M_PI) * std::asin(std::sin(phase_step));
        }
        return result;
    }
};

struct Sawtooth final : Soundwave<Sawtooth> {
    explicit Sawtooth(const float amplitude) : Soundwave(amplitude) {

    };

    float get_phase(float phase_step)  {
        float result = 0.;
        if (amplitude > 0) {
            result = (2 / M_PI) * std::atan(std::tan(phase_step / 2));
        }
        return result;
    }
};

struct Sine final : Soundwave<Sine> {
    explicit Sine(const float amplitude) : Soundwave(amplitude) {
    };

    float get_phase(float phase_step)  {
        if (amplitude > 0) {
            return std::sin(phase_step);
        }
        return 0.;
    }
};

#endif //SOUNDWAVE_H

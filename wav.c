#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define BIT_DEPTH 16
#define DURATION 2

struct SineOscillator {
    float frequency;
    float amplitude;
    float angle;
    float offset;
};

struct SineOscillator *init_sine_oscillator(float freq, float amp) {
    struct SineOscillator *so = malloc(sizeof(struct SineOscillator));

    so->frequency = freq;
    so->amplitude = amp;
    so->angle = 0.0f;
    so->offset = 0.0f;
    so->angle += so->amplitude * sin(2 * M_PI * so->frequency / SAMPLE_RATE);
    so->offset = 2 * M_PI * so->frequency / SAMPLE_RATE;

    return so;
}

void destroy_sine_oscillator(struct SineOscillator* so) {
    free(so);
}

float process(struct SineOscillator* so) {
    float sample = so->amplitude * sin(so->angle);
    so->angle += so->offset;
    return sample;
}

int main() {
    struct SineOscillator *so = init_sine_oscillator(440, 0.5);

    FILE* file = fopen("waveform", "wb");

    float max_amplitude = pow(2, BIT_DEPTH - 1) - 1;
    for(int i = 0; i < SAMPLE_RATE * DURATION; ++i) {
        float sample = process(so);
        int int_sample = (int)(sample * max_amplitude);

        fwrite((char*)&int_sample, 1, 2, file);
    }

    fclose(file);
    destroy_sine_oscillator(so);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SAMPLE_RATE 44100
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

    for(int i = 0; i < SAMPLE_RATE * DURATION; ++i) {
        float res = process(so);
        fprintf(file, "%f", res);
    }

    fclose(file);
    destroy_sine_oscillator(so);
    return 0;
}

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

    FILE* file = fopen("waveform.wav", "wb");

    // Header chunk
    fwrite("RIFF", 1, 4, file);
    fwrite("----", 1, 4, file);
    fwrite("WAVE", 1, 4, file);

    // Format chunk
    fwrite("fmt ", 1, 4, file);
    int size = 16;
    fwrite((char*)&size, 1, 4, file);
    
    int compression_code = 1;
    fwrite((char*)&compression_code, 1, 2, file);

    int channels = 2;
    fwrite((char*)&channels, 1, 2, file);

    int sample_rate = 4;
    fwrite((char*)&sample_rate, 1, 4, file);

    int bit_rate = sample_rate * BIT_DEPTH / 8;
    fwrite((char*)&bit_rate, 1, 4, file);

    int block_align = BIT_DEPTH / 8;
    fwrite((char*)&block_align, 1, 2, file);

    int bit_depth = BIT_DEPTH;
    fwrite((char*)&bit_depth, 1, 2, file);

    // Data chunk
    fwrite("data", 1, 4, file);
    fwrite("----", 1, 4, file);

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

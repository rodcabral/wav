#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define SAMPLE_RATE 44100
#define BIT_DEPTH 16

struct SineOscillator {
    float frequency;
    float amplitude;
    float angle;
    float offset;
};

struct SineOscillator* init_sine_oscillator(float freq, float amp) {
    struct SineOscillator* so = malloc(sizeof(struct SineOscillator));

    so->offset = 0.0f;
    so->frequency = freq;
    so->amplitude = amp;
    so->angle = 0.0f;

    so->offset = 2 * M_PI * so->frequency / SAMPLE_RATE;

    return so;
}

float process_so(struct SineOscillator* so) {
    float sample = so->amplitude * sin(so->angle);
    so->angle += so->offset;
    return sample;
}

void destroy_sine_oscillator(struct SineOscillator* so) {
    free(so);
}

void write(FILE*fptr, int val, int size) {
    fwrite((char*)&val, 1, size, fptr);
}

int main(void) {
    int duration = 2;
    struct SineOscillator* so = init_sine_oscillator(440, 0.5);

    FILE* file = fopen("waveform.wav", "wb");

    // Header
    fwrite("RIFF", 1, 4, file);
    fwrite("----", 1, 4, file);
    fwrite("WAVE", 1, 4, file);

    // Format
    fwrite("fmt ", 1, 4, file);
    write(file, 16, 4);
    write(file, 1, 2);
    write(file, 1, 2);
    write(file, SAMPLE_RATE, 4);
    write(file, SAMPLE_RATE * BIT_DEPTH / 8, 4);
    write(file, BIT_DEPTH / 8, 2);
    write(file, BIT_DEPTH, 2);

    fwrite("data", 1, 4, file);
    fwrite("----", 1, 4, file);
    
    int pre_audio_pos = ftell(file);

    int max_amplitude = pow(2, BIT_DEPTH - 1) - 1;
    for(int i = 0; i < SAMPLE_RATE * duration; ++i) {
        float sample = process_so(so);
        int int_sample = (int)(sample*max_amplitude);
        write(file, int_sample, 2);
    }
    int post_audio_pos = ftell(file);  

    fseek(file, pre_audio_pos - 4, SEEK_SET);
    write(file, post_audio_pos - pre_audio_pos, 4);
        
    fseek(file, 4, SEEK_SET);
    write(file, post_audio_pos - 8, 4);

    printf("pre: %d\n", pre_audio_pos);
    printf("pos: %d\n", post_audio_pos);

    destroy_sine_oscillator(so);
    fclose(file);
    return 0;
}

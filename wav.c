#include <stdio.h>
#include <stdlib.h>

struct SineOscillator {
    float frequency;
    float amplitude;
};

struct SineOscillator *init_sine_oscillator(float freq, float amp) {
    struct SineOscillator *so = malloc(sizeof(struct SineOscillator));

    so->frequency = freq;
    so->amplitude = amp;

    return so;
}

void destroy_sine_oscillator(struct SineOscillator* so) {
    free(so);
}

int main() {

    return 0;
}

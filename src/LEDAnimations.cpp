#ifndef LEDANIMATIONS_CPP
#define LEDANIMATIONS_CPP

#include "application.h"
#include "LEDAnimations.h"

CRGB leds[NUM_LEDS];

typedef void (LEDAnimations::*AnimationList)();

AnimationList animationsRails[] = {&LEDAnimations::confetti, &LEDAnimations::sinelon, &LEDAnimations::juggle,
                                   &LEDAnimations::fillColor, &LEDAnimations::rainbow,
                                   &LEDAnimations::rainbowSlide,
                                   &LEDAnimations::waterfallRainbowBorder};

LEDAnimations::LEDAnimations(SpectrumEqualizerClient *eq) : AmbientBeatsLEDAnimations(eq) {
    animationCount = ARRAY_SIZE(animationsRails);
    audioReactiveOn = false;
    hue = 64;
    brightness = 120;
    saturation = 150;
}

int LEDAnimations::runAnimation() {
  (this->*animationsRails[animation])();
}

int LEDAnimations::toggleAudioReactive() {
  return 0;
}

void LEDAnimations::clearAllLeds() {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void LEDAnimations::fillColor() {
    fill_solid(leds, NUM_LEDS, CHSV(hue, saturation, brightness));
}
//
// enum { SteadyDim, GettingBrighter, GettingDimmerAgain };
// uint8_t PixelState[NUM_LEDS];
//
// void initPixelStates()
// {
//   memset( PixelState, sizeof(PixelState), SteadyDim); // initialize all the pixels to SteadyDim.
//   fill_solid( leds, NUM_LEDS, BASE_COLOR);
// }
//
void LEDAnimations::twinkle(){
//   for( uint16_t i = 0; i < NUM_LEDS; i++) {
//     if( PixelState[i] == SteadyDim) {
//       // this pixels is currently: SteadyDim
//       // so we randomly consider making it start getting brighter
//       if( random8() < CHANCE_OF_TWINKLE) {
//         PixelState[i] = GettingBrighter;
//       }
//
//     } else if( PixelState[i] == GettingBrighter ) {
//       // this pixels is currently: GettingBrighter
//       // so if it's at peak color, switch it to getting dimmer again
//       if( leds[i] >= PEAK_COLOR ) {
//         PixelState[i] = GettingDimmerAgain;
//       } else {
//         // otherwise, just keep brightening it:
//         leds[i] += DELTA_COLOR_UP;
//       }
//
//     } else { // getting dimmer again
//       // this pixels is currently: GettingDimmerAgain
//       // so if it's back to base color, switch it to steady dim
//       if( leds[i] <= BASE_COLOR ) {
//         leds[i] = BASE_COLOR; // reset to exact base color, in case we overshot
//         PixelState[i] = SteadyDim;
//       } else {
//         // otherwise, just keep dimming it down:
//         leds[i] -= DELTA_COLOR_DOWN;
//       }
//     }
 }





void LEDAnimations::redGreenWhiteSlide(){

}

void LEDAnimations::rainbow() {
    fill_rainbow(leds, NUM_LEDS, hue);
}

void LEDAnimations::rainbowSlide() {
    fill_rainbow(leds, NUM_LEDS, hue);
    hue++;
}

// random colored speckles that blink in and fade smoothly
void LEDAnimations::confetti() {
    fadeToBlackBy(leds, NUM_LEDS, 400);
    uint16_t position = random16(NUM_LEDS);
    leds[position] += CHSV(hue, saturation, brightness);
    position = random16(NUM_LEDS);
    leds[position] += CHSV(hue, saturation, brightness);
    position = random16(NUM_LEDS);
    leds[position] += CHSV(hue, saturation, brightness);
    position = random16(NUM_LEDS);
    leds[position] += CHSV(hue, saturation, brightness);
    delay(100);
}

// a colored dot sweeping back and forth, with fading trails
void LEDAnimations::sinelon() {
    int frequencyValue = equalizer->frequenciesLeftChannel[frequencyMode[0]];
    uint16_t frequencyThreshold = clampSensitivity(sensitivity);

    fadeToBlackBy(leds, NUM_LEDS, 1);

    if(!audioReactiveOn || (frequencyValue > frequencyThreshold)) {
        int pos = beatsin16(13, 0, NUM_LEDS);
        leds[pos] += CHSV(hue, saturation, brightness);
    }
}

// eight colored dots, weaving in and out of sync with each other
void LEDAnimations::juggle() {
    int frequencyValue = equalizer->frequenciesLeftChannel[frequencyMode[0]];
    uint16_t frequencyThreshold = clampSensitivity(sensitivity);

    fadeToBlackBy(leds, NUM_LEDS, 20);
    byte dothue = 0;
    if (!audioReactiveOn || frequencyValue > frequencyThreshold) {
        for (int i = 0; i < 8; i++) {
            int currentLocation = beatsin16(i + 7, 0, NUM_LEDS);
            leds[currentLocation] |= CHSV(dothue, saturation, brightness);
            dothue += 32;
        }
    }
}

void LEDAnimations::waterfall() {
    int sensitivityValueMinThreshold = clampSensitivity(sensitivity);
    waterfallBorder(equalizer->frequenciesLeftChannel[frequencyMode[4]], sensitivityValueMinThreshold, brightness);
}

void LEDAnimations::waterfallBorder(int frequencyValue, int frequencyValueMinThreshold, int brightness) {
    if (!audioReactiveOn || frequencyValue > frequencyValueMinThreshold) {
        int mappedFrequencyValue = map(frequencyValue, frequencyValueMinThreshold, 4096, 0, 255);
        mappedFrequencyValue = (mappedFrequencyValue) % 255; //offsetting the base color...
        leds[NUM_LEDS / 2] = CHSV(mappedFrequencyValue, saturation, brightness);
    } else {
        leds[NUM_LEDS / 2] = CRGB(0, 0, 0);
    }
    // When doing a memmove, don't move outside of the bounds of the array!!!
    memmove(&leds[0], &leds[1], (NUM_LEDS / 2) * sizeof(CRGB));
    memmove(&leds[NUM_LEDS / 2 + 1], &leds[NUM_LEDS / 2], (NUM_LEDS / 2) * sizeof(CRGB));
}

void LEDAnimations::waterfallBorderRemote() {
    leds[NUM_LEDS / 2] = CHSV(hue, saturation, brightness);
    memmove(&leds[0], &leds[1], (NUM_LEDS / 2) * sizeof(CRGB));
    memmove(&leds[NUM_LEDS / 2 + 1], &leds[NUM_LEDS / 2], (NUM_LEDS / 2) * sizeof(CRGB));
}

uint8_t hueCounter = 0;

void LEDAnimations::waterfallRainbowBorder() {
    leds[NUM_LEDS / 2] = CHSV(hueCounter, saturation, brightness);
    memmove(&leds[0], &leds[1], (NUM_LEDS / 2) * sizeof(CRGB));
    memmove(&leds[NUM_LEDS / 2 + 1], &leds[NUM_LEDS / 2], (NUM_LEDS / 2) * sizeof(CRGB));
    hueCounter++;
}

void LEDAnimations::equalizerBorderOnly() {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    equalizerLeft(equalizer->frequenciesLeftChannel[frequencyMode[1]], clampSensitivity(sensitivity), true);
    equalizerRight(equalizer->frequenciesLeftChannel[frequencyMode[6]], clampSensitivity(sensitivity), true);
}

void LEDAnimations::equalizerRight(int frequencyValue, int sensitivityThreshold, bool direction) {
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, sensitivityThreshold, 3500, 0, NUM_LEDS / 2 - 1);
        CRGB color = CHSV(map(frequencyValue, sensitivityThreshold, 4096, 0, 255), saturation, brightness);
        if (direction) {
            for (int i = NUM_LEDS / 2 + 1; i < NUM_LEDS / 2 + 1 + numberToLight; i++) {
                leds[i] = color;
            }
        }
    }
}

void LEDAnimations::equalizerLeft(int frequencyValue, int sensitivityThreshold, bool direction) {
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, sensitivityThreshold, 3500, 0, NUM_LEDS / 2 - 1);
        CRGB color = CHSV(map(frequencyValue, sensitivityThreshold, 4096, 0, 255), saturation, brightness);
        if (direction) {
            for (int i = NUM_LEDS / 2; i > NUM_LEDS / 2 - numberToLight; i--) {
                leds[i] = color;
            }
        }
    }
}

#endif

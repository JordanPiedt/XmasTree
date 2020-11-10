#include "application.h"
#include "LEDAnimations.h"
#include "SpectrumEqualizerClient.h"
#include "AmbientBeatsCloudFunctions.h"

#define BORDER_LED_PIN    D1

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

SpectrumEqualizerClient *audioEqualizer;
LEDAnimations *animations;
AmbientBeatsCloudFunctions *cloudFunctions;

// runs once when board turns on
void setup() {
    audioEqualizer = new SpectrumEqualizerClient();
    animations = new LEDAnimations(audioEqualizer);
    cloudFunctions = new AmbientBeatsCloudFunctions(animations);

    FastLED.addLeds<LED_TYPE, BORDER_LED_PIN, COLOR_ORDER>(animations->leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void loop() {
    if(animations->poweredOn) {
        animations->runAnimation();
        FastLED.show();
    }
}
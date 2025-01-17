#include <Arduino.h>


#define PULSE_PIN 1
#define POT_PIN 2

const unsigned long period = 20 * 1000;
const unsigned long min_on_time = 4 * 1000;
const unsigned long min_off_time = 4 * 1000;

unsigned long last_transition_time = 0;

const long MAX_ADC_COUNTS = 1024;
const long high_limit = MAX_ADC_COUNTS * 95 / 100;
const long low_limit = MAX_ADC_COUNTS * 5 / 100;

bool pulse_state = LOW;


void setup() {
    pinMode(POT_PIN, INPUT);
    pinMode(PULSE_PIN, OUTPUT);
}

void loop() {
    int analogValue = analogRead(POT_PIN);

    if (analogValue > high_limit) {
        pulse_state = HIGH;
        digitalWrite(PULSE_PIN, pulse_state);
    }

    else if (analogValue < low_limit) {
        pulse_state = LOW;
        digitalWrite(PULSE_PIN, pulse_state);
    }

    else {
        int pulse_duty = map(analogValue, low_limit, high_limit, 0, 100);

        unsigned long on_time = (unsigned long) ((period * pulse_duty) / 100);
        unsigned long off_time = period - on_time;

        unsigned long time = millis();

        if (pulse_state) {
            if ((time - last_transition_time) > max(on_time, min_on_time)) {
                last_transition_time = time;
                pulse_state = LOW;
                digitalWrite(PULSE_PIN, pulse_state);
            }
        } else {
            if ((time - last_transition_time) > max(off_time, min_off_time)) {
                last_transition_time = time;
                pulse_state = HIGH;
                digitalWrite(PULSE_PIN, pulse_state);
            }
        }
    }
}

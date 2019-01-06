#define KNOB_PIN_1 2
#define KNOB_PIN_2 3
#define KNOB_ENC_P 10
#define JOG_KICKER_PIN 4

#define START_PIN A0
#define START_LED A1
#define STOP_PIN A2
#define STOP_LED A3
#define ZERO_PIN 6
#define ZERO_ENC_LED 7
#define CUT_PIN 8
#define CUT_LED 9
#define TOGGLE_PIN 10
#define TOGGLE_LED 11
#define PEDAL_PIN 13
#define NEOPIXEL_PIN 5

#define SPEAKER_PIN 12

#define TICKS_PER_REV 1600
#define ENC_WHEEL_D 0.995 //inches

#define SPOOL_CUTOFF_LEN 24 //inches

void pin_setup(){
  pinMode(KNOB_PIN_1, INPUT_PULLUP);
  pinMode(KNOB_PIN_2, INPUT_PULLUP);
}

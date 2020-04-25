
#include <Encoder.h>
#include "config.h"
#include "motor.h"
#include "communication.h"


// initialize encoder
Encoder mainEnc(ENCpin1, ENCpin2);
bool encoder_zeroed = false;
bool encoder_down = false;

// initialize motors
DCmotor M1(M1pwmPin, M1dirPin);
DCmotor M2(M2pwmPin, M2dirPin);

bool use_spool = false;

long int target = 0;
int desired_num = 1;
int count = 0;

unsigned long last_tick;

bool kicker_active = false;

#include "sub_states.h"
#include "states.h"

cmd_msg ser_msg;

void setup() {
  Serial.begin(9600);

  M1.P = M1_P;
  M1.D = M1_D;
  M1.deadzone = M1_deadzone;
  M1.max_accel = M1_MAX_ACCEL;

  M2.P = M2_P;
  M2.D = M2_D;
  M2.deadzone = M2_deadzone;
  M2.max_accel = M2_MAX_ACCEL;

  pin_setup();
  last_tick = micros();
}

// *** MAIN LOOP *** //
void loop() {

while (micros()-last_tick < 1000){}
last_tick = micros();

// check if zero is still valid
encoder_down = digitalRead(ENCreleasePin);
if (!encoder_down && encoder_zeroed){
  encoder_zeroed = false;
  send_msg(req_encoder_zero, 0);
}

// get communication
ser_msg = get_msg();

// main state controller
switch (state) {
  // states take the current message as an input and
  // return the state for the next loop
  case manual:
  state = manual_control(ser_msg);
  break;

  case noSpool:
  state = noSpool_control(ser_msg);
  break;

  case spool:
  state = spool_control(ser_msg);
  break;
};
M1.update();
M2.update();
}

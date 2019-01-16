#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "helper_funcs.h"
#include "states.h"
#include "lcdDisplay.h"
#include "buttons.h"
#include "lights.h"
#include "communication.h"
#include "speaker.h"

// initialize encoder
Encoder knob(KNOB_PIN_1, KNOB_PIN_2);

Button start_btn(START_PIN);
Button stop_btn(STOP_PIN);
Button zero_btn(ZERO_PIN);
Button toggle_btn(TOGGLE_PIN);
Button cut_btn(CUT_PIN);
Button jog_kicker_btn(JOG_KICKER_PIN);
Button pedal_btn(PEDAL_PIN);

LED zero_light(ZERO_ENC_LED);
LED start_light(START_LED);
LED stop_light(STOP_LED);
LED cut_light(CUT_LED);
LED toggle_light(TOGGLE_LED);

Speaker speaker(SPEAKER_PIN);

float length=0;
int units = in;
bool use_spool = false;
String keypresses = "";

#include "my_keypad.h"
my_Keypad keypad;

long knob_val = 0;

long last_tick;

bool kicker_active = false;

cmd_msg ser_msg;

void setup() {
  Serial.begin(9600);
  last_tick = micros();
  knob.write(0);
  keypad.setup();
  pin_setup();
  zero_light.on();
  set_backlight(0, 255, 255);
  speaker.done_tone();
}

// *** MAIN LOOP *** //
void loop(){
  bool any_press = standard_controls();
  bool any_key = keypad.update(any_press);
  if (any_press || any_key) speaker.press_tone();
  update_lights();
  speaker.update();
  ser_msg = get_msg();
  switch (ser_msg.type){
  case req_encoder_zero:
    if (ser_msg.msg==1){ zero_light.off(); start_light.on();}
    else {zero_light.on(); start_light.off();}
    break;
  case throw_err:
    change_state(err);
    start_light.off();
    keypad.set_err(ser_msg.msg);
    speaker.err_tone();
    break;
  case start_spool:
  case start_no_spool:
    start_light.blink();
    break;
  case manual:
    //start_light.off();
    break;
  case req_toggle_kicker:
    if (ser_msg.msg) toggle_light.on();
    else toggle_light.off();
    break;
  case cut_start:
    cut_light.fast_blink();
    break;
  case cut_done:
    start_light.on();
    zero_light.off();
    cut_light.off();
    break;
  case done:
    speaker.done_tone();
    break;
  case req_stop:
    start_light.on();
    cut_light.off();
    stop_light.blink_time(1000);
  default:
    break;
  };
}

bool standard_controls() {
  ser_msg = get_msg();
  update_btns();
  bool any_press = false;
  if (start_btn.press()){ send_msg(req_start); any_press=true;}
  if (stop_btn.press()){ send_msg(req_stop); any_press=true;}
  if (zero_btn.press()){ send_msg(req_encoder_zero); any_press=true;}
  if (toggle_btn.press()){ send_msg(req_toggle_kicker,2); any_press=true;}
  if (jog_kicker_btn.press()){ send_msg(req_kicker_motor_speed,10);}
  if (jog_kicker_btn.release()){ send_msg(req_kicker_motor_speed,0);}
  if (cut_btn.press()){ send_msg(req_cut); any_press=true;}
  if (pedal_btn.press()){ send_msg(req_spool_motor_speed,10); any_press=true;}
  if (pedal_btn.release()){ send_msg(req_spool_motor_speed,0); any_press=true;}

  long cur_knob = knob.read();
  if (cur_knob != knob_val){
    // do stuff based on turning the encoder
    send_msg(req_dist_delta, KNOB_ENC_P*(cur_knob - knob_val));
    knob_val = cur_knob;
  }
  return any_press;
}

void update_btns(){
  start_btn.update();
  stop_btn.update();
  zero_btn.update();
  toggle_btn.update();
  jog_kicker_btn.update();
  cut_btn.update();
  pedal_btn.update();
}

void update_lights(){
  zero_light.update();
  start_light.update();
  stop_light.update();
  cut_light.update();
  toggle_light.update();
}

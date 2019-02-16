
bool sub_state_change = true;
enum sub_states{
  set_env,
  move_len,
  cut,
  activate_brakes,
  idle
};
int sub_state;

int change_sub_state(int state){
  sub_state_change = true;
  return state;
}

long t_done = 0;
int settle_time = 50;

bool stop(){
  M1.setSpeed(0);
  M2.setSpeed(0);
  send_msg("STOP");
  sub_state = idle;
  return true;
}

long wire_check_t;
long wire_check_enc;
int get_stall_time(){
  // return time since last movement (ms)
  long current_reading = mainEnc.read();
  if (sub_state_change){
    // first reading
    sub_state_change = false;
    wire_check_t = millis();
    wire_check_enc = current_reading;
    return 0; // assume no time since last reading
  }
  // check for movement
  long t_now = millis();
  if (current_reading != wire_check_enc){
    // sensed movement
    wire_check_t = t_now;
    wire_check_enc = current_reading;
    return 0;
  }
  int stall_t = t_now - wire_check_t;
  if (stall_t > 2000){
    stop();
    send_msg(throw_err, 103);
    return -1; // return error, stalled too long
  }
  return stall_t;
}

bool wire_control(DCmotor &motor, int offset){
  long int current_reading = mainEnc.read();
  long int err = motor.MotorControl(target, current_reading, offset);
  if (err > TICKS_THRESHOLD){
    t_done = millis();
    return false;
  }
  else{
    motor.setSpeed(0);
    if (millis()-t_done > settle_time){
      return true;
    }
    return false;
  }
}

void reset_encoder(){
  mainEnc.write(0);
  encoder_zeroed = true;
}

bool cut_done = true;
int debounce_count = 0;

bool cut_wire(){
  if (sub_state_change){
    analogWrite(CUT_MOTOR_PIN, 255);
    sub_state_change = false;
    send_msg("CUT_START");
  }
  bool cur_val = !digitalRead(CUT_DONE_PIN);
  if (cur_val != cut_done){
    debounce_count ++;
    if (debounce_count < 5){
      cur_val = cut_done;
    }
    else debounce_count = 0;
  }
  else debounce_count=0;
  if (cut_done && !cur_val) cut_done=false;
  if (!cut_done && cur_val){
    analogWrite(CUT_MOTOR_PIN, 0);
    cut_done = true;
    reset_encoder();
    send_msg("CUT_DONE");
    return true;
  }
  return false;
}

long brake_start_t;

bool activate_brake(){
  if (sub_state_change){
    digitalWrite(BRAKE_PIN, HIGH);
    brake_start_t = millis();
    //sub_state_change = false;
  }
  if (millis() - brake_start_t > BRAKE_TIME){
    digitalWrite(BRAKE_PIN, LOW);
    return true;
  }
}

long kicker_start_t;

bool release_kicker(){
  if (!kicker_active) return true;
  if (sub_state_change){
    analogWrite(KICKER_SERVO_PIN, KICKER_POS_OPEN);
    kicker_start_t = millis();
    sub_state_change = false;
  }
  if (millis() - kicker_start_t > KICKER_SERVO_TIME){
    kicker_active = false;
    return true;
  }
  return false;
}

bool activate_kicker(){
  if (kicker_active) return true;
  if (sub_state_change){
    analogWrite(KICKER_SERVO_PIN, KICKER_POS_CLOSED);
    kicker_start_t = millis();
    sub_state_change = false;
  }
  if (millis() - kicker_start_t > KICKER_SERVO_TIME){
    kicker_active = true;
    return true;
  }
  return false;
}

bool toggle_kicker(int type){
  if (kicker_active && (type==0 || type==2)){
    analogWrite(KICKER_SERVO_PIN, KICKER_POS_OPEN);
    kicker_active = false;
  }
  else if (!kicker_active && (type==1 or type==2)) {
    analogWrite(KICKER_SERVO_PIN, KICKER_POS_CLOSED);
    kicker_active = true;
  }
  return kicker_active;
}

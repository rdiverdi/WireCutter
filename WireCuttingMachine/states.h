bool state_change = true;
enum states {
  manual,
  noSpool,
  spool
};
int state = manual;

int change_state(int state){
  state_change = true;
  return state;
}

bool dist_control = false;
long man_target = 0;
long dist_start_t = 0;

int manual_control(cmd_msg msg){
  if (state_change){
    // state setup code
    send_msg("MANUAL");
    state_change = false;
  }
  switch (msg.type){
  case req_start:
    // start button
    if (use_spool) return change_state(spool);
    return change_state(noSpool);
    break;
  case req_target_len:
    send_msg(req_target_len,msg.msg);
    // set target length
    target = msg.msg;
    break;
  case req_encoder_zero:
    send_msg(req_encoder_zero);
    // zero encoder count
    reset_encoder();
    break;
  case req_toggle_kicker:
    // toggle kicker motor
    send_msg(req_toggle_kicker, int(toggle_kicker(msg.msg)));
    break;
  case req_kicker_motor_speed:
    send_msg(req_kicker_motor_speed, msg.msg);
    // run kicker motor
    M1.setSpeed(msg.msg);
    break;
  case req_spool_motor_speed:
    send_msg(req_spool_motor_speed, msg.msg);
    // run spool motor
    M2.setSpeed(msg.msg);
    break;
  case req_spool:
    send_msg(req_spool, msg.msg);
    use_spool = (bool)msg.msg;
    break;
  case req_num_wires:
    send_msg(req_num_wires, msg.msg);
    //set desired number
    desired_num = msg.msg;
    break;
  case req_dist:
    if (M1.Speed() == 0 || dist_control){
      send_msg(req_dist, msg.msg);
      dist_control = true;
      man_target = msg.msg;
      dist_start_t = millis();
    }
    break;
  case req_dist_delta:
    if (M1.Speed() == 0 || dist_control){
      send_msg(req_dist_delta, msg.msg);
      if (!dist_control){
        man_target = mainEnc.read();
      }
      dist_control = true;
      man_target += msg.msg;
      dist_start_t = millis();
    }
    break;
  case req_enc_val:
    send_msg(req_enc_val, mainEnc.read());
    break;
  case req_cut:
    sub_state = change_sub_state(cut);
    break;
  case req_stop:
    stop();
    break;
  default:
    break;
  };
  if (dist_control){
    long cur_dist = mainEnc.read();
    long err = M1.MotorControl(man_target, cur_dist);
    if (millis()-dist_start_t > 500 || err<TICKS_THRESHOLD){
      M1.setSpeed(0);
      dist_control = false;
    }
  }
  if (sub_state == cut){
    if (cut_wire()){
      sub_state = idle;
    }
  }
  return manual;
}


int noSpool_control(cmd_msg msg) {
  if (!encoder_zeroed){
    stop();
    send_msg(throw_err, 101);
    return manual;
  }
  if (msg.type == req_stop){
    stop();
    return manual;
  }
  if (state_change){
    count = 0;
    M1.MotorControlReset();
    long int current_reading = mainEnc.read();
    long int err = target-current_reading;
    if (err < 0){
      send_msg(throw_err, 102);
      return manual;
    }
    send_msg("START_NO_SPOOL");
    sub_state = change_sub_state(set_env);
    state_change = false;
  }
  switch (sub_state){
  case set_env:
    if (activate_kicker()){
      sub_state = change_sub_state(move_len);
    }
    break;
  case move_len:
    if (!check_for_movement()){
      return manual;
    }
    if (wire_control(M1)){
      sub_state = change_sub_state(cut);
      send_msg(req_enc_val, mainEnc.read());
    }
    break;
  case cut:
    if (cut_wire()){
      sub_state = change_sub_state(move_len);
      count ++;
      if (count >= desired_num){
        send_msg("DONE");
        return change_state(manual);
      }
    }
    break;
  };
  return noSpool;
}

int spool_control(cmd_msg msg) {
  if (!encoder_zeroed){
    stop();
    send_msg(throw_err, 101);
    return manual;
  }
  if (msg.type == req_stop){
    stop();
    return manual;
  }
  if (state_change){
    count = 0;
    M2.MotorControlReset();
    long int current_reading = mainEnc.read();
    long int err = target-current_reading;
    if (err < 0){
      send_msg(throw_err, 102);
      return manual;
    }
    send_msg("START_SPOOL");
    sub_state = change_sub_state(set_env);
    state_change = false;
  }
  switch (sub_state){
  case set_env:
    if (release_kicker()){
      sub_state = change_sub_state(move_len);
    }
    break;
  case move_len:
    if (!check_for_movement()){
      return manual;
    }
    if (wire_control(M2)){
      sub_state = change_sub_state(activate_brakes);
    }
    break;
  case activate_brakes:
    bool brake_check;
    bool kicker_check;
    brake_check = activate_brake();
    kicker_check = activate_kicker();
    if (brake_check && kicker_check){
      sub_state = change_sub_state(cut);
    }
    break;
  case cut:
    if (cut_wire()){
      sub_state = change_sub_state(move_len);
      count ++;
      if (count >= desired_num){
        send_msg("DONE");
      }
      return change_state(manual);
    }
    break;
  };
  return spool;
}

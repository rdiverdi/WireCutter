
struct cmd_msg{
  int type;
  long int msg;
};

String cmd_string = "";
int index=0;

enum cmds{
  req_start,
  req_target_len,
  req_encoder_zero,
  req_toggle_kicker,
  req_kicker_motor_speed,
  req_spool_motor_speed,
  req_spool,
  req_num_wires,
  req_dist,
  req_dist_delta,
  req_enc_val,
  req_stop,
  throw_err,
  req_cut,
  // states
  manual,
  start_spool,
  start_no_spool,
  cut_start,
  cut_done,
  done
};

int get_msg_val(String msg_type){
  if (msg_type == "START") return req_start;
  if (msg_type == "SET_LEN") return req_target_len;
  if (msg_type == "ZERO_ENC") return req_encoder_zero;
  if (msg_type == "TOGGLE_KICK") return req_toggle_kicker;
  if (msg_type == "SET_KICK_SPEED") return req_kicker_motor_speed;
  if (msg_type == "SET_SPOOL_SPEED") return req_spool_motor_speed;
  if (msg_type == "USE_SPOOL") return req_spool;
  if (msg_type == "SET_NUM_WIRES") return req_num_wires;
  if (msg_type == "MOVE_TO") return req_dist;
  if (msg_type == "MOVE_BY") return req_dist_delta;
  if (msg_type == "ENC_VAL") return req_enc_val;
  if (msg_type == "STOP") return req_stop;
  if (msg_type == "ERR") return throw_err;
  if (msg_type == "CUT") return req_cut;
  // STATES
  if (msg_type == "MANUAL") return manual;
  if (msg_type == "START_SPOOL") return start_spool;
  if (msg_type == "START_NO_SPOOL") return start_no_spool;
  if (msg_type == "CUT_START") return cut_start;
  if (msg_type == "CUT_DONE") return cut_done;
  if (msg_type == "DONE") return done;
  return 50;
}

String get_msg_string(int msg_type){
  String rsp;
  if (msg_type == req_start) rsp = ("START,");
  if (msg_type == req_target_len) rsp = ("SET_LEN,");
  if (msg_type == req_encoder_zero) rsp = ("ZERO_ENC,");
  if (msg_type == req_toggle_kicker) rsp = ("TOGGLE_KICK,");
  if (msg_type == req_kicker_motor_speed) rsp = ("SET_KICK_SPEED,");
  if (msg_type == req_spool_motor_speed) rsp = ("SET_SPOOL_SPEED,");
  if (msg_type == req_spool) rsp = ("USE_SPOOL,");
  if (msg_type == req_num_wires) rsp = ("SET_NUM_WIRES,");
  if (msg_type == req_dist) rsp = ("MOVE_TO,");
  if (msg_type == req_dist_delta) rsp = ("MOVE_BY,");
  if (msg_type == req_enc_val) rsp = ("ENC_VAL,");
  if (msg_type == req_stop) rsp = ("STOP,");
  if (msg_type == throw_err) rsp = ("ERR,");
  if (msg_type == req_cut) rsp = ("CUT,");
  return rsp;
}

struct cmd_msg get_msg(){
  cmd_msg msg;
  msg.type = 50;
  msg.msg = 0;
  while (Serial.available() > 0){
    char in_char = Serial.read();
    if (in_char != '\n'){
      cmd_string.concat(in_char);
    }
    else{
      int split1 = cmd_string.indexOf(',');
      String msg_type = cmd_string.substring(0, split1);
      msg.type = get_msg_val(msg_type);
      msg.msg = cmd_string.substring(split1+1).toInt();
      //Serial.println(msg.msg);
      cmd_string = "";
    }
  }
  return msg;
}

void send_msg(int rsp_msg_type, long val){
  Serial.print(get_msg_string(rsp_msg_type));
  Serial.print(val);
  Serial.print('\n');
}

void send_msg(int rsp_msg_type){
  Serial.print(get_msg_string(rsp_msg_type));
  Serial.print('1');
  Serial.print('\n');
}

void send_msg(String rsp_msg){
  Serial.print(rsp_msg);
  Serial.print(",1");
  Serial.print('\n');
}

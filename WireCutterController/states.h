#ifndef state_manager
#define state_manager

bool state_change = true;
enum states {
  main_screen,
  edit_units,
  edit_len,
  edit_use_spool,
  edit_num,
  err
};
int state=main_screen;
bool edit = false;

void change_state(int new_state){
  state = new_state;
  state_change = true;
  edit = false;
}

void cycle_state() {
  state_change = true;
  switch (state){
    case main_screen:
      state = edit_units;
      break;
    case edit_units:
      state = edit_len;
      break;
    case edit_len:
      state = edit_use_spool;
      break;
    case edit_use_spool:
      state = edit_num;
      break;
    default:
      state = main_screen;
  };
}

#endif

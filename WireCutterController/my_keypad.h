#include "src/i2cKeypad1/i2cKeypad1.h"

class my_Keypad{
public:
  my_Keypad(){}

  void setup(){
    display.setup();
  }

  char read_keypad(){
     return _keypad.getKey();
  }

  bool update(bool external_btn){
    display.update();
    if (state_change){
      display.clear();
      state_change = false;
    }
    char input = read_keypad();
    switch (state){
      case main_screen:
        if (input=='m') change_state(edit_units);
        break;
      case edit_units:
        if (input=='m' && !edit) change_state(edit_len);
        else if (input=='m' && edit) edit=false;
        else if (input=='\n' && edit){ edit=!edit; change_state(main_screen); }
        else if (input=='\n') {edit=!edit; display.clear();}
        else if (edit){
          if (input=='1'){ change_units(in); edit=false;}
          if (input=='2'){ change_units(ft); edit=false;}
          if (input=='3'){ change_units(mm); edit=false;}
          if (input=='4'){ change_units(cm); edit=false;}
          if (input=='5'){ change_units(m); edit=false;}
        }
        break;

      case edit_len:
        if (input=='m' && !edit) change_state(edit_use_spool);
        else if ((input=='m' || input=='\n')  && edit){
          edit=false;
          length = len_to_ticks(keypresses.toFloat(), units);
          display.set_len(ticks_to_len(length, units));
          send_msg(req_target_len, length);

          use_spool = (length > len_to_ticks(SPOOL_CUTOFF_LEN, in));
          send_msg(req_spool, int(use_spool));
          display.set_spool(use_spool);

          if (input=='\n') change_state(main_screen);
        }
        else if (input=='\n' && !edit){
          edit=true;
          keypresses = "";
          display.clear();
          display.set_len_str("0");
        }
        else if (edit && input && input!='\n'){
          if (input=='\b') keypresses.remove(keypresses.length()-1);
          else keypresses.concat(input);
          display.set_len_str(keypresses);
        }
        break;

      case edit_use_spool:
        if (input == 'm') change_state(main_screen);
        if (input == '\n'){
          use_spool = !use_spool;
          send_msg(req_spool, int(use_spool));
          display.set_spool(use_spool);
          display.clear();
        }
        break;

      default:
        if (input == 'm' || input == '\n' || external_btn){
          change_state(main_screen);
        }
        break;
    }
    return bool(input);
  }

  void change_units(int new_units){
    units = new_units;
    display.set_units(units);
    display.set_len(ticks_to_len(length, units));
    display.clear();
  }

  void set_err(int err){
    display.set_err(err);
  }

private:
  //define the cymbols on the buttons of the keypads
  char hexaKeys[4][4] = {
    {'1','2','3','\n'},
    {'4','5','6','m'},
    {'7','8','9',' '},
    {'.','0','\b',' '}
  };
  byte rowPins[4] = {3, 4, 5, 6}; //connect to the row pinouts of the keypad
  byte colPins[4] = {0, 1, 2, 7}; //connect to the column pinouts of the keypad

  //initialize an instance of class NewKeypad
  Keypad _keypad = Keypad(byte(0x38), makeKeymap(hexaKeys), rowPins, colPins, 4, 4);

  Display display = Display();
};

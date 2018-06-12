#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>
#include "states.h"


class Display{
public:
  Display(){
    last_time = millis();
    _blink = true;
  }
  void setup(){
    _lcd.begin(16, 2);
    _lcd.print("startup");
  }

  void write(String line1, String line2){
    _lcd.setCursor(0,0);
    _lcd.print(line1);
    _lcd.setCursor(0,1);
    _lcd.print(line2);
  }

  void clear(){
    _lcd.clear();
    _full_line1 = "";
    _full_line2 = "";
  }

  void set_len(float len){
    _length = String(len);
  }
  void set_len_str(String len){
    _length = len;
  }
  void set_spool(bool use_spool){
    if (use_spool) _use_spool = "SPOOL";
    else _use_spool = "NoSPOOL";
  }
  void set_copies(int copies){
    _copies = String(copies) + "x";
  }

  void set_units(int units){
    switch (units){
    case mm:
      _units = "mm";
      break;
    case cm:
      _units = "cm";
      break;
    case m:
      _units = "m";
      break;
    case in:
      _units = "in";
      break;
    case ft:
      _units = "ft";
      break;
    }
  }

  String get_main_line_1(){
    String line="LEN(";
    line.concat(_units);
    line.concat("):");
    for (int i=(line.length()+_use_spool.length()); i<16; i++){
      line.concat(" ");
    }
    line.concat(_use_spool);
    return line;
  }
  String get_main_line_2(){
    String line=_length;
    for (int i=(line.length()+_copies.length()); i<16; i++){
      line.concat(" ");
    }
    line.concat(_copies);
    return line;
  }

  void set_err(int err){
    _err = err;
  }
  String get_err_text(){
    switch(_err){
      case 101:
        return "LEN NOT ZEROED";
      case 102:
        return "WIRE TOO LONG";
      case 103:
        return "OUT OF WIRE";
      default:
        return "UNKNOWN";
    };
  }

  void update(){
    long dt = millis()-last_time;
    if ((_blink && (dt > 200)) || dt > 600){
      last_time = millis();
      _blink = !_blink;
    }

    String full_line1;
    String full_line2;
    switch (state){
      case main_screen:
        full_line1 = get_main_line_1();
        full_line2 = get_main_line_2();
        break;
      case edit_units:
        if (edit){
          full_line1 = "Units: 1:in 2:ft";
          full_line2 = "3:mm 4:cm 5:m";
        }
        else{
          full_line1 = "Set Units";
          full_line2 = "current:  (";
          full_line2.concat(_units);
          full_line2.concat(")");
        }
        break;
      case edit_len:
        full_line1 = "Set Length (";
        full_line1.concat(_units);
        full_line1.concat(")");
        if (edit && _blink){
          full_line2 = "               ";
        }
        else{
          full_line2 = _length;
          full_line2.concat("      ");
        }
        break;
      case edit_use_spool:
        full_line1 = "Use Spool:  ";
        if (_use_spool == "SPOOL") full_line1.concat("yes");
        else full_line1.concat("no");
        full_line2 = "ENT to change";
        break;
      case edit_num:
        full_line1 = "Num Copies:";
        full_line2 = _copies;
      case err:
        full_line1 = "ERR ";
        full_line1.concat(_err);
        full_line1.concat(":");
        full_line2 = get_err_text();
    };

    if (full_line1 != _full_line1 || full_line2 != _full_line2){
      write(full_line1, full_line2);
      _full_line1 = full_line1;
      _full_line2 = full_line2;
    }
  }

private:
  Adafruit_LiquidCrystal _lcd = Adafruit_LiquidCrystal(0);
  String _length = "0";
  String _use_spool = "NoSPOOL";
  String _units = "in";
  String _copies = "1x";
  String _full_line1 = "";
  String _full_line2 = "";
  int _err = 0;
  long last_time;
  bool _blink;
};

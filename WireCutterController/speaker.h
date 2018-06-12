#include "pitches.h"

class Speaker{

public:

  Speaker(int pin){
    _pin = pin;
  }

  void update() {
    if (_this_note==1){
      return;
    }
    long now = millis();
    if (now - _start_t > _noteDuration){
      _i ++;
      _this_note = _notes[_i];
      if (_this_note == 1){
        noTone(_pin);
        return;
      }
      _noteDuration = 1000/_durations[_i];
      _start_t = now;
      tone(_pin, _this_note);
    }
  }

  void set_tone(int tones[], int durations[]){
    _notes = tones;
    _durations = durations;
    _i = 0;
    _start_t = millis();
    _this_note = _notes[_i];
    _noteDuration = 1000/_durations[_i];
    tone(_pin, _notes[_i]);
  }

  void done_tone(){
    set_tone(done, done_durations);
  }

  void err_tone(){
    set_tone(err, err_durations);
  }

  void press_tone(){
    set_tone(press, press_durations);
  }

private:
  const int done[5] = { NOTE_E4, NOTE_F4, NOTE_G4, NOTE_B4, 1 };
  const int done_durations[4] = { 8, 8, 8, 4 };

  const int err[2] = { NOTE_C3, 1 };
  const int err_durations[1] = { 4 };

  const int press[2] = { NOTE_A4, 1 };
  const int press_durations[1] = { 16 };

  int* _notes;
  int* _durations;
  int _i = 0;
  int _this_note = 1;
  int _noteDuration;
  long _start_t;

  int _pin;


  void slide(int start_note, int end_note, long time_ms){
    long start_t = micros();
    long now = start_t;
    while (now-start_t < 1000*time_ms){
      tone(8, (end_note-start_note)*(now-start_t)/(1000*time_ms) + start_note);
      now = micros();
      delay(1);
    }
  }
};

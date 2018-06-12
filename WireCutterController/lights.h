class LED{

public:
  LED(int pin){
    _pin = pin;
    pinMode(_pin, OUTPUT);
    _last_t = millis();
  }

  void update(){
    if (_on) return;
    long t_now = millis();
    if (_fast_blink && t_now - _last_t > _fast_blink_t){
      _active = !_active;
      _last_t = t_now;
      digitalWrite(_pin, _active);
    }
    if (_blink && t_now - _last_t > 3*_fast_blink_t){
      _active = !_active;
      _last_t = t_now;
      digitalWrite(_pin, _active);
    }
    if (_slow_blink && t_now - _last_t > 6*_fast_blink_t){
      _active = !_active;
      _last_t = t_now;
      digitalWrite(_pin, _active);
    }
    if (_timed && t_now >_off_t){
      off();
      _timed = false;
    }
  }

  void reset_blink(){
    _fast_blink = false;
    _slow_blink = false;
    _blink = false;
    _timed = false;
  }

  void on(){
    reset_blink();
    _active = true;
    digitalWrite(_pin, _active);
  }

  void off(){
    reset_blink();
    _active = false;
    digitalWrite(_pin, _active);
  }

  void blink(){
    reset_blink();
    _blink = true;
  }

  void fast_blink(){
    reset_blink();
    _fast_blink = true;
  }

  void slow_blink(){
    reset_blink();
    _slow_blink = true;
  }

  void blink_time(int time){
    fast_blink();
    _off_t = millis()+time;
    _timed = true;
  }

private:
  int _pin;
  bool _on = false;
  bool _fast_blink = false;
  bool _blink = false;
  bool _slow_blink = false;
  bool _active = false;
  int _fast_blink_t = 100;
  long _last_t;
  long _off_t = 0;
  bool _timed = false;
};

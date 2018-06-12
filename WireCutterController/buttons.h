class Button{

public:
  Button(int pin){
    _pin = pin;
    _count = 0;
    pinMode(_pin, INPUT_PULLUP);
  }

  int threshold = 2;

  void update(){
    bool cur_read = !digitalRead(_pin);
    _press = false;
    _release = false;
    if (cur_read != _reading){
      _count ++;
      if (_count >= threshold){
        _reading = cur_read;
        _count = 0;
        if (_reading) _press = true;
        else _release = true;
      }
    }
    else{
      _count = 0;
    }
  }

  bool read(){
    return _reading;
  }

  bool press(){
    return _press;
  }

  bool release(){
    return _release;
  }

private:
  int _pin;
  bool _reading;
  bool _press = false;
  bool _release = false;
  int _count;
};

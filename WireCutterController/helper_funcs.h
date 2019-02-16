enum unit_types{
  mm,
  cm,
  m,
  in,
  ft
};

long len_to_ticks(float len, int units){
  if (units==mm) len = len/25.4;
  if (units==cm) len = len/2.54;
  if (units==m) len = len/0.0254;
  if (units==ft) len = len*12;
len=len+0.04;
  float revs = len/(3.1416*ENC_WHEEL_D);
  return revs*TICKS_PER_REV;
}

float ticks_to_len(long ticks, int units){
  float revs = ticks/float(TICKS_PER_REV);
  float len = (3.1416*ENC_WHEEL_D)*revs;
len=len-0.04;
  if (units==mm) len = len*25.4;
  if (units==cm) len = len*2.54;
  if (units==m) len = len*0.0254;
  if (units==ft) len = len/12;
  return len;
}

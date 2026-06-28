int leds[16] = {
  1,2,3,4,
  5,6,7,8,
  9,10,11,12,
  A0,A1,A2,A3
};

void setup() {
  for(int i=0;i<16;i++)
    pinMode(leds[i], OUTPUT);
}

void loop() {

  int M[] = {0,3,4,5,6,7,8,9,10,11,12,15};
  int C[] = {0,1,2,3,4,8,12,13,14,15};

  for(int i=0;i<12;i++)
    digitalWrite(leds[M[i]], HIGH);

  delay(2000);

  for(int i=0;i<12;i++)
    digitalWrite(leds[M[i]], LOW);

  delay(2000);

  for(int i=0;i<11;i++)
    digitalWrite(leds[C[i]], HIGH);

  delay(2000);

  for(int i=0;i<11;i++)
    digitalWrite(leds[C[i]], LOW);

  delay(2000);
}
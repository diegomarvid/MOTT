#include <MOTT.h>

MOTT mott;

const int RX_SIGNAL_PIN = 8;


void setup() {
  Serial.begin(9600);

  pinMode(RX_SIGNAL_PIN, INPUT);

  mott.SetRxPin(RX_SIGNAL_PIN);
  mott.SetBitTime(0.080L, Callback);

  mott.EnableSampling();

  Serial.println("-------------------------");
  Serial.println("Ready to detect signal...");

}

void Callback()
{
  mott.HandleInterrupt();
}


char string[127];

void loop() {
  
  if(mott.SamplingEnded())
  {
    mott.ObtainSample(string);
    Serial.println(string);
    mott.ResumeSampling();
  } 

}

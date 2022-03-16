#include <MOTT.h>

const int TX_SIGNAL_PIN = 8;

MOTT mott;

void setup() {
  Serial.begin(9600);

  pinMode(TX_SIGNAL_PIN, OUTPUT);

  mott.SetTxPin(TX_SIGNAL_PIN);
  mott.SetBitTime(0.080L, Callback);



  Serial.println("Loading up...");

  Serial.println("Sending signal in 3 seconds");
  delay(3000);

  const char* msg = "Hello world!";

  Serial.println(msg);
  mott.SendSignal(msg); 

}

void Callback()
{
  mott.HandleInterrupt();
}


void loop() {

  
 
}

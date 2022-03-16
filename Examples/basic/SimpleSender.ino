#include <MOTT.h>

MOTT mott;

const int TX_SIGNAL_PIN = 7;
const int RX_SIGNAL_PIN = 8;

void setup() {
  Serial.begin(9600);

  mott.SetTxPin(TX_SIGNAL_PIN);
  mott.SetRxPin(RX_SIGNAL_PIN);

  mott.setCarrierFrequencyKHz(38.0)
  mott.SetBitTime(1.0L, Callback);

}

void Callback()
{
  mott.HandleInterrupt();
}


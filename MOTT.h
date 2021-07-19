#ifndef MOTT_h
#define MOTT_h

#include "Arduino.h"
#include <TimerOne.h>


class MOTT
{
	static unsigned const long ms = 1000L;
	static unsigned const int SIGNAL_MAX_SIZE = 7*50;
	
	unsigned long TIMER_TIME;
	unsigned long TIMER_TIME_IN_MS;

	int SIGNAL_SIZE;
	bool signal[SIGNAL_MAX_SIZE];
	
	int TX_SIGNAL_PIN;
	int RX_SIGNAL_PIN;
	
	unsigned int i;
	
	bool sending;
	
	//---------------------//
	int largo;
	bool reading_signal;
	bool reading_ended;
	bool samplingEnable;
  
  private:
	char ConvertBoolArrayToChar(bool arr[]);
    void ConvertCharToBoolArray(char c, bool* newSignal);
	int CreateBitSignalFromCharArray(char* string);
	void SplitSignalIntoArrays(char* string);
	bool IsSignalDetected(int sample);
	void WaitForSignalEndingAfterInterrupt(char* string);
	void BeginSamplingTimer();
	void SendBit();
	void ReadBit();
	
  public:
    MOTT();    
	void SetTxPin(int pin);
	void SetRxPin(int pin);
	void SetBitTime(unsigned long time_in_ms, void (*f)());
	void SendSignal(char* string);
	void SampleSignalIfDetected();
	void ObtainSample(char *string);
	void ResumeSampling();
	bool SamplingEnded();
	void EnableSampling();
	void DisableSampling();
	void HandleInterrupt();

	
	
};

#endif
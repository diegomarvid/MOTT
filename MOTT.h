#ifndef MOTT_h
#define MOTT_h

#include "Arduino.h"

class MOTT
{
	static unsigned const long ms = 1000L;
	static unsigned const int SIGNAL_MAX_SIZE = 7*50;
	static unsigned const long CARRIER_TIME = 13L;

	hw_timer_t * timer = NULL;

	void (*callback)();
	int time_in_ms;
	unsigned long tick_count;
	
	unsigned long TIMER_TIME;
	unsigned long CARRIER_CYCLES;
	unsigned long counter;
	
	int SIGNAL_SIZE;
	bool signal[SIGNAL_MAX_SIZE];
	
	int TX_SIGNAL_PIN;
	int RX_SIGNAL_PIN;
	
	unsigned int i;
	
	bool sending;
	bool output;
	
	//---------------------//
	int largo;
	bool reading_signal;
	bool reading_ended;
	bool samplingEnable;
  
  private:
  	int RoundFloatToInt(float number);
	char ConvertBoolArrayToChar(bool arr[]);
    void ConvertCharToBoolArray(char c, bool* newSignal);
	int CreateBitSignalFromCharArray(const char* string);
	void SplitSignalIntoArrays(char* string);
	bool IsSignalDetected(int sample);
	void WaitForSignalEndingAfterInterrupt(char* string);
	void BeginSamplingTimer();
	void SendBit();
	void ReadBit();
	void StartTimer();
	void EndTimer();
	
  public:
    MOTT();    
	void SetTxPin(int pin);
	void SetRxPin(int pin);
	void SetBitTime(double time_in_ms, void (*f)());
	void SendSignal(const char* string);
	void SampleSignalIfDetected();
	void ObtainSample(char *string);
	void ResumeSampling();
	bool SamplingEnded();
	void EnableSampling();
	void DisableSampling();
	void HandleInterrupt();

	
	
};

#endif
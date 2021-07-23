#include "Arduino.h"
#include "MOTT.h"


MOTT::MOTT()
{
	TIMER_TIME = 1L*ms;
	CARRIER_CYCLES = TIMER_TIME / CARRIER_TIME;

	SIGNAL_SIZE = SIGNAL_MAX_SIZE;
	
	RX_SIGNAL_PIN = 7;
	TX_SIGNAL_PIN = 8;
	
	i = 0;
	sending = false;
	output = true;
	counter = 0;
	
	largo = 0;
	reading_signal = false;
	reading_ended = true;
	samplingEnable = false;
}

void MOTT::SetTxPin(int pin)
{
	TX_SIGNAL_PIN = pin;
}

void MOTT::SetRxPin(int pin)
{
	RX_SIGNAL_PIN = pin;
}

void MOTT::HandleInterrupt()
{
	if(sending) {
		SendBit();
	}else if(reading_signal){
		ReadBit();
	}
	
}

int MOTT::CreateBitSignalFromCharArray(char* string)
{
	
  int len = strlen(string);
	
  int BitLength = (len+1)*7 + 1;

  bool auxiliar_array[7];

  //Flag para recepcion
  signal[0] = 1;

  //Codificar el largo del payload en bits
  ConvertCharToBoolArray((char) len, auxiliar_array);
  
  for(int i = 1; i < 8; i++){
    signal[i] = auxiliar_array[i-1];
  }

  //Codificar el payload en bits
  for(int h = 1; h <= len; h++)
  {
    ConvertCharToBoolArray((char) string[h-1], auxiliar_array);

    for(int j = h*7 + 1; j < h*7 + 8; j++)
    {
      signal[j] = auxiliar_array[j - h*7 - 1];
    }   
  }
  
  SIGNAL_SIZE = BitLength;
  
  return BitLength;
}

void MOTT::SendSignal(char* string)
{
	if(sending == true || reading_signal == true) return;
	
	CreateBitSignalFromCharArray(string);
	sending = true;
	Timer1.restart();
}

void MOTT::SendBit()
{
	
	if(i < SIGNAL_SIZE)
	{
		if(signal[i] == 1)
		{
		  digitalWrite(TX_SIGNAL_PIN,output);
		  //Serial.print(output);
		  output = !output;
		}else{
		  //Serial.print(0);
		  if(counter == 0){
			digitalWrite(TX_SIGNAL_PIN, 0);
		  }
		}
    }

    if(counter == CARRIER_CYCLES - 1)
    {
      counter = 0;
      i++;
      Serial.println("");
    } else{
      counter++;
    }
	
	 if(i == SIGNAL_SIZE) 
  {
      sending = false;
      i = 0;
      //Serial.println("Dejo de enviar");
      digitalWrite(TX_SIGNAL_PIN,0);
      Timer1.stop();
      
  } 
	
  
}

void MOTT::SetBitTime(double time_in_ms, void (*f)())
{
  Timer1.attachInterrupt(f) ;
  
  TIMER_TIME = time_in_ms * ms;
  CARRIER_CYCLES = TIMER_TIME / CARRIER_TIME; 
  
  Serial.print("Velocidad: ");
  Serial.print(TIMER_TIME);
  Serial.println(" us/bit");
  
  Serial.print("Carrier cycles: ");
  Serial.println(CARRIER_CYCLES);

  Timer1.initialize(CARRIER_TIME);  

  Timer1.stop();
}


void MOTT::ConvertCharToBoolArray(char c, bool newSignal[])
{
  for(int i = 0; i < 7; i++)
  {
    newSignal[6-i] = (c >> i) & 0b1;
  }
} 

//----------Sampling-------------------------------//

char MOTT::ConvertBoolArrayToChar(bool arr[])
{

  char ByteSignal = arr[0] * 64;

  int h = 1;
  for(int j = 32; j > 0 ; j = j/2)
  {
    ByteSignal += arr[h] * j;
    h++;
  }

  return ByteSignal;

}

void MOTT::SplitSignalIntoArrays(char* string)
{

  bool auxiliar_array[7];

  //char string[largo];

  for(int h = 0; h < largo; h++)
  {
  
    for(int j = h*7; j < h*7 + 7; j++)
    {
      auxiliar_array[ j - h*7 ] = signal[j];
    }

    string[h] = ConvertBoolArrayToChar(auxiliar_array);

  }

  string[largo] = 0;
  //Serial.println(string);
  
}

void MOTT::ReadBit()
{
  if(i == 0)
  {
    largo = 0;
  }

  //Obtener el largo en bytes
  if(i > 0 && i < 8)
  {
    largo += (1 << (7 - i)) * digitalRead(RX_SIGNAL_PIN);
	//Serial.println(digitalRead(RX_SIGNAL_PIN));
  }

  //Setear la variable al largo en bits
  if(i == 8)
  {
    SIGNAL_SIZE = largo * 7;
  }

  //Leer el payload
  if(i >= 8)
  {
    signal[i-8] = digitalRead(RX_SIGNAL_PIN);
  }


  i++;

  //Finaliza la lectura
  if(i == SIGNAL_SIZE + 1 + 7)
  {
    reading_signal = false;
    reading_ended = false;

    SIGNAL_SIZE = SIGNAL_MAX_SIZE;
    i = 0;
    Timer1.stop();
  }
}

void MOTT::BeginSamplingTimer()
{

	if(sending == true) return;
	
	//Delay para muestrear en el centro del simbolo
	delayMicroseconds(TIMER_TIME / 4);
	Timer1.restart();  
	reading_signal = true;
}

void MOTT::SampleSignalIfDetected()
{

	if(!samplingEnable) return;

	if(reading_signal == false)
	{
		int sample = digitalRead(RX_SIGNAL_PIN);

		if(IsSignalDetected(sample))
		{
		  BeginSamplingTimer();
		}
       
  }
}

bool MOTT::IsSignalDetected(int sample)
{
  return sample == 1 && reading_ended;
}

void MOTT::WaitForSignalEndingAfterInterrupt(char* string)
{
   if(reading_ended == false)
  {
	Serial.println("Entro a devolver el sampling");

    SplitSignalIntoArrays(string);
    //Delay para asegurar de que el pin este quieto pasado la onda
    delayMicroseconds(TIMER_TIME);
    reading_ended = true;

  }
}

bool MOTT::SamplingEnded()
{
	SampleSignalIfDetected();
	
	return reading_ended == false;
}

void MOTT::ResumeSampling()
{
	//Delay para asegurar de que el pin este quieto pasado la onda
    delayMicroseconds(TIMER_TIME);
    reading_ended = true;
	reading_signal = false;

}

void MOTT::ObtainSample(char *string)
{
	SplitSignalIntoArrays(string);
}

void MOTT::EnableSampling()
{
	samplingEnable = true;
}

void MOTT::DisableSampling()
{
	samplingEnable = false;
}





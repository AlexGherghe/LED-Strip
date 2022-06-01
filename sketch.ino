#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "arduinoFFT.h"
 
#define SAMPLES 64             
#define SAMPLING_FREQUENCY 10000 

#define RED D0
#define GREEN D1
#define BLUE D2

#define _SSID "TP-Link_E590"
#define PASSWORD "*******"  
#define UDP_PORT 331 
 
arduinoFFT FFT = arduinoFFT();
WiFiUDP UDP;
 
unsigned int samplingPeriod;
unsigned long microSeconds;

bool isVisualizing = true;

double real[SAMPLES];
double imag[SAMPLES]; 
 
void setup() 
{
  WiFi.begin(_SSID, PASSWORD);
  long initTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - initTime < 15000)
  {
    delay(1000);
  }
  UDP.begin(UDP_PORT);
  
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); 
}
 
void loop() 
{  
  static char packet[15]; 
  static int packetSize;
  packetSize = UDP.parsePacket(); 
  if (packetSize) 
  { 
    int len = UDP.read(packet, 15); 
    if(len == 1)
    {
      if(packet[0] == '0')
        isVisualizing = false;
      else
        isVisualizing = true;
    }
    else if (!isVisualizing && len > 0)
    {
      packet[len] = '\0';
      char *p = strtok(packet, "/");
      analogWrite(RED, atoi(p));
      p = strtok(NULL, "/");
      analogWrite(GREEN, atoi(p));
      p = strtok(NULL, "/");
      analogWrite(BLUE, atoi(p));
    }
  }
  if(isVisualizing)
  {
    for(int i=0; i<SAMPLES; i++)
    {
        microSeconds = micros();   
        
        real[i] = analogRead(A0); 
        imag[i] = 0; 

        while(micros() < (microSeconds + samplingPeriod))
        {
        }
    }
 
    FFT.DCRemoval(real, SAMPLES);
    FFT.Windowing(real, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(real, imag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(real, imag, SAMPLES);

    int count = 0;
    double redVal = 0;
    double greenVal = 0; 
    double blueVal = 0;
    
    for (int i = 0; i < 2; i++) 
    {
      count++;
      redVal += real[i];
    }
    redVal = redVal / count;

    count = 0;
    for (int i = 2 ; i < 15; i++) 
    {
      count++;
      greenVal += real[i];
    }
    greenVal = greenVal / count;

    count = 0;
    for (int i = 15 ; i < 32; i++) 
    {
      count++;
      blueVal += real[i];
    }
    blueVal = blueVal / count;

    if (redVal > 900) {
      analogWrite(RED, map(redVal, 0, 1023, 0, 255));
      analogWrite(BLUE, 0);
      analogWrite(GREEN, 0);
    }
    else {
      analogWrite(RED, 0);
      analogWrite(BLUE, map(blueVal, 0, 1023, 0, 255));
      analogWrite(GREEN, map(greenVal, 0, 1023, 0, 255));
    }
  }
}

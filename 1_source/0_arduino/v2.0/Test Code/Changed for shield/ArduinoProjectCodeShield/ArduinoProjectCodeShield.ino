#include <SPI.h>
#include <CommunicationMedium.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>
#include <ArduinoInteraktiv.h>
#include <DistanceSRF04.h>

#define SENDER
//#define RECEIVER

CommunicationMedium Master;

String UltraSonicKey = "117";//u
String TriggerManualKey = "105";//i
String PiezoTriggerKey = "116";//t
String NoTriggerKey = "112";//p
String TriggerLaserKey = "108";//l
String LatestReceive = "";
bool MustTrigger = false;
DistanceSRF04 Dist;
int distance;

ArduinoInteraktiv camera; 

void setup()
{
  camera.Start(); 
  Serial.begin(57600);
  Master.init();  
  Dist.begin(4,5); //(echo, trigger)
}
void loop()
{
  #ifdef SENDER
    if(Serial.available()>0)
    {
      byte incomingByte = Serial.read();
      Master.SendData((String)incomingByte);      
    }
  #endif
  #ifdef RECEIVER
    String receipt = Master.RecieveData();
    if(receipt!="empty")
    {
      LatestReceive = receipt;   
      MustTrigger = true;    
    }
    //if loops
    if(LatestReceive == UltraSonicKey)
      triggerUltrasonic();
      
    else if(LatestReceive == TriggerManualKey)
    {
      triggerManual();
       
      }
      else if(LatestReceive == PiezoTriggerKey)
      triggerPiezo();
      else if (LatestReceive == TriggerLaserKey)
      triggerLaser();
    else if(LatestReceive == NoTriggerKey)
      noMoreTriggers(); 
  #endif
}
void triggerUltrasonic()
{
  distance = Dist.getDistanceCentimeter(); 
  if (distance <= 170)
  {
    camera.SinglePicture();
  }
}
void triggerManual()
{
  if(MustTrigger)
  {
  camera.SinglePicture();
  MustTrigger = false;
  }
}
void triggerPiezo()
{
  int a = analogRead(A0);
  if(a>=60)
  {
    camera.SinglePicture();
  }
}
void triggerLaser()
{
  //implement
  int a = analogRead(A1);
  if(a>50)
  {
    camera.SinglePicture();
  }
}
void noMoreTriggers()
{
  
}

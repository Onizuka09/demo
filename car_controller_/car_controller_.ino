// nrf24_reliable_datagram_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging client
// with the RHReliableDatagram class, using the RH_NRF24 driver to control a NRF24 radio.
// It is designed to work with the other example nrf24_reliable_datagram_server
// Tested on Uno with Sparkfun WRL-00691 NRF24L01 module
// Tested on Teensy with Sparkfun WRL-00691 NRF24L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
#define Yaxe A0
#define Xaxe A1

// Singleton instance of the radio driver
RH_NRF24 driver;
// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);
// declare unsigned 8-bit data array 
// 2 bytes  for motor speed and 1 byte for direction 
uint8_t data[3] ;
// Dont put this on the stack:
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
int x= 512 ; 
int y =512 ; 

void setup() 
{
  pinMode(Yaxe,INPUT);
  pinMode(Xaxe,INPUT); 
  Serial.begin(9600);
  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm

  data[2]= 0 ;// forward 
}

void loop()
{
  y = analogRead(Yaxe); // read y 
  x=analogRead(Xaxe); //read x 

if(y<460 ){
// move backward 
 data[2]=1 ; 
// set motor spped 
y=(y-460)*-1 ;// to reverse the readings 
 // set the speed 
 data[0]= map(y,0,460,0,255); 
 data[1]= map(y,0,460,0,255); 
}
else if ( y > 564 ){
// move forwards   
data[2] = 0 ;  
// set motors speed 
data[0]= map(y,564,1023,0,255); 
 data[1]= map(y, 564,1023,0,255); 
}
else {
  data[0]= 0  ;
 data[1]= 0 ;
}

if (x< 460 ) {
  // move left 
x=(x-460)*-1 ; 
x= map(x,0,460,0,255); 
data[0]=data[0]+x ; // set the right motor spped in the forward direction 
data [1] =data [1]-x ;// set the motor speed in th backward directon   
if (data [0] > 255 ) data[0]= 255 ; 
if (data [1]< 0) data [1]= 0 ; 
}
else if (x > 564 ){ 
  // move right 
x = map (x,546,1023,0,255);
data[0]=data[0]-x ; // set the right motor spped in the forward direction 
data [1] =data [1]+x ;// set the left motor speed in th backward directon   
if (data [1] > 255 ) data[1]= 255 ; 
if (data [0]< 0) data [0]= 0 ; 
 
  
}

if (data [1] < 8 ) data[1]= 0 ; 
if (data [0]< 8) data [0]= 0 ; 
  
   
  /*Serial.println("--------");
   Serial.print("Y:");
  Serial.println(data[0]);
  Serial.print("X:");
  Serial.println(data[1]);
  Serial.println("Sending to nrf24_reliable_datagram_server");*/
    
  // Send a message to manager_server
  if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      /*Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);*/
    }
    else
    {
      Serial.println("No reply, is nrf24_reliable_datagram_server running?");
    }
  }
  else
    Serial.println("sendtoWait failed");
  delay(9µ:!::::::::0);
}

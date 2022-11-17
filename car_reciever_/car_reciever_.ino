// nrf24_reliable_datagram_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging server
// with the RHReliableDatagram class, using the RH_NRF24 driver to control a NRF24 radio.
// It is designed to work with the other example nrf24_reliable_datagram_client
// Tested on Uno with Sparkfun WRL-00691 NRF24L01 module
// Tested on Teensy with Sparkfun WRL-00691 NRF24L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Singleton instance of the radio driver
RH_NRF24 driver;
// RH_NRF24 driver(8, 7);   // For RFM73 on Anarduino Mini

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

int ena = 9 ; 
int enb = 3 ;  
int in1 = 7 ;
int in2 = 6 ;
int in3 = 4 ;  
int in4 = 5 ;
void setup() 
{
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);    
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
}

uint8_t data[] = "data recieved ";
// Dont put this on the stack:
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
void backward(){
digitalWrite(in1,1);
digitalWrite(in2,0);

digitalWrite(in3,1);
digitalWrite(in4,0);}
void forward(){
digitalWrite(in1,0);
digitalWrite(in2,1);

digitalWrite(in3,0);
digitalWrite(in4,1);
  
}
void loop()
{
  if (manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
    /*  Serial.print("got request from : 0x");
           //Serial.print(from, HEX);
      //Serial.print(": MotorA = ");
      //Serial.print(buf[0]);
      //Serial.print(" MotorB = ");
      //Serial.print(buf[1]);
      //Serial.print(" Dir = ");
      //Serial.println(buf[2]);
      
      // Set Motor Direction*/
      if(buf[2]==1){
         backward();  
       
      }
     else {
      forward(); 
     }
     analogWrite(enb,buf[0]);     // speed motor right 
     analogWrite(ena,buf[1]);     // speed left motor  
      // Send a reply back to the originator client
      if (!manager.sendtoWait(data, sizeof(data), from))
        Serial.println("sendtoWait failed");
    }
  }
}

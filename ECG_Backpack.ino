/*
ECG Backpack Code

This Code collects ECG signals using a 3 electrode BITalino on the user's chest. The code takes an average reading over ten readins to produce a HR. The HR is compared to a threshold set by the user and feedback is given to the user through the Blufruit Application, and LED connected to the backpack. 

The Circuit:
 -Battery Connected to Adafruit Flora
 -ECG Extension of BITalino
 -Output to Adafruit Application using Bluetooth
 -LED Output when HR exceeds threshold
 
 Created Monday April 27, 2020
 By: Gabi Griffith and Liam Lilienthal
 */

//BLUETOOTH
#include<avr/power.h>
#include <Arduino.h>
#include "Adafruit_BLE.h" //extension for Bluefruit Component
#include "Adafruit_BluefruitLE_UART.h"// Bluetooth Communication
#include "BluefruitConfig.h" //see other tab
Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN); //see other tab

//ECG Variables
const int ECG = A7; //May need to change based on flora pin chosen
int sensorValue = 0;
const int LEDpin = 9;
const int interval = 10;
int sensorAvg = 0;
int sensorSum = 0;
int count = 0;

//Threshold variables
int potVal = 0;
const int potPin = A10;
int threshold = 0;
int pulse = 0;

//HR Variables
long prevBeatTime = 0;
float timeGap = 0.5;
long beatTime = 0;
float beatInterval = 0;
int beatCount = 0;
float beatIntervalSum = 0;
float beatIntervalAvg = 0;
int timeCount = 0;
int HR =0;




void setup() {
  //When using computer to record Data (uncomment if using computer)
  //Serial.begin(115200);

  //When using Blutooth to record data (uncomment if using Bluefruit)
  ble.begin(115200);
  pinMode (LEDpin, OUTPUT);
  digitalWrite(LEDpin, LOW);
}

void loop() {
  //Read the potentiometer
  potVal = analogRead(potPin);
  threshold = potVal;

  //Read ECG
  sensorValue = analogRead(ECG);

  //add current reading to the previous readings
  sensorSum = sensorValue + sensorSum;

  //add one to the "count" variable
  count = count +1;

  //When there are 10 Readings
  if(count == interval){

    //Take average
    sensorAvg = sensorSum/count;
    if(sensorAvg > threshold){
      
      
      //calculate time between beats
      beatTime = millis();
      delay(5);
      beatInterval = beatTime - prevBeatTime;
      beatIntervalSum = beatIntervalSum+ beatInterval;
      prevBeatTime = beatTime;

      //set pulse variable to equal the threshold so we can see a "spike" on the graph
      pulse = threshold;
      beatCount = beatCount +1;
      
    }
    else{
      pulse = 0;
    }
   if(beatCount == interval){

     beatIntervalAvg= beatIntervalSum/count;

     HR= 60000/beatIntervalAvg;
     HR = constrain(HR, 40, 150);

     beatCount = 0;
     beatIntervalSum = 0;
   }
   
   if(HR > 90){
    digitalWrite(LEDpin, HIGH);
   }// If the resting HR is above 90, or another preset HR, turn the LED on. HR number can be changed based off of chart in Instructable
   
   else{
    digitalWrite(LEDpin, LOW);
   }
   //Serial Print (Uncomment if connected to the computer)
   // Serial.print(HR);
  // Serial.print(", ");
  // Serial.print(pulse);
  // Serial.print(", ");
  // Serial.print(sensorAvg);
 //  Serial.print(", ");
  // Serial.println(threshold);
   
   //Bluetooth Print (uncomment if printing via bluetooth)
   ble.print(HR);
   ble.print(", ");
   ble.print(pulse);
   ble.print(", ");
   ble.print(sensorAvg);
   ble.print(", ");
   ble.println(threshold);

   sensorSum = 0;
   count = 0;

   delay(5);
  }

}

/*
ECG code

This code collects data from a BITalino ECG sensor. When the ECG signal crosses a threshold the LED flashes, indicating a heart beat. The threshold is set by a potentiometer. The SoftwareSerial library is used to enable the Arduino to communicate with the computer via Bluetooth to minimize electrical risk to the user. A moving average is used to smooth the ECG signal. Heart rate is calculated from time between logged heart beats.

The ECG sensor should be connected to analog pin 0 (A0).
A potentiometer should be connected to analog pin 1 (A1).
An LED should be connected to digital pin 9.

Created 4 December 2017
By Emily J. Farrar
Modified 4 January 2018
By Emily J Farrar

ENGR 375 Bioinstrumentation and Measurement
Messiah College Department of Engineering
*/

//include the SoftwareSerial library, which is needed to 
//use the Bluetooth module from pins 10 and 11
#include <SoftwareSerial.h>

//assign communication pins
SoftwareSerial BTSerial(10,11); 

//ECG variables
const int ECG = A0;
int sensorValue = 0;
const int LEDpin = 9;
const int interval = 10;
int sensorAvg = 0;
int sensorSum = 0;
int count = 0;

//threshold variables
int potVal = 0;
const int potPin = A1;
int threshold = 0;
int pulse = 0;

//HR variables
long prevBeatTime = 0;
float timeGap = 0.5;
long beatTime=0;
float beatInterval = 0;
int beatCount = 0;
float beatIntervalSum =0;
float beatIntervalAvg = 0;
int timeCount = 0;
int HR = 0;


void setup() {

//initialize serial comm at 115200 baud (BITalino default)
Serial.begin(115200);

//initialize bluetooth serial comm at 115200 baud
BTSerial.begin(115200); 

//set LEDpin (pin 9) to be an output
pinMode(LEDpin, OUTPUT);

//turn LED off
digitalWrite(LEDpin, LOW);

}

void loop() {

// read the potentiometer to set threshold value
potVal = analogRead(potPin);
threshold = potVal;

//read the signal from the ECG
sensorValue = analogRead(ECG);

//add the current reading to the previous readings
sensorSum = sensorValue+sensorSum;

//add one to the "count" variable
count = count +1;

//if 10 readings have been recorded (is count =10?)
if (count == interval){

  //take average of last 10 readings
  sensorAvg = sensorSum/count; 

//if average over 10 readings is higher than the threshold,
  if (sensorAvg > threshold){

    //turn LED on
    digitalWrite(LEDpin, HIGH);

    //calculate the time between this beat and the last beat
    beatTime = millis();
    delay(5);
    beatInterval = beatTime- prevBeatTime;
    beatIntervalSum = beatIntervalSum + beatInterval;
    prevBeatTime = beatTime;
    int val = beattimefunction(beatTime, prevBeatTime); 
    Serial.print(val); 
    delay(5); 
    

    //set pulse variable to equal the threshold, so we see
    //a "spike" on the graph
    pulse = threshold; 
    beatCount = beatCount +1;  
  }
  

  //if ECG is not higher than the threshold, LED off and pulse = 0
  else{
    digitalWrite(LEDpin, LOW);
    pulse= 0;
  }

//check if ten beats have been recorded, calculate HR
if(beatCount == interval){
  
  //calc avg seconds between beats
  beatIntervalAvg = beatIntervalSum/count; 
  
  //calculate beats per minute
  HR = 60000/beatIntervalAvg; 
  HR = constrain(HR, 40, 150);

  //reset beat counter
  beatCount = 0; 

  //reset beatInterval sum
  beatIntervalSum = 0; 
  }

//print readings via USB to serial monitor
Serial.print(HR);
Serial.print(", ");
Serial.print(pulse);
Serial.print(", ");
Serial.print(sensorAvg);
Serial.print(", ");
Serial.println(threshold);

//print readings via BT to serial monitor
BTSerial.print(HR);
BTSerial.print(", ");
BTSerial.print(pulse);
BTSerial.print(", ");
BTSerial.print(sensorAvg);
BTSerial.print(", ");
BTSerial.println(threshold);

//reset sensorSum and count variables
sensorSum = 0; 
count = 0; 
  
//wait 5 milliseconds for stability
delay(5);

}

}


//beatTime = millis();

int beattimefunction(int b,int p) {
    int diff;
    diff = b - p;
    return diff;
}

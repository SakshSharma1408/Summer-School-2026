#include <DHT.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MQ2_PIN 34
#define RED_PIN 25
#define GREEN_PIN 26
#define BLUE_PIN 27
#define BUZZER 14

int baseline=0;

void setColor(bool r,bool g,bool b){
  digitalWrite(RED_PIN,r);
  digitalWrite(GREEN_PIN,g);
  digitalWrite(BLUE_PIN,b);
}

void sendBluetoothAlert(String status,int gas,float temp,float hum){
  if(status=="SAFE"){
    SerialBT.println("SAFE");
    SerialBT.printf("Gas:%d\nTemp:%.1f C\nHumidity:%.1f%%\n\n",gas,temp,hum);
  }else if(status=="MODERATE"){
    SerialBT.println("WARNING");
    SerialBT.println("Moderate Smoke Detected");
    SerialBT.printf("Gas=%d\nCheck Fireplace\n\n",gas);
  }else{
    SerialBT.println("DANGER!!");
    SerialBT.println("Smoke Level Critical");
    SerialBT.printf("Gas=%d\nOpen Windows\nLeave Room if Necessary\n\n",gas);
  }
}

void setup(){
  Serial.begin(115200);
  SerialBT.begin("ESP32_AirMonitor");
  dht.begin();

  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(BLUE_PIN,OUTPUT);
  pinMode(BUZZER,OUTPUT);

  Serial.println("Stabilizing MQ-2...");
  delay(10000);

  long sum=0;
  for(int i=0;i<30;i++){
    sum+=analogRead(MQ2_PIN);
    delay(200);
  }
  baseline=sum/30;

  Serial.println("Timestamp, MQ-2 Value, DHT11 Temp, DHT11 Humidity, Severity Level");
}

void loop(){
  int gasRaw=analogRead(MQ2_PIN);
  int diff=gasRaw-baseline;

  float temp=dht.readTemperature();
  float hum=dht.readHumidity();

  if(isnan(temp)||isnan(hum)){
    Serial.println("DHT ERROR");
    delay(2000);
    return;
  }

  String status;

  if(diff<80){
    status="SAFE";
    setColor(LOW,HIGH,LOW);
    noTone(BUZZER);
  }else if(diff<250){
    status="MODERATE";
    setColor(HIGH,HIGH,LOW);
    tone(BUZZER,1000,200);
  }else{
    status="DANGER";
    setColor(HIGH,LOW,LOW);
    tone(BUZZER,2000);
  }

  int sec=millis()/1000;
  char ts[6];
  sprintf(ts,"%02d:%02d",sec/60,sec%60);

  Serial.print(ts);
  Serial.print(", ");
  Serial.print(gasRaw);
  Serial.print(", ");
  Serial.print(temp);
  Serial.print(", ");
  Serial.print(hum);
  Serial.print(", ");
  Serial.println(status);

  sendBluetoothAlert(status,gasRaw,temp,hum);

  delay(2000);
}
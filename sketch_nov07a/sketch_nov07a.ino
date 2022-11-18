#include <HttpClient.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include "DHT.h"


#define DHT11Pin 2
#define DHTType DHT11


const char* ssid = "0011004438670";
const char* password = "vietcombank";


// Server
#define MQTT_PORT 8883
#define MQTT_USER "group6iot"
#define MQTT_PASSWORD "group6iot"
#define MQTT_HOST "ab1c5c5665b041a2846f373a4b181509.s2.eu.hivemq.cloud"

#define MQTT_ENVIROMENT_CAPTURE "enviroment/capture"
#define MQTT_DEVICE_UPDATE_STATUS "device/update-status"



WiFiClient wifiClient;
PubSubClient client(wifiClient);

//Led
int sensorPin = A0;
int ledPin =9;
int current_ledState = LOW;
int last_ledState = LOW;

//Temperature and Humility
DHT HT(DHT11Pin,DHTType);

//Time
// unsigned long previousMillis = 0; 
// const long interval = 5000;



void setup_wifi() {
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void connect_to_broker() {
  while (!client.connected()) {
    String clientId = "ArduinoUno";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      client.subscribe(MQTT_ENVIROMENT_CAPTURE);
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      delay(2000);
    }
  }
}
 
void callback(char* topic, String* payload) {
//  Serial.println("-------new message from broker-----");
//  Serial.print("topic: ");
//  Serial.println(topic);
//  Serial.print("message: ");
//  Serial.println(payload);
}

void setup() {

  setup_wifi();
  client.setServer(MQTT_HOST, MQTT_PORT);
  //client.setCallback(callback);
  //connect_to_broker();
  
  //Led
  pinMode(ledPin, OUTPUT);

  //Temperature and Humility
  Serial.begin(9600);
  HT.begin();
  delay(1000); 
}
void readTempAndHumAndLed() {
    //Read Temp, Humi
   //unsigned long currentMillis = millis();
   //if(currentMillis - previousMillis >= interval){
    float humi;
    float tempC;
    float tempF;
    String data;

    humi = HT.readHumidity();
    tempC = HT.readTemperature();
    tempF = HT.readTemperature(true);

    if(isnan(humi) || isnan(tempC)){
      Serial.print("None");
    } 
    else {
      //previousMillis = currentMillis;
      Serial.println(humi,0);
      Serial.println(tempC,1);
    }

  //Read light
    int sensorValue = analogRead(sensorPin);
    Serial.println(sensorValue);
    if(sensorValue < 70){
      digitalWrite(ledPin,HIGH);
    }
    else{
      digitalWrite(ledPin,LOW);
    }

    delay(1000); 

//  data = "humidity=" + String(humi) + "|temperature=" + String(tempC) + "|light=" + String(sensorValue);
//  client.publish(MQTT_ENVIROMENT_CAPTURE, data.c_str());
}


//nhận payload gửi đến client.sub.... => nhận message để bật tắt

void loop() {
//  if (!client.connected()) {
//    connect_to_broker();
//    Serial.println("ERROR");
//  }
//  else{
      readTempAndHumAndLed();
  //}
  
}







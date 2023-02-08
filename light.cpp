#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wifi.h>
#include <Bounce2.h>
#include <string.h>

#define RED 26
#define YELLOW 25
#define GREEN 33
#define BUTTON_RED 27
#define BUTTON_YELLOW 14
#define BUTTON_GREEN 12
#define LDR 32


//const int _size = 2 * JSON_OBJECT_SIZE(20);
//StaticJsonDocument<_size> JSONPost;
//StaticJsonDocument<_size> JSONGet;

Bounce debouncer_red = Bounce();
Bounce debouncer_yellow = Bounce();
Bounce debouncer_green = Bounce();
const int ledPin =  2;      // the number of the LED pin
const int threshold =  40;      // the number of the LED pin
int ledState = LOW;             // ledState used to set the LED
int count_RED = 1;
int count_YELLOW = 1;
int count_GREEN = 1;
const char *ssid = "group15";
const char *password = "thisisapassword";
int OrderResR, OrderResY, OrderResG;
int Res = 0;
bool IsParty = 0;
bool IsAutoG = 0,IsAutoY=0,IsAutoR = 0;
bool IsOnG = 0, IsOnY = 0,IsOnR=0;

const String baseUrl = "http://group15.exceed19.online";

void Connect_Wifi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("OK! IP=");
    Serial.println(WiFi.localIP());
}

void GET_post(){
    DynamicJsonDocument doc(2048);
    const String urlP = baseUrl + "/party/";
    const String urlG = baseUrl + "/bulb/0/";
    const String urlY = baseUrl + "/bulb/1/";
    const String urlR = baseUrl + "/bulb/2/";
    HTTPClient http;
    http.begin(urlP);
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200 || httpResponseCode == 201) {
        //Serial.print("HTTP ");
        //Serial.println(httpResponseCode);
        String payload = http.getString();
        deserializeJson(doc,payload);

        IsParty = doc["party"].as<bool>();
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    http.begin(urlG);
    httpResponseCode = http.GET();
    if (httpResponseCode == 200 || httpResponseCode == 201) {
        //Serial.print("HTTP ");
        //Serial.println(httpResponseCode);
        String payload = http.getString();
        deserializeJson(doc,payload);

        IsAutoG = doc["is_auto"].as<bool>();
        IsOnG = doc["is_on"].as<bool>();
        OrderResG = doc["light_level"].as<int>();
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    http.begin(urlY);
    httpResponseCode = http.GET();
    if (httpResponseCode == 200 || httpResponseCode == 201) {
        //Serial.print("HTTP ");
        //Serial.println(httpResponseCode);
        String payload = http.getString();
        deserializeJson(doc,payload);

        IsAutoY = doc["is_auto"].as<bool>();
        IsOnY = doc["is_on"].as<bool>();
        OrderResY = doc["light_level"].as<int>();
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    http.begin(urlR);
    httpResponseCode = http.GET();
    if (httpResponseCode == 200 || httpResponseCode == 201) {
        //Serial.print("HTTP ");
        //Serial.println(httpResponseCode);
        String payload = http.getString();
        deserializeJson(doc,payload);

        IsAutoR = doc["is_auto"].as<bool>();
        IsOnR = doc["is_on"].as<bool>();
        OrderResR = doc["light_level"].as<int>();
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
}

void Light_RED () {
  if(touchRead(T7)<=25 && count_RED==0){
    ledcWrite(0,OrderResR);
  }
  else if(touchRead(T7)<=25 && count_RED==1){
    ledcWrite(0,0);
  }
  else if(touchRead(T7)>=40 && count_RED==0){
    count_RED = 1;
  }
  else if(touchRead(T7)>=40 && count_RED==1){
    count_RED = 0;
  }
}

void Light_YELLOW () {
  if(touchRead(T6)<=25 && count_YELLOW==0){
    ledcWrite(1,OrderResY);
  }
  else if(touchRead(T6)<=25 && count_YELLOW==1){
    ledcWrite(1,0);
  }
  else if(touchRead(T6)>=40 && count_YELLOW==0){
    count_YELLOW = 1;
  }
  else if(touchRead(T6)>=40 && count_YELLOW==1){
    count_YELLOW = 0;
  }
}

void Light_GREEN () {
  if(touchRead(T5)<=25 && count_GREEN==0){
    ledcWrite(2,OrderResG);
  }
  else if(touchRead(T5)<=25 && count_GREEN==1){
    ledcWrite(2,0);
  }
  else if(touchRead(T5)>=40 && count_GREEN==0){
    count_GREEN = 1;
  }
  else if(touchRead(T5)>=40 && count_GREEN==1){
    count_GREEN = 0;
  }
}

void Auto_RED(int Light){
  if (Light<180){
    ledcWrite(0,OrderResR);
    delay(100);
  }
  else{
    ledcWrite(0,0);
    delay(100);
  }
}

void Auto_YELLOW(int Light){
  if (Light<180){
    ledcWrite(1,OrderResR);
    delay(100);
  }
  else{
    ledcWrite(1,0);
    delay(100);
  }
}

void Auto_GREEN(int Light){
  if (Light<180){
    ledcWrite(2,OrderResR);
    delay(100);
  }
  else{
    ledcWrite(2,0);
    delay(100);
  }
}

void Is_Party(){
  ledcWrite(0,255);
  ledcWrite(1,0);
  ledcWrite(2,255);
  delay(100);
  ledcWrite(0,0);
  ledcWrite(1,255);
  ledcWrite(2,0);
  delay(100);
  ledcWrite(0,255);
  ledcWrite(1,255);
  ledcWrite(2,0);
  delay(100);
  delay(100);
  ledcWrite(0,0);
  ledcWrite(1,255);
  ledcWrite(2,255);
  delay(100);
  delay(100);
  ledcWrite(0,255);
  ledcWrite(1,255);
  ledcWrite(2,255);
  delay(100);
  delay(100);
  ledcWrite(0,0);
  ledcWrite(1,0);
  ledcWrite(2,0);
  delay(100); 
}

void setup() {
  // put your setup code here, to run once:
  //pinMode(ledPin, OUTPUT);
  Connect_Wifi();
  Serial.begin(115200);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(RED, 0);
  ledcSetup(1, 5000, 8);
  ledcAttachPin(YELLOW, 1);
  ledcSetup(2, 5000, 8);
  ledcAttachPin(GREEN, 2);
}

void loop() {  
  GET_post();
  int Light = map(analogRead(LDR),500,3600,0,255);
  if (IsParty){
    Is_Party();
  }
  else{
    if(IsAutoR){
      Auto_RED(Light);
    }
    else{
      touchAttachInterrupt(T7 , Light_RED, threshold );
    }
    if(IsAutoY){
      Auto_YELLOW(Light);
    }
    else{
      touchAttachInterrupt(T6 , Light_YELLOW, threshold );
    }
    if(IsAutoG){
      Auto_GREEN(Light);
    }
    else{
      touchAttachInterrupt(T5 , Light_GREEN , threshold );
    }
  }
}




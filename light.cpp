//ต้องไม่ใช้loop แยกเป็น2 task
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#define BUTTON7 27
#define BUTTON6 14
#define BUTTON5 12
#define RED 26
#define YELLOW 25
#define GREEN 33
#define LDR 32
const char *ssid = "group15";
const char *password = "thisisapassword";
int OrderResR = 0, OrderResY = 0, OrderResG = 0;
int Res = 0;
bool IsParty = 0;
bool IsAutoG = 0,IsAutoY=0,IsAutoR = 0;
bool IsOnG = 0, IsOnY = 0,IsOnR=0;
Bounce debouncer = Bounce();

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

void setup() {
    Connect_Wifi();
    Serial.begin(115200);
    debouncer.attach(BUTTON7, INPUT_PULLUP);
    debouncer.attach(BUTTON6, INPUT_PULLUP);
    debouncer.attach(BUTTON5, INPUT_PULLUP);
    debouncer.interval(35); 
    ledcSetup(0, 5000, 8);
    ledcAttachPin(GREEN, 0);
    ledcSetup(1, 5000, 8);
    ledcAttachPin(YELLOW, 1);
    ledcSetup(2, 5000, 8);
    ledcAttachPin(RED, 2);


    delay(1000);
}

void loop() {
    GET_post();
    debouncer.update();
    int Light = map(analogRead(LDR),500,3600,0,255);
    Res = 250-Light;
    if (IsParty){
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
        ledcWrite(0,0);
        ledcWrite(1,255);
        ledcWrite(2,255);       
        delay(100);
        ledcWrite(0,255);
        ledcWrite(1,255);
        ledcWrite(2,255);        
        delay(100);
        ledcWrite(0,0);
        ledcWrite(1,0);
        ledcWrite(2,0);
        delay(100);        
    } else {
        //Grean
        if (IsAutoG){
            ledcWrite(0,Res);
            delay(100);
        } else if (IsOnG){
                ledcWrite(0,OrderResG);
                delay(100);
            } else {
                ledcWrite(0,0);
                delay(100);
            }
        
        //Yellow
        if (IsAutoY){
            ledcWrite(1,Res);
            delay(100);
        } else if (IsOnY){
                ledcWrite(1,OrderResY);
                delay(100);
            } else {
                ledcWrite(1,0);
                delay(100);
            }
        
        //Red
        if (IsAutoR){
            ledcWrite(2,Res);
            delay(100);
        } else if (IsOnR){
                ledcWrite(2,OrderResR);
                delay(100);
            } else {
                ledcWrite(2,0);
                delay(100);
            }
        
    }
    delay(100);
}




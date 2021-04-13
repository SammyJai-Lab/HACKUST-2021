//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"	// Install Firebase ESP8266 library

#include <ESP8266WiFi.h>


#define FIREBASE_HOST "fir-iot-85fdf-default-rtdb.firebaseio.com/" //Without http:// or https:// schemes
#define FIREBASE_AUTH "sG8YYCucVC9Xjd0503QDNCJybTLKBAFPpANTTwN0"
#define WIFI_SSID "printer"
#define WIFI_PASSWORD "12345678"

int BUTPIN = D1;
int led = D5; // Connect LED to D5

int buttonState = 0;
int buttonCount = 0;

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;

FirebaseJson json;

void setup() {

    Serial.begin(9600);

    pinMode(BUTPIN, INPUT);
    pinMode(led, OUTPUT);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

}

void sensorUpdate() {
    
    buttonState = digitalRead(BUTPIN);
    if (buttonState == HIGH) {
        buttonCount++;
    }

    if (Firebase.setInt(firebaseData, "/FirebaseIOT/buttonState", buttonState)) {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.println("TYPE: " + firebaseData.dataType());
        Serial.println("ETag: " + firebaseData.ETag());
        Serial.println("------------------------------------");
        Serial.println();
    } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }

    if (Firebase.setInt(firebaseData, "/FirebaseIOT/buttonCount", buttonCount)) {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.println("TYPE: " + firebaseData.dataType());
        Serial.println("ETag: " + firebaseData.ETag());
        Serial.println("------------------------------------");
        Serial.println();
    } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }

    if (Firebase.getString(ledData, "/FirebaseIOT/led")) {
        Serial.println(ledData.stringData());
        if (ledData.stringData() == "1") {
            digitalWrite(led, HIGH);
        } else if (ledData.stringData() == "0") {
            digitalWrite(led, LOW);
        }
    } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + ledData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }

}

void loop() {
    sensorUpdate();


    delay(10);
}

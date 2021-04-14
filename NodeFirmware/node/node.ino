#include "FirebaseESP8266.h" //Including firebase library to enable the ability for the node to connect to the firebase

#include <ESP8266WiFi.h> //ESP8266WiFi.h from "http://arduino.esp8266.com/stable/package_esp8266com_index.json"

//Libraries for MFRC522 reader
#include <SPI.h>
#include <MFRC522.h>


#define FIREBASE_HOST "fir-iot-85fdf-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "sG8YYCucVC9Xjd0503QDNCJybTLKBAFPpANTTwN0"
#define WIFI_SSID "printer"
#define WIFI_PASSWORD "12345678"

const int PRESS_THRESHOLD = 7000;

constexpr uint8_t RST_PIN = D3;
constexpr uint8_t SSR_PIN = D4;

MFRC522 rfid(SSR_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

String tag;

int button = D1;
int ledRed = D8;
int ledGreen = D2;

int buttonState = 0; //State of button on the main module. 1 = pressed - 0 = not pressed
int ledRedState = 0; //State of red led on the main module. 1 = Lights on - 0 = Lights off
int ledGreenState = 0; //State of green led on the main module. 1 = Lights on - 0 = Lights off

boolean buttonActive = false;
boolean longPressActive = false;

long buttonTimer = 0;
long longPressTime = 2500;

bool serveState = false;

FirebaseData firebaseData;
FirebaseData ledData;

FirebaseJson json;

//TO-DO Button not doing long press and short press right...
//https://www.instructables.com/Arduino-Dual-Function-Button-Long-PressShort-Press/

void setup() {

    Serial.begin(9600);

    SPI.begin();
    rfid.PCD_Init();

    pinMode(button, INPUT);
    pinMode(ledRed, OUTPUT);
    pinMode(ledGreen, OUTPUT);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    int timer = 0;
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".Connecting to wifi --timer: ");
        Serial.println(timer);
        timer += 300;
        delay(300);
    }

    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

}

void shortPress() {
    if (Firebase.setInt(firebaseData, "/FirebaseIOT/serveState", 1)) {
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

    if (Firebase.setString(ledData, "/FirebaseIOT/ledRed", "1")) {
        Serial.println("PASSED");
        Serial.println("PATH: " + ledData.dataPath());
        Serial.println("TYPE: " + ledData.dataType());
        Serial.println("ETag: " + ledData.ETag());
        Serial.println("------------------------------------");
        Serial.println();
    } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + ledData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }

    if (Firebase.setInt(firebaseData, "/FirebaseIOT/buttonCount", Firebase.getInt(firebaseData, "/FirebaseIOT/buttonCount") + 1)) {
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
}

void longPress() {
    //When button is pressed for 7 seconds
    //Send Reset Pending signal to backend
    if (Firebase.setInt(firebaseData, "/FirebaseIOT/ResetPending", 1)) {
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
}

void buttonUpdate() {
    if (digitalRead(button) == HIGH) {
		if (buttonActive == false) {
			buttonActive = true;
			buttonTimer = millis();
		}
		if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
			longPressActive = true;
		}
	} else {
		if (buttonActive == true) {
			if (longPressActive == true) {
                longPress();
                Serial.println("Long Press Detected");
				longPressActive = false;
			} else {
				shortPress();
                Serial.println("Short Press Detected");
			}
			buttonActive = false;
		}
	}
}

void ledUpdate() {
    if (Firebase.getString(ledData, "/FirebaseIOT/ledRed")) {
        Serial.println(ledData.stringData());
        if (ledData.stringData() == "1") {
            digitalWrite(ledRed, HIGH);
        } else if (ledData.stringData() == "0") {
            digitalWrite(ledRed, LOW);
        }
        Serial.println("LED RED DATA FETCH SUCCESS");
        Serial.println("LED STATE: " + ledData.stringData());
        Serial.println("------------------------------------");
        Serial.println();
    } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + ledData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }

    if (Firebase.getString(ledData, "/FirebaseIOT/ledGreen")) {
        Serial.println(ledData.stringData());
        if (ledData.stringData() == "1") {
            digitalWrite(ledGreen, HIGH);
        } else if (ledData.stringData() == "0") {
            digitalWrite(ledGreen, LOW);
        }
        Serial.println("LED GREEN DATA FETCH SUCCESS");
        Serial.println("LED STATE: " + ledData.stringData());
        Serial.println("------------------------------------");
        Serial.println();
    } else {
        Serial.println("LED GREEN DATA FETCH FAILED");
        Serial.println("REASON: " + ledData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }

}

void rfidSensorUpdate() {
    if ( ! rfid.PICC_IsNewCardPresent())
        return;
    if (rfid.PICC_ReadCardSerial()) {
        for (byte i = 0; i < 4; i++) {
            tag += rfid.uid.uidByte[i];
        }
        Serial.println(tag);

        if (Firebase.getInt(firebaseData, "/FirebaseIOT/serveState")) {
            Serial.println("PASSED");
        }
        if (firebaseData.intData() == 1) {
            if (Firebase.setInt(firebaseData, "/FirebaseIOT/serveState", 0)) {
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

            if (Firebase.setString(ledData, "/FirebaseIOT/ledRed", "0")) {
                Serial.println("PASSED");
                Serial.println("PATH: " + ledData.dataPath());
                Serial.println("TYPE: " + ledData.dataType());
                Serial.println("ETag: " + ledData.ETag());
                Serial.println("------------------------------------");
                Serial.println();
            } else {
                Serial.println("FAILED");
                Serial.println("REASON: " + ledData.errorReason());
                Serial.println("------------------------------------");
                Serial.println();
            }
            if (Firebase.pushString(firebaseData, "/FirebaseIOT/serveHistory", tag)) {
                Serial.println("PASSED");
                Serial.println("------------------------------------");
                Serial.println();
            } else {
                Serial.println("FAILED");
                Serial.println("REASON: " + firebaseData.errorReason());
                Serial.println("------------------------------------");
                Serial.println();
            }
        } else {
            if (Firebase.pushString(firebaseData, "/FirebaseIOT/foodDelivery", tag)) {
                Serial.println("PASSED");
                Serial.println("------------------------------------");
                Serial.println();
            } else {
                Serial.println("FAILED");
                Serial.println("REASON: " + firebaseData.errorReason());
                Serial.println("------------------------------------");
                Serial.println();
            }
        }

        tag = "";
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
    }
}

void stateUpdate() {
    int x = 1;
}

void loop() {
    buttonUpdate();
    ledUpdate();
    rfidSensorUpdate();
    stateUpdate();
    
}

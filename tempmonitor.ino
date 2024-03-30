#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "DHT.h"

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define WIFI_SSID "Rafsanjani"
#define WIFI_PASSWORD "12341234"
#define API_KEY "AIzaSyDTiSPMtYjCvtxyTZh2EApYBUlPGVpd6e0"
#define DATABASE_URL "https://tempmonitor-267d5-default-rtdb.asia-southeast1.firebasedatabase.app" 

#define DHTPIN 15
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // dht start
  dht.begin(); 
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    //read DHT sensor
    float HUM = dht.readHumidity();
    float SUHU = dht.readTemperature();

    if(isnan(SUHU) || isnan(HUM)){
      Serial.println("gagal membaca dari DHT!");
    }else{
      Serial.print("kelembaban: "); Serial.print(HUM);
      Serial.print("suhu: ");Serial.println(SUHU);
    }

    // Write to firebase temperature data
    if (Firebase.RTDB.setFloat(&fbdo, "tempmonitor/temp", SUHU)){
      Serial.println("Berhasil!");
      Serial.println("Path: " + fbdo.dataPath());
      Serial.println("Tipe: " + fbdo.dataType());
    }
    else {
      Serial.println("Gagal!");
      Serial.println("Alasan: " + fbdo.errorReason());
    }
    count++;
    
    // Write to firebase humidity data
    if (Firebase.RTDB.setFloat(&fbdo, "tempmonitor/humi", HUM)){
      Serial.println("Berhasil!");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("Tipe: " + fbdo.dataType());
    }
    else {
      Serial.println("Gagal!");
      Serial.println("Alasan: " + fbdo.errorReason());
    }
  }
}

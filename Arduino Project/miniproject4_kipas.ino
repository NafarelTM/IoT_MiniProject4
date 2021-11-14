#include <DHT.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "Panjijaya"
#define WIFI_PASSWORD "0274383771"

#define FIREBASE_HOST "https://iot-miniproject4-5fd7b-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "IvOeAQLGFJ9I5kH9zEs1GnrpspjituLqwX4Tm1yr"

//DHT 11
#define DHTPIN 23
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);  

//RELAY
#define RELAYPIN 22

//Define FirebaseESP32 data object
FirebaseData dataFirebase;

void conn() {
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

  Firebase.setReadTimeout(dataFirebase, 1000 * 60);
  Firebase.setwriteSizeLimit(dataFirebase, "tiny");
}

int suhu_value;
int suhu_limit;
int on_off;

void sensorSuhu() {
  suhu_value = dht.readTemperature();
  Serial.print("Nilai Suhu: ");
  Serial.println(suhu_value);
  Firebase.setInt(dataFirebase, "/kipas_mp4/suhu_value", suhu_value);
  delay(1000);
  Firebase.getInt(dataFirebase, "/kipas_mp4/suhu_limit");
  suhu_limit = dataFirebase.intData();
  
  if(suhu_value > suhu_limit) {
    digitalWrite(RELAYPIN, LOW);
  }
  else if(suhu_value <= suhu_limit) {
    digitalWrite(RELAYPIN, HIGH);
  }
}

void setup() {  
  Serial.begin(9600);
  conn(); 
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);

}

void loop() {
  Firebase.getInt(dataFirebase, "/kipas_mp4/on_off");
  on_off = dataFirebase.intData();
  
  if(on_off == true) {
    sensorSuhu();
  }
  else if(on_off == false) {
    digitalWrite(RELAYPIN, HIGH);
  }
}

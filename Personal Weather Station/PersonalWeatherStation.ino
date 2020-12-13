#include <DHT.h>
#include<Wire.h>
#include<Adafruit_BMP085.h>
#include<Adafruit_Sensor.h>
#include<ESP8266WiFi.h>
#include"ThingSpeak.h"


//DHT pin and type declaration
#define DHTPIN 2
#define DHTTYPE DHT11

//create dht and bmp object
Adafruit_BMP085 bmp;
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

unsigned long myChannelNumber = 1171696;
const char * myWriteAPIKey = "NLQAW0MDW2HF4E1K";

void setup(){
  //baudrate of data streaming 
  Serial.begin(9600);

  //initiate thingspeak and wifi mode
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  
  //sensor test
  dht.begin();
  while(!bmp.begin())
     Serial.println("Sensor don't Recognize");
     
  //*tambahin fitur display serial print
  
  Serial.println("Recognize");
  Serial.print("Temperature "); Serial.print("(*C)"); Serial.print("  "); Serial.print("Humidity "); Serial.println("(%)");
}

void loop(){
  delay(2000);
  wifiConnection();
  float temp = bmp.readTemperature();
  float hum = dht.readHumidity();
  if(isnan(temp) || isnan(hum)){
    if(isnan(hum))
      Serial.println("Please check dht wiring!!");
    if(isnan(temp))
      Serial.println("Please check bmp wiring!!");
  }
  else{
    Serial.print(temp); Serial.print("              "); Serial.println(hum);
  }

  // set the fields with the values
  ThingSpeak.setField(1, hum);
  ThingSpeak.setField(2, temp);


  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}


void wifiConnection(){
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println("71Sumedang");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin("71Sumedang", "23462071");
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected");
  }
}

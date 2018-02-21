#include <featherOLED.h>
#include <noaa.h>
#include <WiFiManager.h>

#define sclk 14
#define mosi 13
#define cs 15
#define updateDelay 100000 // Time between http requests
const int DCpin =  4;
const int RSTpin =  5;
featherOLED oled = featherOLED(sclk, mosi, cs, DCpin, 128, 128); //sclk, mosi, cs, DCpin, max x, max y



void setup() {
  pinMode(DCpin, OUTPUT);
  pinMode(sclk, OUTPUT);
  pinMode(mosi, OUTPUT);
  pinMode(RSTpin, OUTPUT);
  pinMode(cs, OUTPUT);
  digitalWrite(cs, LOW);   
  digitalWrite(RSTpin, HIGH);
  delay(500);
  digitalWrite(RSTpin, LOW);
  delay(500);
  digitalWrite(RSTpin, HIGH);
  delay(500);

  //Initialize the OLED
  oled.init();
  oled.blank();

  //Start here
  Serial.begin(115200);
  Serial.println("Starting wifi manager...");
  WiFiManager wifiManager;
  // Uncomment for testing wifi manager
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback); // The function to call before the wifi manager config is launched (add OLED notification)
  wifiManager.autoConnect("Winds-Instrument", "whitewolf"); // Setup the wifi manager access point
  
}

void loop() {

  uint8_t Red = 31, Green = 63, Blue = 31; //Max Red = 31, Max Green = 63, MaxBlue = 31
  int test = 9;
  
  oled.print(0,0,Red,Green,Blue,"PRESIDIO");
  oled.paginate(120,3,1);
  
  
  //Setup the NOAA stations
  NOAA presidio(9414290,1);
  presidio.update();
  Serial.print("Current wind speed (knots): ");
  Serial.println(presidio.getSpeed(0));
  Serial.print("Current wind direction (degrees): ");
  Serial.println(presidio.getDirection());
  Serial.print("Current wind gust (knots): ");
  Serial.println(presidio.getGust());
  
  //CHECK THE MEMORY USAGE
  Serial.println(ESP.getFreeHeap());

  long next = millis() + updateDelay;
  while(millis()<next){
    Serial.print("Seconds until next update: ");
    Serial.println((next-millis())/1000);
    delay(10000);
  }
}

void configModeCallback(WiFiManager *myWiFiManager) {
  //Code to show the wifi configuration settings on the OLED goes here
  Serial.println("Wifi manager callback received.");
}

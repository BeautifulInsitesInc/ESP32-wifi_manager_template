#include "main.h"
#include "lcd_functions.h"
#include "wifi_manager.h"




void setup()
{
  Serial.begin(115200);
  out("Starting");
  
  #ifdef LCD_DISPLAY
    lcdInit();
  #endif

  
  
  wifiManagerSetup(); // WiFi Manager, SPIFF uploader, OTA Updates

  
}

unsigned long voltageStart = millis();
void loop()
{
  wifiManagerLoop();
  #ifdef LCD_DISPLAY
    lcdBrightness();
  #endif

  /*
  //Check battery pin
  if (millis() >= voltageStart+10000){
    voltageStart = millis();
    int voltage = analogRead(35)*2;
    out("Battery Level = "); out(voltage); out(" /1135: "); out(voltage/1135);
  }
  */
}
#include "main.h"
#include "lcd_functions.h"
#include "wifi_manager.h"




void setup()
{
  Serial.begin(115200);
  Serial.print("Starting");
  
  #ifdef LCD_DISPLAY
    lcdInit();
  #endif

  
  
  wifiManagerSetup(); // WiFi Manager, SPIFF uploader, OTA Updates

  
}

void loop()
{
  wifiManagerLoop();
  #ifdef LCD_DISPLAY
    lcdBrightness();
  #endif
}
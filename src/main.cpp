#include "main.h"




void setup()
{
  Serial.begin(115200);
  Serial.println("Starting");
  Serial.print("Starting");
  
  #ifdef LCD_DISPLAY
    lcdInit();
  #endif
  
  wifiManagerSetup(); // WiFi Manager, SPIFF uploader, OTA Updates

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
    
  server.serveStatic("/", SPIFFS, "/");

  AsyncElegantOTA.begin(&server);
  server.begin();
  TelnetServer.begin();
  initWebSocket();

  digitalWrite(LED_BUILTIN, LOW);


  
  
  //telnetSetup();
  
}


void loop()
{
  wifiManagerLoop();
  #ifdef LCD_DISPLAY
    lcdBrightness();
  #endif
  telnetLoop();
  batteryLevel();

  
}
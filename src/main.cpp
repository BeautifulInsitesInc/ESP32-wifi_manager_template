#include <Arduino.h>
//#include <FS.h>  //this needs to be first, or it all crashes and burns..
#include "wifi_manager_functions.h"
//#include <SPIFFS.h>
//#include <ArduinoJson.h>





void setup() {
    Serial.begin(115200);
    
    setup_wifi_manager();
   
    
}

void loop() {
    //wm.process();
    //if(wm_nonblocking) wm.process(); // avoid delays() in loop when non-blocking and other long running code  
    wifi_manager_check_for_reset();
    
}



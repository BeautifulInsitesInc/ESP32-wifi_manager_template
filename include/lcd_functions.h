#ifndef LCD_FUNCTIONS_H // This is so if it included in mulitple cpp files, the code will not be duplicated
#define LCD_FUNCTIONS_H

#include <SPIFFS.h>
#include <TJpg_Decoder.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <FS.h> // for ili9488  // The SPIFFS (FLASH filing system) is used to hold touch screencalibration data
#define FS_NO_GLOBALS

#define TTGO_240x135 // use for the TTGO ESP32 board
#define LCD_BL_PIN 4 // LED backlight pin to turn on or off
#define LCD_BUTTON 0


int time_screentime = 120; //time for screen backlight to turn off in seconds.
unsigned long timer_start_screentime = 0; // to record mill at start of timer for screen shut off
int brightness_level = 100;

#define LCD_DISPLAY // Used to include LCD libraries - comment Serial.print to turn off

#ifdef LCD_DISPLAY
  #define lcdout(x) tft.print(x)
  #define lcdoutln(x) tft.println(x)
#else
  #define lcdout(x)
  #define lcdoutln(x)
#endif


TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
#define TFT_GREY 0x5AEB // New colour



// ============ DISPLAYING JPEGS ======================
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){// render each block to the TFT.
  if ( y >= tft.height() ) return 0;// Stop further decoding as image is running off bottom of screen
  tft.pushImage(x, y, w, h, bitmap);  // This function will clip the image block rendering automatically at the TFT boundaries
  return 1;// Return 1 to decode next block
}

void spiffInit(){
    if (!SPIFFS.begin()) { //  Initialize SPIFFS
        outln("SPIFFS init failed");
        return;
    }
    outln("SPIFFS init finished");
}

void spiffListFiles(){//list contents of SPIFF
    File root = SPIFFS.open("/");
    File listing = root.openNextFile();
    while(listing){
        outln("FILE: ");
        outln(listing.name());
        listing = root.openNextFile();
    }
}

void loadFile(const char *name){
  //tft.fillScreen(TFT_RED);
  uint32_t t = millis();// Time recorded for test purposes
  uint16_t w = 0, h = 0, scale;// Get the width and height in pixels of the jpeg if you wish
  TJpgDec.getFsJpgSize(&w, &h, name); 
  tft.setRotation(w > h ? 1 : 0);
  for (scale = 1; scale <= 8; scale <<= 1) {
    if (w <= tft.width() * scale && h <= tft.height() * scale) break;
  }
  TJpgDec.setJpgScale(scale);
  TJpgDec.drawFsJpg(0, 0, name);// Draw the image, top left at 0,0
  t = millis() - t;  // How much time did rendering take

  char buf[80];
  sprintf(buf, "%s %dx%d 1:%d %u ms", name, w, h, scale, t);
  //tft.setCursor(0, tft.height() - 8);
  //tft.print(buf);
  outln(buf);
  //delay(2000);
}

void lcdSplashScreen(){
  
  loadFile("/si_ttgo240x135d.jpg");
  delay(500);
  loadFile("/si_ttgo240x135.jpg");
  delay(1000);
  /*
    tft.fillScreen(TFT_RED);
    uint32_t t = millis();  // Time recorded for test purposes
    uint16_t w = 0, h = 0;// Get the width and height in pixels of the jpeg if you wish
    TJpgDec.getFsJpgSize(&w, &h, "/si_ttgo240x135.jpg"); // Note name preceded with "/"
    outln("Width = "); outln(w); outln(", height = "); outln(h);
    TJpgDec.drawFsJpg(0, 0, "/si_ttgo240x135.jpg");  // Draw the image, top left at 0,0
    t = millis() - t;  // How much time did rendering take (ESP8266 80MHz 271ms, 160MHz 157ms, ESP32 SPI 120ms, 8bit parallel 105ms
    outln(t); outln(" ms");
    delay(5000);// Wait before drawing again
  */
}



void lcdClearText(){
  tft.setRotation(1);
  tft.setCursor(0, 10, 2);// X,Y, Font
  tft.setTextColor(TFT_BLACK,TFT_WHITE);  tft.setTextSize(2);
  tft.println("       ");
  tft.setTextSize(1);
  tft.println();
  tft.println("             ");
  tft.println("             ");
  tft.println("             ");
  tft.setCursor(0, 10, 2); tft.setTextSize(2);
}



void lcdSetBrightness(uint32_t newBrightness){
  ledcWrite(0, newBrightness); // 0-15, 0-255 (with 8 bit resolution);  0=totally dark;255=totally shiny
}



void lcdInit(){
  tft.init();
  spiffInit();
  //spiffListFiles();
  tft.setRotation(1);
  tft.begin();
  //tft.setTextColor(0xFFFF, 0x0000);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  TJpgDec.setJpgScale(1);// The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setCallback(tft_output);  // The decoder must be given the exact name of the rendering function above
  lcdSplashScreen();
  //tft.init();

  tft.fillScreen(TFT_BLACK); 

  loadFile("/sidelogo.jpg");
  delay(2000);

  lcdClearText();
  
  //tft.println("Starting...");
  //delay(1000);
  //tft.println("It's Alive!");
  
  timer_start_screentime = millis();
  //pinMode(LCD_BL_PIN, INPUT_PULLUP);

  //Turn on LCD and controll intensity
  pinMode(TFT_BL, OUTPUT);
  ledcSetup(0, 5000, 8); // 0-15, 5000, 8
  ledcAttachPin(TFT_BL, 0); // TFT_BL, 0 - 15
  //ledcWrite(0, 1); // 0-15, 0-255 (with 8 bit resolution); 0=totally dark;255=totally shiny

  lcdSetBrightness(brightness_level);


}

void lcdBrightness(){

    if((millis() >= timer_start_screentime + (time_screentime*1000)) && brightness_level !=0) {
      lcdSetBrightness(100);
      delay(100);
      lcdSetBrightness(0);
      brightness_level = 0;
      timer_start_screentime = millis();
    }

    if (digitalRead(LCD_BUTTON) == 0){
      outln("Screen Button pressed onced Screen brightness is "); outln(brightness_level);
      delay(100);
      if (digitalRead(LCD_BUTTON) == 0){
        outln("Screen Button held");
        if(brightness_level == 0) brightness_level = 100;
        else if (brightness_level == 100) brightness_level = 255;
        else brightness_level = 0;
      }
    

    }
lcdSetBrightness(brightness_level);
//outln("Screen bright ness  set to : "); outln(brightness_level);
    
}

#endif
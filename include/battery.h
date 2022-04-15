#ifndef BATTERY_H
#define BATTERY_H


unsigned long voltageStart = millis();

void batteryLevel(){
  if (millis() >= voltageStart+10000){
    voltageStart = millis();
    int batteryRead = analogRead(35);
    float voltage = batteryRead * 3.3 / 4095;
    tout("Battery Read = "); tout(batteryRead); tout("    Voltage : "); toutln(voltage);
  }

}


#endif